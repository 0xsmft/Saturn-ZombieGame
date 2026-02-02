#include "sppch.h"
#include "GameState.h"

#include "AmmoCrateSpawner.h"
#include "EnemySpawnLocation.h"

#include "Saturn/Alura/AluraCanvas.h"
#include "Saturn/Vulkan/AluraRenderer.h"

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
}

void GameState::OnUpdate( Saturn::Timestep ts )
{
	m_TimeUntilNextWave -= ts;

	if( m_TimeUntilNextWave <= 0 )
	{
		AdvanceToNextWave();

		m_TimeUntilNextWave = 15.0f;
	}

	// Fix - API 3.0

	// Alura UI pass.
	g_AluraCanvas->PushFontSize( 32.0f );

	// Timer text
	{
		// Fix - API 3.2
		std::string text = std::format( "{0}s", std::ceil( m_TimeUntilNextWave ) );
		// Fix - API 3.1
		const auto textSize = g_AluraCanvas->CalcTextSize( text );

		g_AluraCanvas->SetNextItemPosition( glm::vec2{ ( g_AluraCanvas->GetWidth() - textSize.x ) - g_AluraCanvas->GetStyle().ItemSpacing.x, g_AluraCanvas->GetStyle().ItemSpacing.y } );
		g_AluraCanvas->AddText( text );
	}

	// Wave text
	{
		// Fix - API 3.2
		std::string text = std::format( "Wave {0}", m_CurrentWave );
		// Fix - API 3.1
		const auto textSize = g_AluraCanvas->CalcTextSize( text );

		auto pos = glm::vec2{ ( g_AluraCanvas->GetWidth() - textSize.x ) - g_AluraCanvas->GetStyle().ItemSpacing.x, g_AluraCanvas->GetCursorPosition().y };

		g_AluraCanvas->SetNextItemPosition( pos );
		g_AluraCanvas->AddText( text );
	}

	g_AluraCanvas->PopFontSize();
}

void GameState::OnPhysicsUpdate( Saturn::Timestep ts )
{
}

void GameState::SetupFirstWave()
{
	// Prep time
	m_TimeUntilNextWave = 10.0f;
//	m_TimeUntilNextWave = 60.0f;
}

void GameState::SetupGameState()
{
	SetupFirstWave();
}

void GameState::AdvanceToNextWave()
{
	++m_CurrentWave;

#if !defined(SAT_DIST)
	SAT_CORE_INFO( "Advanced to wave number: {0}", m_CurrentWave );
#endif

	auto enemySpawners = GetScene()->GetAllEntitiesWithClass<EnemySpawnLocation>();
	for( auto& rEntity : enemySpawners )
	{
		rEntity->IncrementNumberToSpawn();
		rEntity->Spawn();
	}
}
