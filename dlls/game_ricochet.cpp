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

#include "game_ricochet.h"

RicochetGame::RicochetGame(GameId gameId) : Game(gameId)
{
}

const char* RicochetGame::GetGameDir() const
{
	return "ricochet";
}