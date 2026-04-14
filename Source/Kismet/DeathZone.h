#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "DeathZone.Gen.h"

using namespace Saturn;

/**
 * The death zone is a huge box collier that will kill ANY entities that enters its box collider
 * This avoids soft locks if the player or any enemies somehow managed to escape the map.
 */
SCLASS( VisibleInEditor )
class DeathZone : public Entity
{
	GENERATED_BODY()
public:
	DeathZone();
	~DeathZone();

	virtual void BeginPlay() override;
	virtual void OnEntityHit( Entity* pOther, bool isTrigger );

};
