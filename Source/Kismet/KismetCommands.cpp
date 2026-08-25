#include "sppch.h"

#include "Player.h"
#include "GameState.h"

#include "Saturn/RuntimeConsole/ConsoleCommandManager.h"
#include "Saturn/RuntimeConsole/ConsoleCommand.h"

#include "Saturn/Scene/Scene.h"
#include "SharedGlobals.h"

using namespace Saturn;

static void CmmCmd_Ks( std::string key, std::string value )
{
	if( key == "WC" )
	{
		if( value == "YES" )
		{
			const auto gameStates = g_ActiveScene->GetAllEntitiesWithClass<GameState>();
			for( auto& rGameState : gameStates )
			{
				rGameState->SetCanSpawn( true );
			}
		}
		else if( value == "NO" )
		{
			const auto gameStates = g_ActiveScene->GetAllEntitiesWithClass<GameState>();
			for( auto& rGameState : gameStates )
			{
				rGameState->SetCanSpawn( false );
			}
		}
	}
	else if( key == "AX" ) 
	{
		const auto players = g_ActiveScene->GetAllEntitiesWithClass<Player>();
		for( auto& rPlayer : players )
		{
			rPlayer->SetInvulnerable( true );
		}
	}
}

static const ConsoleCommandArgsVoidRet<decltype( CmmCmd_Ks ), std::string, std::string> ksCommand( "ks", CmmCmd_Ks );
