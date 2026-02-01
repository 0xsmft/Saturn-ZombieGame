#include "sppch.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void Enemy::OnUpdate( Saturn::Timestep ts )
{
	Super::OnUpdate( ts );
}

void Enemy::TakeDamage( int32_t damage )
{
	m_Health = glm::max( 0, m_Health - damage );

	if( m_Health == 0 )
	{
		GetScene()->DestroyEntity( this );
	}
}
