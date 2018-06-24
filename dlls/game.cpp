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

bool Game::IsDeathmatch()
{
	return true;
}

bool Game::IsCTF()
{
	return false;
}

bool Game::IsCapturePoint()
{
	return false;
}

unsigned int Game::BotsOnTeam( int team )
{
	int iOnTeam = 0;

	for( int i = 0; i < MAX_PLAYERS; i++ )
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
bool Game::IsValidEdict( edict_t *pEdict )
{
	return IsValidEntity( pEdict );
}

bool Game::CanChoosePlayerModel()
{
	return true;
}

int Game::GetTeam( edict_t *pEdict )
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

	infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEdict );
	strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));

	for (int index=0; index < num_teams; index++)
	{
		if (strcmp(model_name, team_names[index]) == 0)
			return index;
	}

	return 0;
}

void Game::SetGame( eGame game )
{
	this->m_iModId = game;
}

bool Game::IsGunmanChronicles()
{
	return this->m_iModId == eGame::REWOLF;
}

int GearboxGame::GetTeam( edict_t *pEdict )
{
	if( this->IsCTF() || !this->IsCapturePoint() )
	{
		char *infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEdict );
		char szModelName[32];

		strcpy( szModelName, g_engfuncs.pfnInfoKeyValue(infobuffer, "model") );

		if( !strcmp(szModelName, "ctf_barney") || !strcmp(szModelName, "cl_suit") || !strcmp(szModelName, "ctf_gina") ||
			!strcmp(szModelName, "ctf_gordon") || !strcmp(szModelName, "otis") || !strcmp(szModelName, "ctf_scientist") )
		{
			return OpposingForceBot::TEAM_BLACK_MESA;
		}
		else if( !strcmp(szModelName, "beret") || !strcmp(szModelName, "drill") || !strcmp(szModelName, "grunt") ||
			!strcmp(szModelName, "recruit") || !strcmp(szModelName, "shephard") || !strcmp(szModelName, "tower") )
		{
			return OpposingForceBot::TEAM_OPPOSING_FORCE;
		}

		// unknown team
		return 0;
	}

	// use default deathmatch behaviour
	return Game::GetTeam( pEdict );
}

int CStrikeGame::GetTeam( edict_t *pEdict )
{
	char *infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEdict );
	char szModelName[32];

	strcpy(szModelName, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));

	if( !strcmp(szModelName, "terror") || !strcmp(szModelName, "arab") ||! strcmp(szModelName, "leet") ||
		!strcmp(szModelName, "arctic") || !strcmp(szModelName, "guerilla") )
	{
		return 0;
	}
	else if( !strcmp(szModelName, "urban") || !strcmp(szModelName, "gsg9") || !strcmp(szModelName, "sas") ||
		!strcmp(szModelName, "gign") || !strcmp(szModelName, "vip") )
	{
		return 1;
	}

	// unknown team
	return 0;
}

int DODGame::GetTeam( edict_t *pEdict )
{
	return pEdict->v.team;
}

int TFCGame::GetTeam( edict_t *pEdict )
{
	return pEdict->v.team - 1;
}

int NSGame::GetTeam( edict_t *pEdict )
{
	return pEdict->v.team;
}