#include "Player.h"

#include "Consumable.h"

#include "Enemy.h"

#if !defined(SAT_DIST)
#include "GameState.h"
#endif

#include "Saturn/Core/Random.h"

#include "Saturn/GameFramework/SClass.h"

#include "Saturn/Physics/PhysicsScene.h"
#include "Saturn/Physics/PhysicsRigidBody.h"

#include "Saturn/Audio/AudioSystem.h"
#include "Saturn/Asset/AssetManager.h"
#include "Saturn/Asset/TextureSourceAsset.h"

#include "Saturn/Project/Project.h"

#include "Saturn/Alura/AluraCanvas.h"
#include "Saturn/Vulkan/AluraRenderer.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::BeginPlay() 
{
	Super::BeginPlay();

	// Fix - API 0.1
	Ref<TextureSourceAsset> sourceAsset = AssetManager::Get()->GetAssetAs<TextureSourceAsset>( 2338072335932728136llu );
	m_HudCrosshairTexture = sourceAsset->GetTexture();
	
	//////////////////////////////////////////////////////////////////////////

	if( GetRigidBody() )
	{
		GetRigidBody()->SetOnCollisionHit( SAT_BIND_EVENT_FN( OnMeshHit ) );
		GetRigidBody()->SetOnCollisionExit( SAT_BIND_EVENT_FN( OnMeshExit ) );
	}

	for( const auto& rID : GetChildren() )
	{
		SharedPtr<Entity> child = GetScene()->FindEntityByID( rID );
		if( child && child->GetName() == "Weapon" )
		{
			m_Weapon = child;
		}
	}

	// Fix - API 2.0
	m_Weapon->SetParent( GetCameraEntity()->GetUUID() );
	GetCameraEntity()->AddChild( m_Weapon->GetUUID() );

	GetChildren().erase( std::remove( GetChildren().begin(), GetChildren().end(), m_Weapon->GetUUID() ), GetChildren().end() );

	m_Ammo = m_MaxAmmoInMag;

	m_PrevForward = CalculateForward();
	m_PrevRight = CalculateRight();
}

void Player::OnUpdate( Timestep ts )
{
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

	// Update weapon movement.
	if( m_Weapon )
	{
		// This is all we are getting for now.
		// I have NO clue how to make this work.
		// we probably have to calculate our angle offset
		// from our turning... angle bullshit.
		
		const auto& camera = GetCameraEntity()->GetComponent<CameraComponent>().Camera;		
		auto rotation = m_Weapon->GetLocalRotation();
		rotation.x = camera->GetPitch();

		m_Weapon->SetRotation( rotation );
	}

	// Consumable hit detection.
	// Ray cast every frame!
	RaycastHitResult result;
	TransformComponent tc = GetScene()->GetWorldSpaceTransform( GetCameraEntity() );

	if( GetScene()->Raycast( tc.Position + CalculateForward(), CalculateForward(), 10.0f, &result ) )
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
					m_StatusMessageText = "E";
					ShowMessageText();
				}
			}
			else
			{
				HideMessageText();
				m_IntractableEntityHit = nullptr;
			}
		}
	}
	else
	{
		HideMessageText();
		m_IntractableEntityHit = nullptr;
	}

	// ALURA Hud
	DrawHud( ts );
}

