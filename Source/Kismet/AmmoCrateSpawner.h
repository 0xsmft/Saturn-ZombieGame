#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "AmmoCrateSpawner.Gen.h"

using namespace Saturn;

/**
 * Ammo Crate Spawner
 * 
 * OnBeginPlay the spawner will try to spawn.
 * 
 * The GameState controls when it should try to respawn.
 */
SCLASS( VisibleInEditor )
class AmmoCrateSpawner : public Entity
{
	GENERATED_BODY()
public:
	AmmoCrateSpawner();
	~AmmoCrateSpawner();

	virtual void BeginPlay() override;
	virtual void OnUpdate( Saturn::Timestep ts ) override;

public:
	/**
	 * Spawn the ammo crate now! Fuck the RNG.
	 */
	void ForceSpawn();

	/**
	 * Let the RNG decide if we should spawn or not.
	 */
	void RequestRespawn();

	/**
	 * Get the value of the magazines in the crate.
	 */
	uint32_t GetValue() const { return m_Value; }

private:
	void TrySpawnAgain();

private:
	uint32_t m_Value = 0u;
};
