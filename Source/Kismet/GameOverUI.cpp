#include "sppch.h"
#include "GameOverUI.h"

#include "Saturn/Alura/AluraCanvas.h"
#include "Saturn/Scene/Scene.h"

#include "Saturn/GameFramework/GameFramework.h"
#include "SharedGlobals.h"

GameOverUI::GameOverUI()
{
}

GameOverUI::~GameOverUI()
{
}

void GameOverUI::OnInit()
{
}

void GameOverUI::OnDraw( Timestep ts )
{
	g_AluraCanvas->PushFontSize( 32.0f );
	g_AluraCanvas->AddText( "Game Over" );

	if( g_AluraCanvas->AddButton( "Retry" ) )
	{
		g_ActiveScene->TravelToScene( "PlayingField" );
	}

	if( g_AluraCanvas->AddButton( "Quit Application" ) )
	{
		RcCloseApplication();
	}

	g_AluraCanvas->PopFontSize();
}

void GameOverUI::OnDestroy()
{
}

void GameOverUI::OnEvent( Event& rEvent )
{
}
