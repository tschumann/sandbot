//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: game rules code
//
//=============================================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "game.h"
#include "wpt.h"
#include "waypoint.h"

Game::Game( GameId gameId ) : gameId( gameId )
{
}

Game::~Game()
{
}

void Game::Cleanup()
{
}

int Game::GetMaxPlayers() const
{
	return gpGlobals->maxClients;
}

bool Game::CanAddBots() const
{
	return gpGlobals->deathmatch > 0.0f;
}

bool Game::IsTeamPlay() const
{
	return CVAR_GET_FLOAT("mp_teamplay") > 0.0f;
}

bool Game::IsDeathmatch() const
{
	return true;
}

bool Game::IsCTF() const
{
	return false;
}

bool Game::IsCapturePoint() const
{
	return false;
}

unsigned int Game::BotsOnTeam( const int team ) const
{
	int iOnTeam = 0;

	for( int i = 0; i < Game::MAX_PLAYERS; i++ )
	{
		if( pGame->GetTeam( pBots[i]->pEdict ) == team )
		{
			iOnTeam++;
		}
	}

	return iOnTeam;
}

// TODO: in Opposing Force CTF at least, bots will also target
// spectators - check iuser2 or something to see if spectator?
bool Game::IsValidEdict( const edict_t *pEdict ) const
{
	return IsValidEntity( pEdict );
}

bool Game::CanChoosePlayerModel() const
{
	return true;
}

int Game::GetTeam( const edict_t *pEdict ) const
{
	extern char team_names[MAX_TEAMS][MAX_TEAMNAME_LENGTH];
	extern int num_teams;

	char *infobuffer;
	char model_name[32];

	if (team_names[0][0] == 0)
	{
		char *pName;
		char teamlist[MAX_TEAMS*MAX_TEAMNAME_LENGTH];
		int i;

		num_teams = 0;
		strcpy(teamlist, CVAR_GET_STRING("mp_teamlist"));
		pName = teamlist;
		pName = strtok(pName, ";");

		while (pName != NULL && *pName)
		{
			// check that team isn't defined twice
			for (i=0; i < num_teams; i++)
			{
				if (strcmp(pName, team_names[i]) == 0)
				{
					break;
				}
			}

			if (i == num_teams)
			{
				strcpy(team_names[num_teams], pName);
				num_teams++;
			}
			pName = strtok(NULL, ";");
		}
	}

	// TODO: does pfnGetInfoKeyBuffer modify the edict_t*?
	infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( const_cast<edict_t*>(pEdict) );
	strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));

	for (int index=0; index < num_teams; index++)
	{
		if (strcmp(model_name, team_names[index]) == 0)
			return index;
	}

	return 0;
}

bool Game::IsGunmanChronicles() const
{
	return mod_id == REWOLF_DLL;
}

void Game::GetSaveGameComment( char *pBuffer, int iMaxLength ) const
{
	ALERT( at_console, "Setting save game comment to %s\n", STRING(gpGlobals->mapname) );
	strncpy( pBuffer, STRING(gpGlobals->mapname), iMaxLength );
	ALERT( at_console, "Save game comment is %s\n", pBuffer );
}

bool Game::UseToOpenDoor() const
{
	return false;
}
