#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "game.h"
#include "wpt.h"
#include "waypoint.h"

Game::~Game()
{
}

void Game::Cleanup()
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

bool Game::IsGunmanChronicles()
{
	extern int mod_id;
	return mod_id == REWOLF_DLL;
}

void Game::GetSaveGameComment( char *pBuffer, int iMaxLength )
{
	strncpy( pBuffer, STRING(gpGlobals->mapname), iMaxLength );
}

bool Game::UseToOpenDoor()
{
	return false;
}

void ValveGame::GetSaveGameComment( char *pBuffer, int iMaxLength )
{
	// TODO: does GameUI.dll localise these names?
	if( !strncmp( STRING( gpGlobals->mapname ), "t0a0", strlen( "t0a0" ) ) ) // a, b, b1, b2, c, d
	{
		strncpy( pBuffer, "HAZARD COURSE", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c0a0", strlen( "c0a0" ) ) ) // a, b, c, d, e
	{
		strncpy( pBuffer, "BLACK MESA INBOUND", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a0" ) )
	{
		strncpy( pBuffer, "ANOMOLOUS MATERIALS", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a0a" ) )
	{
		strncpy( pBuffer, "ANOMOLOUS MATERIALS", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a0b" ) )
	{
		strncpy( pBuffer, "ANOMOLOUS MATERIALS", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a0c" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a0d" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a0e" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a1" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a1a" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a1b" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a1c" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a1d" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c1a1f" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c1a2", strlen( "c1a2" ) ) ) // a, b, c, d
	{
		strncpy( pBuffer, "OFFICE COMPLEX", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c1a3", strlen( "c1a3" ) ) ) // a, b, c, d
	{
		strncpy( pBuffer, "\"WE'VE GOT HOSTILES\"", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c1a4", strlen( "c1a4" ) ) ) // b, d, e, f, g, i, j, k
	{
		strncpy( pBuffer, "BLAST PIT", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c2a1", strlen( "c2a1" ) ) ) // a, b
	{
		strncpy( pBuffer, "POWER UP", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c2a2" , strlen( "c2a2" ) ) ) // a, b1, b2, c, d, e, f, g, h
	{
		strncpy( pBuffer, "ON A RAIL", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a3" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a3a" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a3b" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a3c" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a3d" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a3e" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4a" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4b" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4c" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4d" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4e" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4f" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c2a4g" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c2a5", strlen( "c2a5" ) ) ) // a, b, c, d, e, f, g, w, x
	{
		strncpy( pBuffer, "SURFACE TENSION", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a1" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a1a" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a1b" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a2" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a2a" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a2b" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a2c" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a2d" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a2e" ) )
	{
		strncpy( pBuffer, "LAMBDA CORE", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c3a2f" ) )
	{
		strncpy( pBuffer, "LAMBDA CORE", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a1" ) )
	{
		strncpy( pBuffer, "XEN", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a1a" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a1b" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a1c" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a1d" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a1e" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a1f" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strncmp( STRING( gpGlobals->mapname ), "c4a2", strlen( "c4a2" ) ) ) // a, b
	{
		strncpy( pBuffer, "GONARCH'S LAIR", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c4a3" ) )
	{
		strncpy( pBuffer, "NIHILANTH", iMaxLength );
	}
	else if( !strcmp( STRING( gpGlobals->mapname ), "c5a1" ) )
	{
		strncpy( pBuffer, "NIHILANTH", iMaxLength );
	}
	else
	{
		strncpy( pBuffer, STRING( gpGlobals->mapname ), iMaxLength );
	}
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