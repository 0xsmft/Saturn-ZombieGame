#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Alura/AluraDrawer.h"

#include "Player.h"

#include "PlayerHUD.Gen.h"

namespace Saturn {
	class Texture2D;
}

using namespace Saturn;

/**
 * Player Heads up display class
 * 
 * Draws player health and ammo.
 */
SCLASS( VisibleInEditor )
class PlayerHUD : public AluraDrawer
{
	GENERATED_BODY()
public:
	PlayerHUD();
	~PlayerHUD();

	// AluraDrawer
	virtual void OnInit() override;
	virtual void OnDraw( Timestep ts ) override;
	virtual void OnDestroy() override;
	virtual void OnEvent( Event& rEvent ) override;

public:
	/**
	 * Set the player for this HUD.
	 */
	void SetPlayer( SharedPtr<Player> player ) { m_PlayerEntity = player; }

	/**
	 * Display a message text on the left hand side of the screen.
	 * 
	 * @param rMessage - the message to display
	 * @param timeInSeconds - the time that the message should last for.
	 */
	void ShowMessageText( const std::string& rMessage, float timeInSeconds = 2.5f );

	/**
	 * Hide the current message text and clear the stored message.
	 */
	void HideMessageText();

private:
	SharedPtr<Player> m_PlayerEntity;

	std::string m_StatusMessageText{};

	// Time in seconds.
	float m_MessageTimeRemaining = 0.0f;
	bool m_ShowMessageText = false;

	// API 1.1
	Ref<Texture2D> m_HudCrosshairTexture = nullptr;
};
