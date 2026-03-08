#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/GameFramework/Character.h"

#include "Player.Gen.h"

using namespace Saturn;

class PlayerHUD;

/**
 * THE player.
 */
SCLASS( VisibleInEditor )
class Player : public Character
{
	GENERATED_BODY()
public:
	Player();
	virtual ~Player();

	virtual void BeginPlay() override;
	virtual void OnUpdate( Saturn::Timestep ts ) override;

	virtual void SetupInputBindings() override;

public:
	int32_t GetHealth() const { return m_Health; }
	uint32_t GetAmmo() const { return m_Ammo; }
	uint32_t GetMaxAmmoInMag() const { return m_MaxAmmoInMag; }
	uint32_t GetNumberOfMagazines() const { return m_NumberOfMagazines; }

private:
	void Use();
	void Reload();
	void Interact();
#if !defined(SAT_DIST)
	void DbgMenuHandle();
#endif

	void TakeDamage( int32_t damage );

	void OnMeshHit( SharedPtr<Entity> Other );
	void OnMeshExit( SharedPtr<Entity> Other );

private:
	int32_t m_Health = 100;
	uint32_t m_Stamina = 100u;
	uint32_t m_Hunger = 100u;

	// Weapon
	uint32_t m_Ammo = 0u;
	uint32_t m_MaxAmmoInMag = 30u;
	uint32_t m_NumberOfMagazines = 1u;

	SharedPtr<Entity> m_Weapon = nullptr;

	// The current intractable entity that the player is looking at.
	SharedPtr<Entity> m_IntractableEntityHit = nullptr;

	Ref<PlayerHUD> m_PlayerHUD;
	
	glm::vec3 m_PrevForward{};
	glm::vec3 m_PrevRight{};

	float m_CurrentYaw = 0.0f;
	
	// Time in seconds.
	float m_DamageCooldownTime = 0.50f;

	bool m_AlreadyReloading = false;
	bool m_AlreadyTakingDamage = false;
};
