#include "sppch.h"
#include "MenuGameState.h"

#include "Saturn/Alura/AluraCanvas.h"
#include "SharedGlobals.h"

MenuGameState::MenuGameState()
{
}

MenuGameState::~MenuGameState()
{
}

void MenuGameState::BeginPlay()
{
	m_MainMenu = Ref<MainMenuUI>::Create();
	g_AluraCanvas->AddDrawer( m_MainMenu );
}
