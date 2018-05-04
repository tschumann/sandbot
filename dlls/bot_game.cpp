#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

Game::~Game()
{
}

int Game::GetMaxPlayers()
{
	return gpGlobals->maxClients;
}

bool Game::CanAddBots()
{
	return gpGlobals->deathmatch > 0.0f;
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

// TODO: in Opposing Force CTF at least, bots will also target
// spectators - check iuser2 or something to see if spectator?
bool Game::IsValidEnemy( edict_t *pEdict )
{
	return IsValidEntity( pEdict );
}

bool Game::CanChoosePlayerModel()
{
	return true;
}