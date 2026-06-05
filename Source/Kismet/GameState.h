#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "GameState.Gen.h"

using namespace Saturn;

namespace Saturn {
	class Sound;
}

class GameStateHUD;

/**
 * The actual game state...
 */
enum class GameStateStage : uint8_t
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
 * and when the next wave should occur, it also keeps track of ammo boxes and heath kits.
 */
SCLASS( VisibleInEditor )
class GameState : public Entity
{
	GENERATED_BODY()
public:
	GameState();
	virtual ~GameState();

public:
	void BeginPlay() override;
	void OnUpdate( Saturn::Timestep ts ) override;
	void OnPhysicsUpdate( Saturn::Timestep ts ) override;

public:
	uint32_t GetCurrentWave() const { return m_CurrentWave; }
	uint32_t GetWaveDifficulty() const { return m_WaveDifficulty; }
	uint32_t GetNumberOfWavesSinceLastInt() const { return m_NumberOfWavesSinceLastInt; }
	float TimeUntilNextGameStateChange() const { return m_TimeUntilNextGameState; }
	GameStateStage GetCurrentStage() const { return m_GameState; }

private:
	void TickGameState( Timestep ts );

	void SetupFirstWave();
	void SetupGameState();

	void AdvanceToNextWave();
	void ReplenishConsumables();

private:
	uint32_t m_CurrentWave = 0u;
	uint32_t m_WaveDifficulty = 1u;

	// Number of wave since last intermission
	uint32_t m_NumberOfWavesSinceLastInt = 0u;

	// The time until the next game state change, in seconds.
	float m_TimeUntilNextGameState = 0.0f;

	Ref<Sound> m_TickSound;
	Ref<GameStateHUD> m_GameStateHUD;
	
	GameStateStage m_GameState = GameStateStage::Init;
};