void Player::DrawHud( Timestep ts )
{
	// Fix - API 3.0

	// Alura UI pass.
	g_AluraCanvas->PushFontSize( 32.0f );

	// Player
	{
		g_AluraCanvas->AddText( "Health" );
		g_AluraCanvas->SameLine();
		g_AluraCanvas->AddProgressBar( ( float ) ( m_Health / 100 ), { 64.0f, 16.0f } );
		g_AluraCanvas->SameLine();
		g_AluraCanvas->AddText( std::format( "{0} / 100", m_Health ) );
	}

	// Weapon
	{
		// Fix - API 3.2
		std::string text = std::format( "{0} / {1} ({2} Magazines)", m_Ammo, m_MaxAmmoInMag, m_NumberOfMagazines );

		// Fix - API 3.1
		const auto textSize = g_AluraCanvas->CalcTextSize( text );

		g_AluraCanvas->SetNextItemPosition( glm::vec2{ g_AluraCanvas->GetStyle().ItemSpacing.x * 0.25f, ( g_AluraCanvas->GetHeight() - textSize.y ) - g_AluraCanvas->GetStyle().ItemSpacing.y - 2.0f } );
		g_AluraCanvas->AddText( text );
	}

	g_AluraCanvas->PopFontSize();

	if( m_ShowMessageText )
	{
		m_MessageTimeRemaining -= ts.Seconds();

		if( m_MessageTimeRemaining <= 0 )
		{
			m_MessageTimeRemaining = 0.0f;
			m_ShowMessageText = false;
			return;
		}

		g_AluraCanvas->PushFontSize( 28.0f );
		{
			// Fix - API 3.1
			const auto textSize = g_AluraCanvas->CalcTextSize( m_StatusMessageText );
			g_AluraCanvas->SetNextItemPosition( glm::vec2{ g_AluraCanvas->GetStyle().ItemSpacing.x, ( ( g_AluraCanvas->GetHeight() * 0.5f ) - textSize.y ) - g_AluraCanvas->GetStyle().ItemSpacing.y - 2.0f } );
			g_AluraCanvas->AddText( m_StatusMessageText );
		}
		g_AluraCanvas->PopFontSize();
	}

	// Crosshair image.
	g_AluraCanvas->SetNextItemPosition( glm::vec2{ g_AluraCanvas->GetWidth() * 0.5f,  g_AluraCanvas->GetHeight() * 0.5f } );
	g_AluraCanvas->AddImage( { 24.0F, 24.0F }, m_HudCrosshairTexture );
}

void Player::Use()
{
	if( Input::Get().GetCursorMode() != RubyCursorMode::Locked )
		return;

	if( m_Ammo == 0 ) 
	{
		// Show status message.
		m_StatusMessageText = "No Ammo.";
		ShowMessageText();

		// Fix - API 0.1
		// Play ammo empty sound.
		AudioSystem::Get().RequestNewSound( 8451897523760267992llu );
		return;
	}

	--m_Ammo;

	// Hitscan weapons.
	RaycastHitResult result;
	TransformComponent tc = GetScene()->GetWorldSpaceTransform( GetCameraEntity() );

	if( GetScene()->Raycast( tc.Position + CalculateForward(), CalculateForward(), 100.0f, &result ) )
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
		m_StatusMessageText = "Already full ammo.";
		ShowMessageText();
	}
	else if( m_AlreadyReloading )
	{
		m_StatusMessageText = "Already reloading.";
		ShowMessageText();
	}
	else if( m_NumberOfMagazines == 0 )
	{
		m_StatusMessageText = "No Magazines to reload from.";
		ShowMessageText();
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
						m_StatusMessageText = "Already full health";
						ShowMessageText();
						break;
					}

					m_Health = glm::min( 100u, m_Health + consumable->GetValue() );
					consumable->OnUse();
				} break;
			}
		}
	}
}

#if !defined(SAT_DIST)
void Player::DbgMenuHandle()
{
	auto gameStates = GetScene()->GetAllEntitiesWithClass<GameState>();
	for( auto& rGameState : gameStates )
	{
		rGameState->ShowOrHideDbgMenu();
	}
}
#endif

void Player::OnMeshHit( SharedPtr<Entity> Other )
{
	if( Other->GetClass() == Enemy::StaticClass() )
	{
		TakeDamage( 15 );
	}
}

void Player::HandleMenu()
{
#if !defined(SAT_DIST)
	if( Input::Get().KeyPressed( RubyKey_LeftShift ) || Input::Get().KeyPressed( RubyKey_RightShift ) )
#endif
	{
		GetScene()->PauseGame();
		Input::Get().SetCursorMode( RubyCursorMode::Normal, true );
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

void Player::OnMeshExit( SharedPtr<Entity> Other )
{
}

void Player::ShowMessageText( float timeInSeconds /*= 2.5f */ )
{
	m_MessageTimeRemaining = timeInSeconds;
	m_ShowMessageText = true;
}

void Player::HideMessageText()
{
	m_MessageTimeRemaining = 0.0f;
	m_ShowMessageText = false;
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

//	m_PlayerInputController->BindAction( "Jump", ActionBindingTriggerState::Released, SAT_BIND_EVENT_FN( Player::Jump ) );

	m_PlayerInputController->BindAction( "Use", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Player::Use ) );
	m_PlayerInputController->BindAction( "Reload", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Player::Reload ) );
	m_PlayerInputController->BindAction( "Interact", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Player::Interact ) );

#if !defined(SAT_DIST)
	m_PlayerInputController->BindAction( "DbgMenu", ActionBindingTriggerState::Pressed, SAT_BIND_EVENT_FN( Player::DbgMenuHandle ) );
#endif
}
