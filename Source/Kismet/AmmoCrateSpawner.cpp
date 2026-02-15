#include "AmmoCrateSpawner.h"

#include "Saturn/Core/Random.h"
#include "Saturn/Physics/PhysicsRigidBody.h"

AmmoCrateSpawner::AmmoCrateSpawner()
{
	AddComponent<StaticMeshComponent>();
	AddComponent<BoxColliderComponent>();

	m_Type = ConsumableType::Ammo;
}

AmmoCrateSpawner::~AmmoCrateSpawner()
{
}

void AmmoCrateSpawner::BeginPlay()
{
	TrySpawnAgain();
}

void AmmoCrateSpawner::ForceSpawn()
{
	Super::ForceSpawn();

	// Set Number of mags.
	m_Value = Random::RandomElementInRange( 1, 4 );
}

void AmmoCrateSpawner::RequestRespawn()
{
	TrySpawnAgain();
}

void AmmoCrateSpawner::TrySpawnAgain()
{
	Super::TrySpawnAgain();

	GetComponent<RigidbodyComponent>().Rigidbody->SetShapeTrigger( !IsVisible() );
}
