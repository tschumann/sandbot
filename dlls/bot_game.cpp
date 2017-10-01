#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

bool Game::IsMultiplayer()
{
	return gpGlobals->deathmatch > 0.0f;
}

int Game::GetMaxPlayers()
{
	return gpGlobals->maxClients;
}

bool Game::CanAddBots()
{
	return true;
}

bool Game::IsTeamPlay()
{
	return CVAR_GET_FLOAT("mp_teamplay") > 0.0f;
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

// player edicts seem to be reused (they are always the first
// ones in the edict list) and bot edicts don't seem to be
// properly marked as invalid so a few extra checks are needed
// to make sure the given edict doesn't belong to a bot that has
// been kicked
// see http://forums.bots-united.com/printthread.php?t=3517&pp=10
bool Game::IsValidEdict( edict_t *pEdict )
{
	if( pEdict == NULL )
	{
		return false;
	}
	if( pEdict->free )
	{
		return false;
	}
	if( pEdict->v.flags & FL_KILLME )
	{
		return false;
	}
	if( FStrEq(STRING(pEdict->v.netname), "") )
	{
		return false;
	}
	if( pEdict->v.classname == 0 )
	{
		return false;
	}
	if( STRING(pEdict->v.classname)[0] == 0 )
	{
		return false;
	}

	return true;
}