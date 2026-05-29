#include "Player.h"

#include "Consumable.h"

#include "Enemy.h"

#if !defined(SAT_DIST)
#include "GameState.h"
#endif

#include "PlayerHUD.h"

#include "Saturn/Core/Random.h"

#include "Saturn/GameFramework/SClass.h"

#include "Saturn/Physics/PhysicsScene.h"
#include "Saturn/Physics/PhysicsRigidBody.h"

#include "Saturn/Audio/AudioSystem.h"

#include "Saturn/Project/Project.h"

#include "Saturn/Alura/AluraCanvas.h"

#include "Saturn/Animation/AnimationController.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::BeginPlay() 
{
	Super::BeginPlay();
	
	//////////////////////////////////////////////////////////////////////////

	m_PlayerHUD = Ref<PlayerHUD>::Create();
	m_PlayerHUD->SetPlayer( SharedFromThis() );
	g_AluraCanvas->AddDrawer( m_PlayerHUD );

	//////////////////////////////////////////////////////////////////////////

	m_Ammo = m_MaxAmmoInMag;

	m_PrevForward = CalculateForward();
	m_PrevRight = CalculateRight();

	GetComponent<SkeletalMeshComponent>().LocalAnimator->GetAnimGraph()->SetVariable( "Speed", &m_MovementSpeed );
	GetComponent<SkeletalMeshComponent>().LocalAnimator->GetAnimGraph()->SetVariable( "IsReloading", &m_AlreadyReloading );
	GetComponent<SkeletalMeshComponent>().LocalAnimator->GetAnimGraph()->SetVariable( "IsFiring", &m_IsFiring );
}

void Player::OnUpdate( Timestep ts )
{
	// API - 4.0
	if( GetScene()->IsPaused() )
		return;

	Super::OnUpdate( ts );

	// Damage checking.
	if( m_AlreadyTakingDamage && ( m_DamageCooldownTime -= ts.Seconds() ) <= 0.0f )
	{
#if defined(SAT_DEBUG)
		SAT_CORE_INFO( "The Player is able to take damage again..." );
#endif

		m_DamageCooldownTime = 0.0f;
		m_AlreadyTakingDamage = false;
	}

	// Consumable hit detection.
	// Ray cast every frame!
	RaycastHitResult result;
	TransformComponent tc = GetScene()->GetWorldSpaceTransform( GetCameraEntity() );

	if( GetScene()->RaycastIgnore( SharedFromThis(), tc.Position + CalculateForward(), CalculateForward(), 10.0f, &result ) )
	{
		if( result.Hit )
		{
			if( result.Hit->GetClass()->IsChildOf( Consumable::StaticClass() ) )
			{
				auto c = result.Hit.As<Consumable>();
				if( c->IsInteractable() )
				{
					m_IntractableEntityHit = result.Hit;

					// Display "E" to interact
					m_PlayerHUD->ShowMessageText( "E" );
				}
			}
			else
			{
				m_PlayerHUD->HideMessageText();
				m_IntractableEntityHit = nullptr;
			}
		}
	}
	else
	{
		m_PlayerHUD->HideMessageText();
		m_IntractableEntityHit = nullptr;
	}
}

void Player::OnEntityHit( Entity* pOther, bool isTrigger )
{
	if( pOther->GetClass() == Enemy::StaticClass() )
	{
		TakeDamage( 15 );
	}
}

void Player::Use()
{
	if( Input::Get().GetCursorMode() != RubyCursorMode::Locked )
		return;

	if( m_Ammo == 0 ) 
	{
		// Show status message.
		m_PlayerHUD->ShowMessageText( "No Ammo." );

		// Fix - API 0.1
		// Play ammo empty sound.
		AudioSystem::Get().RequestNewSound( 8451897523760267992llu );
		return;
	}

	--m_Ammo;

	// Hitscan weapons.
	RaycastHitResult result;
	TransformComponent tc = GetScene()->GetWorldSpaceTransform( GetCameraEntity() );
	const glm::vec3 startingPosition = tc.Position;

	if( GetScene()->RaycastIgnore( SharedFromThis(), startingPosition + CalculateForward(), CalculateForward(), 100.0f, &result ) )
	{
		if( result.Hit )
		{
			if( result.Hit->GetClass() == Enemy::StaticClass() )
			{
				auto enemy = result.Hit.As<Enemy>();
				if( enemy )
				{
					// Yes we could be fancy and do some crazy cool maths to figure out
					// what our damage should be based on the distance
					// but we are shooting a rife and lets be real most of the entities have to be in close range
					// anyways.
					// 
					// MaxDistance == 100.0 == 100m
					// Anyways, more RNG!
					enemy->TakeDamage( ( int )Random::RandomElementInRange( 25, 88 ) );
				}
			}
		}
	}

	// Fix - API 0.1
	// Play fire sound.
	AudioSystem::Get().PlaySoundAtLocation( 3293489935082472872llu, Saturn::UUID(), GetLocalPosition() );
}

