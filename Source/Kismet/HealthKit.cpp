#include "HealthKit.h"

#include "Saturn/Core/Random.h"
#include "Saturn/Physics/PhysicsRigidBody.h"

HealthKit::HealthKit()
{
	AddComponent<StaticMeshComponent>();
	AddComponent<BoxColliderComponent>();

	m_Type = ConsumableType::HealthKit;
}

HealthKit::~HealthKit()
{
}

void HealthKit::BeginPlay()
{
	// Health Kits do not spawn until requested by the GameState.
//	TrySpawnAgain();
}

void HealthKit::ForceSpawn()
{
	Super::ForceSpawn();

	// With the amount of RNG in this game, there should be RNG if to check if the game should 
	// open or not.
	m_Value = Random::RandomElementInRange( 25, 100 );
}

void HealthKit::RequestRespawn()
{
	Super::RequestRespawn();
}

void HealthKit::TrySpawnAgain()
{
	Super::TrySpawnAgain();

	GetComponent<RigidbodyComponent>().Rigidbody->SetShapeTrigger( !IsVisible() );
}

