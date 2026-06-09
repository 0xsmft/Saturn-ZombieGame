#include "sppch.h"
#include "GameState.h"

#include "Saturn/Scene/Scene.h"

#include "AmmoCrateSpawner.h"
#include "HealthKit.h"

#include "EnemySpawnLocation.h"

#include "GameStateHUD.h"

#include "Saturn/Core/Random.h"

#include "Saturn/Alura/AluraCanvas.h"

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

	m_GameStateHUD = Ref<GameStateHUD>::Create();
	g_AluraCanvas->AddDrawer( m_GameStateHUD );

	SetupGameState();
	m_TickSound = AsRequestSound2D( 8050284560379928628llu );
}

void GameState::OnUpdate( Saturn::Timestep ts )
{
	TickGameState( ts );
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

void GameState::SetupFirstWave()
{
#if defined(SAT_DEBUG)
	m_TimeUntilNextGameState = 3.0f;
#else
	m_TimeUntilNextGameState = 3.0f;
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
	
	uint32_t numberToSpawn = ( 5 + m_CurrentWave * 2 ) + ( glm::pow( ( float ) ( 0.6f * ( float ) m_CurrentWave ), 1.4f ) );
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
