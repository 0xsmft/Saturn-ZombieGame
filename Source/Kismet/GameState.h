#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "GameState.Gen.h"

using namespace Saturn;

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

private:
	void SetupFirstWave();
	void SetupGameState();

	void AdvanceToNextWave();

private:
	uint32_t m_CurrentWave = 0;
	// In seconds
	float m_TimeUntilNextWave = 0.0f;
};
