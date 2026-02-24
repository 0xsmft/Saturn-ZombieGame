#include "Consumable.h"

#include "Saturn/Core/Random.h"

Consumable::Consumable()
{
}

Consumable::~Consumable()
{
}

void Consumable::ForceSpawn()
{
	Show();
}

void Consumable::RequestRespawn()
{
	TrySpawnAgain();
}

void Consumable::OnUse()
{
	Hide();
}

void Consumable::TrySpawnAgain()
{
	if( Random::RandomBool() )
	{
		ForceSpawn();
	}
	else
	{
		Hide();
		m_Value = 0u;
	}
}

