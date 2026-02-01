#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "Projectile.Gen.h"

using namespace Saturn;

SCLASS( VisibleInEditor )
class Projectile : public Entity
{
	GENERATED_BODY()
public:
	Projectile();
	~Projectile();

	virtual void BeginPlay() override;
	virtual void OnUpdate( Timestep ts ) override;

private:

};
