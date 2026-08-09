#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Scene/Entity.h"

#include "MainMenuUI.h"

#include "MenuGameState.Gen.h"

using namespace Saturn;

/**
 * MenuGameState
 * 
 * Creates the MainMenuUI
 */
SCLASS( VisibleInEditor )
class MenuGameState : public Entity
{
	GENERATED_BODY()
public:
	MenuGameState();
	virtual ~MenuGameState();

public:
	virtual void BeginPlay() override;

private:
	Ref<MainMenuUI> m_MainMenu;
};
