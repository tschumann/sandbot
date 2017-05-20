#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

bool Game::CanAddBots()
{
	return false;
}

bool Game::IsTeamPlay()
{
	return CVAR_GET_FLOAT("mp_teamplay") > 0;
}

bool Game::IsCTF()
{
	return false;
}

unsigned int Game::BotsOnTeam( int team )
{
	int iOnTeam = 0;

	for( int i = 0; i < MAX_PLAYERS; i++ )
	{
		if( pBots[i]->GetTeam() == team )
		{
			iOnTeam++;
		}
	}

	return iOnTeam;
}