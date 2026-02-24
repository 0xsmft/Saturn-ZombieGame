#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Consumable.h"

#include "AmmoCrateSpawner.Gen.h"

using namespace Saturn;

/**
 * Ammo Crate Spawner
 * 
 * OnBeginPlay the spawner will try to spawn.
 * 
 * The GameState controls when it should try to respawn.
 * 
 * NOTE: Although this is called AmmoCrateSpawner, 
 * in the ClassInstance it will have it's mesh, box collider and it's rigidbody,
 * meaning that this class IS the AmmoCrate itself, yes I know it's dumb... but I don't want to overcomplicate the system again.
 */
SCLASS( VisibleInEditor )
class AmmoCrateSpawner : public Consumable
{
	GENERATED_BODY()
public:
	AmmoCrateSpawner();
	~AmmoCrateSpawner();

	virtual void BeginPlay() override;

public:
	//////////////////////////////////////////////////////////////////////////
	// Consumable API

	/**
	 * Spawn the ammo crate now! Fuck the RNG.
	 */
	virtual void ForceSpawn() override;

	/**
	 * Let the RNG decide if we should spawn or not.
	 */
	virtual void RequestRespawn() override;

	/**
	 * See documentation in Consumable
	 */
	virtual void OnUse() override;

protected:
	virtual void TrySpawnAgain() override;
};
