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

#include "game_naturalselection.h"

NSGame::NSGame( GameId gameId ) : Game( gameId )
{
}

const char* NSGame::GetGameDir() const
{
	return "ns";
}

int NSGame::GetTeam( const edict_t *pEdict ) const
{
	return pEdict->v.team;
}