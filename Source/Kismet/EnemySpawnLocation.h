#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "EnemySpawnLocation.Gen.h"

using namespace Saturn;

SCLASS( VisibleInEditor )
class EnemySpawnLocation : public Entity
{
	GENERATED_BODY()
public:
	EnemySpawnLocation();
	~EnemySpawnLocation();

	virtual void BeginPlay() override;
	virtual void OnUpdate( Saturn::Timestep ts ) override;

public:
	/**
	 * Spawn more enemies.
	 */
	void Spawn();

	void IncrementNumberToSpawn() { ++m_NumberToSpawn; }
	uint32_t GetNumberToSpawn() { return m_NumberToSpawn; }
	void SetNumberToSpawn( uint32_t n ) { m_NumberToSpawn = n; }

private:
	uint32_t m_NumberToSpawn = 1;

	std::vector<glm::vec3> m_SpawingLocations;
};
