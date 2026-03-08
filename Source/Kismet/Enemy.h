#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/AI/AIAgentEntity.h"

#include "Enemy.Gen.h"

using namespace Saturn;

namespace Saturn {
	class Sound;
}

/**
 * The enemy entity class.
 * Able to take damage and deal damage to the player.
 */
SCLASS( VisibleInEditor )
class Enemy : public AIAgentEntity
{
	GENERATED_BODY()
public:
	Enemy();
	virtual ~Enemy();

	virtual void BeginPlay() override;
	virtual void OnUpdate( Saturn::Timestep ts ) override;

public:
	void OnMeshHit( SharedPtr<Entity> Other );
	
	void TakeDamage( int32_t damage );

private:
	int32_t m_Health = 100;

	Ref<Sound> m_AmbientSound = nullptr;
};
