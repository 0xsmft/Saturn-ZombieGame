#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Consumable.h"

#include "HealthKit.Gen.h"

using namespace Saturn;

/**
 * Rewards health once consumed, it's a first aid kit.
 */
SCLASS( VisibleInEditor )
class HealthKit : public Consumable
{
	GENERATED_BODY()
public:
	HealthKit();
	~HealthKit();

	virtual void BeginPlay() override;

public:
	//////////////////////////////////////////////////////////////////////////
	// Consumable API
	virtual void ForceSpawn() override;
	virtual void RequestRespawn() override;

public:
	/**
	* A health kit considered "interactable" if it's visible in the Scene.
	*/
	[[nodiscard]] bool IsInteractable() const { return IsVisible(); }

protected:
	virtual void TrySpawnAgain() override;
};
