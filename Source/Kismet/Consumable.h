#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "Consumable.Gen.h"

using namespace Saturn;

/**
 * What type of consumable is this?
 */
enum class ConsumableType
{
	Unknown,
	Ammo,
	HealthKit,
};

/**
 * Base class for all "Consumable" entities, i.e. Ammo and HealthKits.
 * 
 * In this game because all Consumable have a random chance of spawning, this class facilitates such functionality.
 */
SCLASS( VisibleInEditor )
class Consumable : public Entity
{
	GENERATED_BODY()
public:
	Consumable();
	~Consumable();

public:
	/**
	* Spawn the ammo crate now! Fuck the RNG.
	*/
	virtual void ForceSpawn();

	/**
	 * Let the RNG decide if we should spawn or not.
	 */
	virtual void RequestRespawn();

	/**
	 * Get the "value" of this consumable.
	 * 
	 * Value could mean the amount of magazines in it if this was an AmmoCrateSpawner
	 * or in the case of the HealthKit this value would be the amount of health to reward.
	 */
	uint32_t GetValue() const { return m_Value; }

	ConsumableType GetType() const { return m_Type; }

protected:
	virtual void TrySpawnAgain();

protected:
	uint32_t m_Value = 0u;

	// I know we could use Consumable::StaticClass(), however using an enum is one faster and two more readable code
	// as we don't have to do if and if elses.
	ConsumableType m_Type = ConsumableType::Unknown;
};
