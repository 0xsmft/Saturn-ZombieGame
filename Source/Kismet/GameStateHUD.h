#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Alura/AluraDrawer.h"

#include "GameState.h"

#include "GameStateHUD.Gen.h"

using namespace Saturn;

/**
 * GameState UI
 * 
 * Draws the current game state and time until next the next game state change.
 */
SCLASS( VisibleInEditor )
class GameStateHUD : public AluraDrawer
{
	GENERATED_BODY()
public:
	GameStateHUD();
	~GameStateHUD();

	// AluraDrawer
	virtual void OnInit() override;
	virtual void OnDraw( Timestep ts ) override;
	virtual void OnDestroy() override;
	virtual void OnEvent( Event& rEvent ) override;

#if !defined(SAT_DIST)
public:
	inline void ShowOrHideDbgMenu() { m_ShowGameStateDebug ^= 1; }
#endif

private:
	SharedPtr<GameState> m_GameState = nullptr;

#if !defined(SAT_DIST)
	bool m_ShowGameStateDebug = false;
#endif
};
