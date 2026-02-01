#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/GameFramework/Character.h"

#include "Player.Gen.h"

using namespace Saturn;

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

private:
	void Use();
	void Reload();
	void Interact();

	void TakeDamage( int32_t damage );

	void OnMeshHit( SharedPtr<Entity> Other );
	void OnMeshExit( SharedPtr<Entity> Other );

	void ShowMessageText( float timeInSeconds = 2.5f );
	void HideMessageText();

private:
	int32_t m_Health = 100;
	uint32_t m_Stamina = 100u;
	uint32_t m_Hunger = 100u;

	// Weapon
	uint32_t m_Ammo = 0u;
	uint32_t m_MaxAmmoInMag = 30u;
	uint32_t m_NumberOfMagazines = 1u;

	SharedPtr<Entity> m_Weapon = nullptr;

	glm::vec3 m_PrevForward{};
	glm::vec3 m_PrevRight{};
	float m_CurrentYaw = 0.0f;
	float m_MessageTimeRemaining = 0.0f;

	bool m_ShowMessageText = false;
	bool m_AlreadyReloading = false;

	std::string m_StatusMessageText{};
};
