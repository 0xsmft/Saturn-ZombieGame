#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/Alura/AluraDrawer.h"

#include "GameOverUI.Gen.h"

using namespace Saturn;

SCLASS( VisibleInEditor )
class GameOverUI : public AluraDrawer
{
	GENERATED_BODY()
public:
	GameOverUI();
	virtual ~GameOverUI();

	virtual void OnInit() override;
	virtual void OnDraw( Timestep ts ) override;
	virtual void OnDestroy() override;
	virtual void OnEvent( Event& rEvent ) override;
};
