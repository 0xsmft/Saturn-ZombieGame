#include "sppch.h"
#include "Projectile.h"

Projectile::Projectile()
{
	AddComponent<SphereColliderComponent>();
	AddComponent<RigidbodyComponent>();
	AddComponent<StaticMeshComponent>();
}

Projectile::~Projectile()
{
}

void Projectile::BeginPlay()
{

}

void Projectile::OnUpdate( Timestep ts )
{

}

