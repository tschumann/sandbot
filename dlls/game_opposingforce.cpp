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

#include "game_opposingforce.h"

GearboxGame::GearboxGame( GameId gameId ) : Game( gameId )
{
}

int GearboxGame::GetTeam( const edict_t *pEdict ) const
{
	if( this->IsCTF() || this->IsCapturePoint() )
	{
		// TODO: does pfnGetInfoKeyBuffer modify the edict_t*?
		char *infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( const_cast<edict_t *>(pEdict) );
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