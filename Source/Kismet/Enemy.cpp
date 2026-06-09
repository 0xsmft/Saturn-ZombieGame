#include "sppch.h"
#include "Enemy.h"

#include "Saturn/Scene/Scene.h"

#include "Player.h"

#include "Saturn/Core/Random.h"
#include "Saturn/Audio/AudioSystem.h"
#include "Saturn/Physics/PhysicsRigidBody.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	// Rare case, but if this enemy is being destroyed without being killed
	// e.g. scene cleanup we must also unload the sound.
	if( m_AmbientSound )
	{
		AudioSystem::Get().UnloadSound( m_AmbientSound );
	}
}

void Enemy::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Shit code until graph sounds have spatialisation.
	std::array< Saturn::UUID, 3 > soundIDs;
	soundIDs[ 0 ] = 2482004967821507364llu; // SFX_ZbGrunt1
	soundIDs[ 1 ] = 2990208416715314967llu; // SFX_ZbGrunt2
	soundIDs[ 2 ] = 11474219673214559782llu; // SFX_ZbGrunt3

	// Fix - API 0.1
	m_AmbientSound = AudioSystem::Get().PlaySoundAtLocation( soundIDs[ Random::RandomElementInRange( 0, 2 ) ], Saturn::UUID(), GetLocalPosition(), false );

	// Work around to avoid enemies having the same sound play at the exact same moment.
	int pcmFrameOffset = ( int ) Random::RandomElementInRange( 0, 10 );
	m_AmbientSound->Play( pcmFrameOffset );
	m_AmbientSound->Loop( true );
}

void Enemy::OnUpdate( Saturn::Timestep ts )
{
	// API - 4.0
	if( GetScene()->IsPaused() )
		return;

	Super::OnUpdate( ts );

	// We destroy the sound, then tick for one frame, then the entity is destroyed, which makes sense
	// due to our differed destruction queue.
	if( m_AmbientSound )
		m_AmbientSound->SetPosition( GetLocalPosition() );
}

void Enemy::OnMeshHit( SharedPtr<Entity> Other )
{
}

void Enemy::TakeDamage( int32_t damage )
{
	m_Health = glm::max( 0, m_Health - damage );

	if( m_Health == 0 )
	{
		// Fix - API 
		AudioSystem::Get().UnloadSound( m_AmbientSound );
		m_AmbientSound = nullptr;

		GetScene()->DestroyEntity( this );
	}
}
