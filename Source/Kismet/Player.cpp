#include "Player.h"

#include "AmmoCrateSpawner.h"
#include "Enemy.h"

#include "Saturn/Core/Random.h"

#include "Saturn/Physics/PhysicsScene.h"
#include "Saturn/Physics/PhysicsRigidBody.h"

#include "Saturn/Audio/AudioSystem.h"
#include "Saturn/Asset/AssetManager.h"

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

	if( m_Weapon )
	{
		const auto& camera = GetCameraEntity()->GetComponent<CameraComponent>().Camera;
		
		auto rotation = m_Weapon->GetLocalRotation();
		rotation.x = camera->GetPitch();
//		rotation.y = -camera->GetYaw();

		m_Weapon->SetRotation( rotation );

//		auto camPos = camera->GetPosition();
	}

	// Alura UI pass.
	g_AluraCanvas->PushFontSize( 32.0f );

	// Timer text
	{
		std::string text = std::format( "{0} / {1} ({2} Bullets)", m_Ammo, m_MaxAmmoInMag, m_MaxAmmoInMag * m_NumberOfMagazines );
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
			const auto textSize = g_AluraCanvas->CalcTextSize( m_StatusMessageText );
			g_AluraCanvas->SetNextItemPosition( glm::vec2{ g_AluraCanvas->GetStyle().ItemSpacing.x, ( ( g_AluraCanvas->GetHeight() * 0.5f ) - textSize.y ) - g_AluraCanvas->GetStyle().ItemSpacing.y - 2.0f } );
			g_AluraCanvas->AddText( m_StatusMessageText );
		}
		g_AluraCanvas->PopFontSize();
	}
}

void Player::Use()
{
	if( m_Ammo == 0 ) 
	{
		// Play ammo empty sound
		m_StatusMessageText = "No Ammo.";
		ShowMessageText();

		AudioSystem::Get().RequestNewSound( 8451897523760267992llu );
		return;
	}

	--m_Ammo;

	// Hitscan weapons.
	RaycastHitResult result;
	TransformComponent tc = GetScene()->GetWorldSpaceTransform( GetCameraEntity() );

	if( GetScene()->Raycast( tc.Position + CalculateForward(), CalculateForward(), 10.0f, &result ) )
	{
		if( result.Hit )
		{
			if( result.Hit->GetClass() == Enemy::StaticClass() )
			{
				auto enemy = result.Hit.As<Enemy>();
				if( enemy )
				{
					enemy->TakeDamage( ( int )Random::RandomElementInRange( 25, 88 ) );
				}
			}
		}
	}

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
	RaycastHitResult result;
	TransformComponent tc = GetScene()->GetWorldSpaceTransform( GetCameraEntity() );

	if( GetScene()->Raycast( tc.Position + CalculateForward(), CalculateForward(), 10.0f, &result ) )
	{
		if( result.Hit )
		{
			if( result.Hit->GetClass() == AmmoCrateSpawner::StaticClass() )
			{
				auto ammoCrate = result.Hit.As<AmmoCrateSpawner>();
				if( ammoCrate )
				{
					m_NumberOfMagazines += ammoCrate->GetValue();
					ammoCrate->Hide();

					AudioSystem::Get().RequestNewSound( 16177217556467335637llu );
				}
			}
		}
	}
}

void Player::OnMeshHit( SharedPtr<Entity> Other )
{
	if( Other->GetClass() == Enemy::StaticClass() )
	{
		TakeDamage( 50 );
	}
}

void Player::TakeDamage( int32_t damage )
{
	m_Health = glm::max( 0, m_Health - damage );

	if( m_Health == 0 )
	{
//		GetScene()->DestroyEntity( this );

		SAT_CORE_INFO( "Health: {0}", m_Health );
	}
}

void Player::OnMeshExit( SharedPtr<Entity> Other )
{
	if( Other->GetClass() == Enemy::StaticClass() )
	{
		TakeDamage( 50 );
	}
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
}
