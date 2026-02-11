#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "GameState.Gen.h"

using namespace Saturn;

/**
 * The actual game state...
 */
enum class EGameState
{
	// Wave 0
	Init,

	WaveInProgress,
	
	// Preparation time
	Intermission,
	
	// Player is dead
	Dead
};

/**
 * The game state entity controls the state of the game.
 * 
 * In this game it acts as the wave manager deciding how many entities should spawn 
 * and when the next wave should occur, it also keeps track of ammo boxes.
 */
SCLASS( VisibleInEditor )
class GameState : public Entity
{
	GENERATED_BODY()
public:
	GameState();
	~GameState();

public:
	void BeginPlay() override;
	void OnUpdate( Saturn::Timestep ts ) override;
	void OnPhysicsUpdate( Saturn::Timestep ts ) override;

#if !defined(SAT_DIST)
public:
	inline void ShowOrHideDbgMenu() { m_ShowGameStateDebug ^= 1; }
#endif

private:
	void TickGameState( Timestep ts );
	void DrawGameStateUI();

	void SetupFirstWave();
	void SetupGameState();

	void AdvanceToNextWave();
	void ReplenishAmmoCrates();

private:
	uint32_t m_CurrentWave = 0u;
	uint32_t m_WaveDifficulty = 1u;

	// Number of wave since last intermission
	uint32_t m_NumberOfWavesSinceLastInt = 0u;

	// The time until the next game state change, in seconds.
	float m_TimeUntilNextGameState = 0.0f;
#if !defined(SAT_DIST)
	bool m_ShowGameStateDebug = false;
#endif
	EGameState m_GameState = EGameState::Init;
};
