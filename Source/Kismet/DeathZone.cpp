#include "DeathZone.h"

#include "Saturn/Scene/Scene.h"

#include "Player.h"
#include "GameOverUI.h"

#include "Saturn/Physics/PhysicsRigidBody.h"
#include "Saturn/Alura/AluraCanvas.h"

#include "SharedGlobals.h"

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
		if( pOther->GetClass() == Player::StaticClass() )
		{
			auto camEntity = GetScene()->CreateEntity( "DzCamera" );
			auto& rCameraComp = camEntity->AddComponent<CameraComponent>();
			rCameraComp.MainCamera = true;

			g_AluraCanvas->RemoveAllDrawers();

			Ref<GameOverUI> gameOverUI = Ref<GameOverUI>::Create();
			g_AluraCanvas->AddDrawer( gameOverUI );
		}

		SAT_CORE_INFO( "DeathZone killed entity: {0}", pOther->GetName() );

		// Destroy entity...
		GetScene()->DestroyEntity( pOther );
	}
}
