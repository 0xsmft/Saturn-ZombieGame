#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Alura/AluraDrawer.h"

#include "MainMenuUI.Gen.h"

using namespace Saturn;

/**
 * MainMenuUI
 * 
 * Draws the main menu.
 */
SCLASS( VisibleInEditor )
class MainMenuUI : public AluraDrawer
{
	GENERATED_BODY()
public:
	MainMenuUI();
	virtual ~MainMenuUI();

	// AluraDrawer
	virtual void OnInit() override;
	virtual void OnDraw( Timestep ts ) override;
	virtual void OnDestroy() override;
	virtual void OnEvent( Event& rEvent ) override;

};
