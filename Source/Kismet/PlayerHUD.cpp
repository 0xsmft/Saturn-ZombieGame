#include "sppch.h"
#include "PlayerHUD.h"

#include "Saturn/Scene/Scene.h"

#include "Saturn/Alura/AluraCanvas.h"
#include "Saturn/Vulkan/AluraRenderer.h"

#include "Saturn/Asset/AssetManager.h"
#include "Saturn/Asset/TextureSourceAsset.h"

PlayerHUD::PlayerHUD()
{
}

PlayerHUD::~PlayerHUD()
{
	OnDestroy();
}

void PlayerHUD::OnInit()
{
	// Fix - API 0.1
	Ref<TextureSourceAsset> sourceAsset = AssetManager::Get()->GetAssetAs<TextureSourceAsset>( 2338072335932728136llu );
	m_HudCrosshairTexture = sourceAsset->GetTexture();
}

void PlayerHUD::OnDraw( Timestep ts )
{
	if( !m_PlayerEntity || g_ActiveScene->IsPaused() )
		return;

	// Alura UI pass.
	g_AluraCanvas->PushFontSize( 32.0f );

	if( m_ShowPlayerDebug )
	{
		g_AluraCanvas->AddText( std::format( "Health: {}", m_PlayerEntity->GetHealth() ) );
		g_AluraCanvas->AddText( std::format( "Speed: {:.2f}", m_PlayerEntity->GetMovementSpeed() ) );

		g_AluraCanvas->PopFontSize();
		return;
	}

	// Player
	{
		g_AluraCanvas->AddText( "Health" );
		g_AluraCanvas->SameLine();
		g_AluraCanvas->AddProgressBar( ( float ) ( m_PlayerEntity->GetHealth() / 100 ), { 64.0f, 24.0f } );
		g_AluraCanvas->SameLine();
		g_AluraCanvas->AddText( std::format( "{0} / 100", m_PlayerEntity->GetHealth() ) );
	}

	// Weapon
	{
		// Fix - API 3.2
		std::string text = std::format( "{0} / {1} ({2} Magazines)", m_PlayerEntity->GetAmmo(), m_PlayerEntity->GetMaxAmmoInMag(), m_PlayerEntity->GetNumberOfMagazines() );

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

void PlayerHUD::OnDestroy()
{
	m_PlayerEntity = nullptr;
}

void PlayerHUD::OnEvent( Event& rEvent )
{
	switch( rEvent.Type )
	{
		case EventType::KeyPressed:
		{
			RubyKeyEvent& rKeyEvent = ( RubyKeyEvent& ) rEvent;

			if( rKeyEvent.GetKeycode() == RubyKey_F2 )
			{
				m_ShowPlayerDebug ^= 1;
			}

		} break;

		default:
			break;
	}
}

void PlayerHUD::ShowMessageText( const std::string& rMessage, float timeInSeconds )
{
	m_StatusMessageText = rMessage;
	m_MessageTimeRemaining = timeInSeconds;
	m_ShowMessageText = true;
}

void PlayerHUD::HideMessageText() 
{
	m_StatusMessageText = "";
	m_MessageTimeRemaining = 0.0f;
	m_ShowMessageText = false;
}
