#include "sppch.h"
#include "GameStateHUD.h"

#include "Saturn/Scene/Scene.h"

#include "Saturn/Alura/AluraCanvas.h"
#include "Saturn/Vulkan/AluraRenderer.h"
#include "Saturn/Core/App.h"

#include "Saturn/GameFramework/GameFramework.h"

GameStateHUD::GameStateHUD()
{
}

GameStateHUD::~GameStateHUD()
{
}

void GameStateHUD::OnInit()
{
	// I know this implies that there are multiple game states, there isn't, there is only ever one.
	auto gameStates = g_ActiveScene->GetAllEntitiesWithClass<GameState>();
	for( auto& rEntity : gameStates )
	{
		m_GameState = rEntity.As<GameState>();
	}
}

void GameStateHUD::OnDraw( Timestep ts )
{
	// Paused check.
	if( g_ActiveScene->IsPaused() )
	{
		g_AluraCanvas->PushFontSize( 32.0f );

		g_AluraCanvas->AddText( "Paused" );
		g_AluraCanvas->AddText( "Press ESC to unpause" );

		if( g_AluraCanvas->AddButton( "Quit Application" ) )
		{
			RcCloseApplication();
		}

		g_AluraCanvas->PopFontSize();

		return;
	}

	// Game state debug.
	if( m_ShowGameStateDebug )
	{
		g_AluraCanvas->PushFontSize( 32.0f );
		{
			g_AluraCanvas->AddText( "=== Game State Debug ===" );
			g_AluraCanvas->TextFormatted( "FPS: {0}", Application::Get()->Framerate() );

			g_AluraCanvas->Indent();
			{
				g_AluraCanvas->TextFormatted( "Current Wave: {0}", m_GameState->GetCurrentWave() );
				g_AluraCanvas->TextFormatted( "Wave Difficulty: {0}", m_GameState->GetWaveDifficulty() );
				g_AluraCanvas->TextFormatted( "Number Of Waves Since Last Intermission: {0}", m_GameState->GetNumberOfWavesSinceLastInt() );
				g_AluraCanvas->TextFormatted( "Time remaining: {0}s", m_GameState->TimeUntilNextGameStateChange() );
				g_AluraCanvas->TextFormatted( "Game state: {0}", ( int ) m_GameState->GetCurrentStage() );
			}
			g_AluraCanvas->Unindent();
		}
		g_AluraCanvas->PopFontSize();

		return;
	}

	// Alura UI pass.
	g_AluraCanvas->PushFontSize( 32.0f );

	// Timer text
	{
		std::string text = std::format( "{0}s", std::ceil( m_GameState->TimeUntilNextGameStateChange() ) );
		// Fix - API 3.1
		const auto textSize = g_AluraCanvas->CalcTextSize( text );

//		g_AluraCanvas->SetNextItemPosition( glm::vec2{ ( g_AluraCanvas->GetWidth() - textSize.x ) - g_AluraCanvas->GetStyle().ItemSpacing.x, g_AluraCanvas->GetStyle().ItemSpacing.y } );
		g_AluraCanvas->AddText( text );
	}

	// Game state text
	{
		std::string text{};

		switch( m_GameState->GetCurrentStage() )
		{
			case GameStateStage::WaveInProgress:
				text = std::format( "Wave {0}", m_GameState->GetCurrentWave() );
				break;

			case GameStateStage::Intermission:
				text = "Intermission - prepare for next wave";
				break;

			case GameStateStage::Init:
			case GameStateStage::Dead:
			default:
				break;
		}

		// Fix - API 3.1
		const auto textSize = g_AluraCanvas->CalcTextSize( text );
		const auto pos = glm::vec2{ ( g_AluraCanvas->GetWidth() - textSize.x ) - g_AluraCanvas->GetStyle().ItemSpacing.x, g_AluraCanvas->GetCursorPosition().y };

		g_AluraCanvas->SetNextItemPosition( pos );
		g_AluraCanvas->AddText( text );
	}

	g_AluraCanvas->PopFontSize();
}

void GameStateHUD::OnDestroy()
{
	m_GameState = nullptr;
}

void GameStateHUD::OnEvent( Event& rEvent )
{
	switch( rEvent.Type )
	{
		case EventType::KeyPressed:
		{
			RubyKeyEvent& rKeyEvent = ( RubyKeyEvent& )rEvent;
			
#if !defined(SAT_DIST)
			if( rKeyEvent.GetKeycode() == RubyKey_Esc && rKeyEvent.GetModifers() == RubyKey_LeftShift )
#else
			if( rKeyEvent.GetKeycode() == RubyKey_Esc )
#endif
			{
				if( g_ActiveScene->IsPaused() )
				{
					g_ActiveScene->UnpauseGame();
					Input::Get().SetCursorMode( RubyCursorMode::Locked, true );
				}
				else
				{
					g_ActiveScene->PauseGame();
					Input::Get().SetCursorMode( RubyCursorMode::Normal, true );
				}
			}

			if( rKeyEvent.GetKeycode() == RubyKey_F1 )
			{
				m_ShowGameStateDebug ^= 1;
			}

		} break;

		default:
			break;
	}
}
