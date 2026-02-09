#include "AmmoCrateSpawner.h"

#include "Saturn/Core/Random.h"

#include "Saturn/Asset/AssetManager.h"

#include "Saturn/Physics/PhysicsRigidBody.h"

AmmoCrateSpawner::AmmoCrateSpawner()
{
	AddComponent<StaticMeshComponent>();
	AddComponent<BoxColliderComponent>();
}

AmmoCrateSpawner::~AmmoCrateSpawner()
{
}

void AmmoCrateSpawner::BeginPlay()
{
	TrySpawnAgain();
}

void AmmoCrateSpawner::OnUpdate( Saturn::Timestep ts )
{
}

void AmmoCrateSpawner::ForceSpawn()
{
	Show();
	GetComponent<RigidbodyComponent>().Rigidbody->SetShapeTrigger( false );

	// Number of mags.
	m_Value = Random::RandomElementInRange( 1, 4 );
}

void AmmoCrateSpawner::RequestRespawn()
{
	TrySpawnAgain();
}

void AmmoCrateSpawner::TrySpawnAgain()
{
	if( Random::RandomBool() )
	{
		ForceSpawn();
	}
	else
	{
		Hide();
		GetComponent<RigidbodyComponent>().Rigidbody->SetShapeTrigger( true );
		m_Value = 0u;
	}
}
