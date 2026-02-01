#include "AmmoCrateSpawner.h"

#include "Saturn/Core/Random.h"

#include "Saturn/Asset/AssetManager.h"

#include "Saturn/Physics/PhysicsRigidBody.h"

AmmoCrateSpawner::AmmoCrateSpawner()
{
	AddComponent<StaticMeshComponent>();
	AddComponent<BoxColliderComponent>();
//	AddComponent<RigidbodyComponent>().IsKinematic = true;
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
	if( m_CheckRandom )
	{
		TrySpawnAgain();
		m_CheckRandom = false;
	}
}

void AmmoCrateSpawner::TrySpawnAgain()
{
	if( Random::RandomBool() )
	{
		Show();
		GetComponent<RigidbodyComponent>().Rigidbody->SetShapeTrigger( false );

		// Number of mags.
		m_Value = Random::RandomElementInRange( 1, 4 );
	}
	else
	{
		Hide();
		GetComponent<RigidbodyComponent>().Rigidbody->SetShapeTrigger( true );
	}
}
