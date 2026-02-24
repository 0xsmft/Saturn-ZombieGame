#include "sppch.h"
#include "GameState.h"

#include "AmmoCrateSpawner.h"
#include "HealthKit.h"

#include "EnemySpawnLocation.h"

#include "Saturn/Core/Random.h"

#include "Saturn/Alura/AluraCanvas.h"
#include "Saturn/Vulkan/AluraRenderer.h"

#include "Saturn/GameFramework/GameFramework.h"

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::BeginPlay()
{
	Super::BeginPlay();

	SetupGameState();

	m_TickSound = AsRequestSound2D( 8050284560379928628llu );
}

void GameState::OnUpdate( Saturn::Timestep ts )
{
	TickGameState( ts );
	DrawGameStateUI();
}

void GameState::OnPhysicsUpdate( Saturn::Timestep ts )
{
}

void GameState::TickGameState( Timestep ts )
{
	m_TimeUntilNextGameState -= ts;

	if( m_TimeUntilNextGameState <= 0 )
	{
		switch( m_GameState )
		{
			case GameStateStage::WaveInProgress:
			{
				// Go to next wave.
				// NOTE: We may chance state after this to EGameState::Intermission!
				AdvanceToNextWave();
			} break;
			
			case GameStateStage::Intermission:
			{
				// If we are currently in intermission and the time is up, set state to waves and begin spawning again.
				m_GameState = GameStateStage::WaveInProgress;

				AdvanceToNextWave();
			} break;

			case GameStateStage::Init:
			case GameStateStage::Dead:
			default:
				break;
		}
	}
	else if( m_TimeUntilNextGameState <= 3 )
	{
		if( !m_TickSound->IsPlaying() )
		{
			m_TickSound->Play();
		}
	}
}

void GameState::DrawGameStateUI()
{
	// Fix - API 3.0
#if !defined(SAT_DIST)
	if( m_ShowGameStateDebug )
	{
		g_AluraCanvas->PushFontSize( 32.0f );
		{
			g_AluraCanvas->AddText( "=== Game State Debug ===" );

			g_AluraCanvas->Indent();
			{
				// Fix - API 3.2
				g_AluraCanvas->AddText( std::format( "Current Wave: {0}", m_CurrentWave ) );
				g_AluraCanvas->AddText( std::format( "Wave Difficulty: {0}", m_WaveDifficulty ) );
				g_AluraCanvas->AddText( std::format( "Number Of Waves Since Last Intermission: {0}", m_NumberOfWavesSinceLastInt ) );
				g_AluraCanvas->AddText( std::format( "Time remaining: {0}s", m_TimeUntilNextGameState ) );
				g_AluraCanvas->AddText( std::format( "Game state: {0}", ( int ) m_GameState ) );
			}
			g_AluraCanvas->Unindent();
		}
		g_AluraCanvas->PopFontSize();

		return;
	}
#endif

	// Alura UI pass.
	g_AluraCanvas->PushFontSize( 32.0f );

	// Timer text
	{
		// Fix - API 3.2
		std::string text = std::format( "{0}s", std::ceil( m_TimeUntilNextGameState ) );
		// Fix - API 3.1
		const auto textSize = g_AluraCanvas->CalcTextSize( text );

		g_AluraCanvas->SetNextItemPosition( glm::vec2{ ( g_AluraCanvas->GetWidth() - textSize.x ) - g_AluraCanvas->GetStyle().ItemSpacing.x, g_AluraCanvas->GetStyle().ItemSpacing.y } );
		g_AluraCanvas->AddText( text );
	}

	// Game state text
	{
		// Fix - API 3.2
		std::string text{};

		switch( m_GameState )
		{
			case GameStateStage::WaveInProgress:
				text = std::format( "Wave {0}", m_CurrentWave );
				break;

			case GameStateStage::Intermission:
				text = "Intermission - prepare for next wave";
				break;

			case GameStateStage::Init:
			case GameStateStage::Dead:
			default:
				break;
		}

		// Fix - API 3.1
		const auto textSize = g_AluraCanvas->CalcTextSize( text );
		const auto pos = glm::vec2{ ( g_AluraCanvas->GetWidth() - textSize.x ) - g_AluraCanvas->GetStyle().ItemSpacing.x, g_AluraCanvas->GetCursorPosition().y };

		g_AluraCanvas->SetNextItemPosition( pos );
		g_AluraCanvas->AddText( text );
	}

	g_AluraCanvas->PopFontSize();
}

void GameState::SetupFirstWave()
{
#if defined(SAT_DEBUG)
	m_TimeUntilNextGameState = 3.0f;
#else
	m_TimeUntilNextGameState = 60.0f;
#endif
}

void GameState::SetupGameState()
{
	m_GameState = GameStateStage::Intermission;
	SetupFirstWave();
}

void GameState::AdvanceToNextWave()
{
	// At least five waves need to have happened for an intermission to happen (again)
	// and of course we need our RNG because who doesn't love that.
	if( m_NumberOfWavesSinceLastInt >= 5 && Random::RandomBool() )
	{
		m_GameState = GameStateStage::Intermission;
		m_NumberOfWavesSinceLastInt = 0;
		m_TimeUntilNextGameState = Random::RandomFloatInRange( 15.0f, 45.0f );

		// We need to make sure we replenish the ammo crates...
		ReplenishConsumables();

		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// NO INTERMISION, MOVE ON TO THE NEXT WAVE...

	++m_CurrentWave;
	++m_NumberOfWavesSinceLastInt;

	if( Random::RandomBool() && m_WaveDifficulty != 5 )
	{
		++m_WaveDifficulty;
	}

	m_TimeUntilNextGameState = 15.0f;

#if !defined(SAT_DIST)
	SAT_CORE_INFO( "Advanced to wave number: {0}", m_CurrentWave );
#endif

	auto enemySpawners = GetScene()->GetAllEntitiesWithClass<EnemySpawnLocation>();
	
	uint32_t numberToSpawn = ( 5 + m_CurrentWave * 2 ) + ( glm::pow( ( float ) ( 0.6f * m_CurrentWave ), 1.4f ) );
	numberToSpawn /= enemySpawners.size();

	for( auto& rEntity : enemySpawners )
	{
		// Should we add more enemies to spawn? hmmm lets ask the RNG...
		if( Random::RandomBool() )
		{
			const auto currentAmount = rEntity->GetNumberToSpawn();
			
			rEntity->SetNumberToSpawn( numberToSpawn );
#if defined(SAT_DEBUG)
			SAT_CORE_INFO( "Spawning {0} entities", rEntity->GetNumberToSpawn() );
#endif
		}

		rEntity->Spawn();
	}

	ReplenishConsumables();
}

void GameState::ReplenishConsumables()
{
	int numberOfVisibleSpawners = 0;

	auto ammoSpanwers = GetScene()->GetAllEntitiesWithClass<AmmoCrateSpawner>();
	for( auto& rEntity : ammoSpanwers )
	{
		rEntity->RequestRespawn();

		if( rEntity->IsVisible() )
			++numberOfVisibleSpawners;
	}

	// We must at least have one visible ammo crate.
	if( numberOfVisibleSpawners == 0 && ammoSpanwers.size() )
	{
		// RNG again to decide what spawner should become visible.
		ammoSpanwers[ Random::RandomElementInRange( 0, ammoSpanwers.size() - 1 ) ]->ForceSpawn();
	}

	// Spawn health kits
	auto healthKits = GetScene()->GetAllEntitiesWithClass<HealthKit>();
	for( auto& rEntity : healthKits )
	{
		rEntity->RequestRespawn();
	}
}
