#include "DeathZone.h"

#include "Player.h"

#include "Saturn/Physics/PhysicsRigidBody.h"

DeathZone::DeathZone()
{
	AddComponent<RigidbodyComponent>();
	AddComponent<BoxColliderComponent>();
}

DeathZone::~DeathZone()
{
}

void DeathZone::BeginPlay()
{
	if( auto* pRigidbody = GetComponent<RigidbodyComponent>().Rigidbody; pRigidbody )
	{
		pRigidbody->SetOnCollisionHit( SAT_BIND_EVENT_FN( OnMeshHit ) );
	}
}

void DeathZone::OnMeshHit( SharedPtr<Entity> Other )
{
	if( Other )
	{
		// If we just killed the player then we need to spawn the camera again, so that the whole runtime doesn't end.
		if( Other->StaticClass() == Player::StaticClass() )
		{
			auto camEntity = GetScene()->CreateEntity( "DzCamera" );
			auto& rCameraComp = camEntity->AddComponent<CameraComponent>();
			rCameraComp.MainCamera = true;
		}

		SAT_CORE_INFO( "DeathZone killed entity: {0}", Other->GetName() );

		// Destory entity...
		GetScene()->DestroyEntity( Other.Get() );
	}
}
