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

#include "game_teamfortressclassic.h"

TFCGame::TFCGame( GameId gameId ) : Game( gameId )
{
}

const char* TFCGame::GetGameDir() const
{
	return "tfc";
}

int TFCGame::GetTeam( const edict_t *pEdict ) const
{
	return pEdict->v.team - 1;
}
