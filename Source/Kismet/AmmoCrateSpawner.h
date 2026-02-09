#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "AmmoCrateSpawner.Gen.h"

using namespace Saturn;

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
	void ForceSpawn();
	void RequestRespawn();
	uint32_t GetValue() const { return m_Value; }

private:
	void TrySpawnAgain();

private:
	uint32_t m_Value = 0u;
};
