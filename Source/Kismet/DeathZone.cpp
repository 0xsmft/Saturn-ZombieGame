#include "DeathZone.h"

#include "Saturn/Scene/Scene.h"

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
}

void DeathZone::OnEntityHit( Entity* pOther, bool isTrigger )
{
	if( pOther )
	{
		// If we just killed the player then we need to spawn the camera again, so that the whole runtime doesn't end.
		if( pOther->StaticClass() == Player::StaticClass() )
		{
			auto camEntity = GetScene()->CreateEntity( "DzCamera" );
			auto& rCameraComp = camEntity->AddComponent<CameraComponent>();
			rCameraComp.MainCamera = true;
		}

		SAT_CORE_INFO( "DeathZone killed entity: {0}", pOther->GetName() );

		// Destory entity...
		GetScene()->DestroyEntity( pOther );
	}
}
