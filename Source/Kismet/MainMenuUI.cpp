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
	g_AluraCanvas->PushFontSize( 32.0f );
	g_AluraCanvas->AddText( "Saturn-ZombieGame" );

	if( g_AluraCanvas->AddButton( "Play" ) )
	{
		g_ActiveScene->TravelToScene( "PlayingField" );
	}

	if( g_AluraCanvas->AddButton( "Settings" ) )
	{
	}

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
