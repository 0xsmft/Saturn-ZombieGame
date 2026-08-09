#include "sppch.h"
#include "MainMenuUI.h"

#include "Saturn/GameFramework/GameFramework.h"
#include "Saturn/Alura/AluraCanvas.h"
#include "Saturn/Scene/Scene.h"
#include "SharedGlobals.h"

MainMenuUI::MainMenuUI()
{
}

MainMenuUI::~MainMenuUI()
{
}

void MainMenuUI::OnInit()
{
}

void MainMenuUI::OnDraw( Timestep ts )
{
//	g_AluraCanvas->AlignNextItemCenterXY( g_AluraCanvas->CalcTextSize( "Saturn-ZombieGame" ) );
	g_AluraCanvas->PushFontSize( 32.0f );
	g_AluraCanvas->AddText( "Saturn-ZombieGame" );

//	g_AluraCanvas->NudgeNextItemPosition( glm::vec2{ 0.0f, 25.0f }, true );

	if( g_AluraCanvas->AddButton( "Play" ) )
	{
		g_ActiveScene->TravelToScene( "PlayingField" );
	}

//	g_AluraCanvas->NudgeNextItemPosition( glm::vec2{ 0.0f, 25.0f }, true );

	if( g_AluraCanvas->AddButton( "Settings" ) )
	{
	}

// 	g_AluraCanvas->NudgeNextItemPosition( glm::vec2{ 0.0f, 25.0f }, true );

	if( g_AluraCanvas->AddButton( "Quit" ) )
	{
		RcCloseApplication();
	}

	g_AluraCanvas->PopFontSize();
}

void MainMenuUI::OnDestroy()
{
}

void MainMenuUI::OnEvent( Event& rEvent )
{
}
