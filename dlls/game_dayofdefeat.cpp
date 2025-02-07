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

#include "game_dayofdefeat.h"

DODGame::DODGame( GameId gameId ) : Game( gameId )
{
}

const char *DODGame::GetGameDir() const
{
	return "dod";
}

int DODGame::GetTeam( const edict_t *pEdict ) const
{
	return pEdict->v.team;
}