void Player::Reload()
{
	if( m_Ammo == m_MaxAmmoInMag )
	{
		m_PlayerHUD->ShowMessageText( "Already full ammo." );
	}
	else if( m_AlreadyReloading )
	{
		m_PlayerHUD->ShowMessageText( "Already reloading." );
	}
	else if( m_NumberOfMagazines == 0 )
	{
		m_PlayerHUD->ShowMessageText( "No magazines to reload from." );
	}
	else
	{
		m_AlreadyReloading = true;

		// Fix - API 0.1
		Ref<Sound> reloadSound = AudioSystem::Get().RequestNewSound( 570831928178778653llu, Saturn::UUID(), false );
		reloadSound->AddOnCompleteFunction(
			[ this ]( const Saturn::UUID& soundID )
		{
			m_Ammo = m_MaxAmmoInMag;
			--m_NumberOfMagazines;
	
			m_AlreadyReloading = false;
		} );

		reloadSound->Play();
	}
}

void Player::Interact() 
{
	if( Input::Get().GetCursorMode() != RubyCursorMode::Locked )
		return;

	if( m_IntractableEntityHit )
	{
		auto consumable = m_IntractableEntityHit.As<Consumable>();

		// No need to check if its interactable because we wouldn't of gotten here
		// if it wasn't.
		if( consumable /*&& consumable->IsInteractable()*/ )
		{
			switch( consumable->GetType() )
			{
				case ConsumableType::Unknown:
				default:
					break;

				case ConsumableType::Ammo:
				{
					m_NumberOfMagazines += consumable->GetValue();
					consumable->OnUse();

					// Fix - API 0.1 - Play get ammo sound
					AudioSystem::Get().RequestNewSound( 16177217556467335637llu );
				} break;

				case ConsumableType::HealthKit:
				{
					if( m_Health == 100u )
					{
						m_PlayerHUD->ShowMessageText( "Already full health." );
						break;
					}

					m_Health = glm::min( 100u, m_Health + consumable->GetValue() );
					consumable->OnUse();
				} break;
			}
		}
	}
}

void Player::TakeDamage( int32_t damage )
{
	// This is not the best way to handle damage at all, but some entities will swarm the player
	// causing player to die in seconds,
	// so to mitigate this there is a small delay on taking damage.
	// 
	// TODO: Accrue damage.
	if( !m_AlreadyTakingDamage )
	{
#if defined(SAT_DEBUG)
		SAT_CORE_INFO( "Player taking damage {0}", damage );
#endif

		// Reset cool down.
		m_DamageCooldownTime = 0.5f;
		m_Health = glm::max( 0, m_Health - damage );
		m_AlreadyTakingDamage = true;
	}
}

/*
void Player::Jump() 
{
}
*/

void Player::SetupInputBindings()
{
	m_PlayerInputController->BindAction( "Forward", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Character::MoveForward ) );
	m_PlayerInputController->BindAction( "Forward", ActionBindingTriggerState::Released, SAT_BIND_EVENT_FN( Character::MoveForwardEnd ) );

	m_PlayerInputController->BindAction( "Backward", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Character::MoveBack ) );
	m_PlayerInputController->BindAction( "Backward", ActionBindingTriggerState::Released, SAT_BIND_EVENT_FN( Character::MoveForwardEnd ) );

	m_PlayerInputController->BindAction( "Left", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Character::MoveLeft ) );
	m_PlayerInputController->BindAction( "Left", ActionBindingTriggerState::Released, SAT_BIND_EVENT_FN( Character::MoveLeftEnd ) );

	m_PlayerInputController->BindAction( "Right", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Character::MoveRight ) );
	m_PlayerInputController->BindAction( "Right", ActionBindingTriggerState::Released, SAT_BIND_EVENT_FN( Character::MoveLeftEnd ) );

	m_PlayerInputController->BindAction( "Sprint", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Character::StartSprint ) );
	m_PlayerInputController->BindAction( "Sprint", ActionBindingTriggerState::Released, SAT_BIND_EVENT_FN( Character::EndSprint ) );

//	m_PlayerInputController->BindAction( "Jump", ActionBindingTriggerState::Released, SAT_BIND_EVENT_FN( Player::Jump ) );

	m_PlayerInputController->BindAction( "Use", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Player::Use ) );
	m_PlayerInputController->BindAction( "Reload", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Player::Reload ) );
	m_PlayerInputController->BindAction( "Interact", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Player::Interact ) );

}
