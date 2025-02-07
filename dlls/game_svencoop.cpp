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

#include "game_svencoop.h"

SvenCoopGame::SvenCoopGame(GameId gameId) : Game(gameId)
{
}

const char* SvenCoopGame::GetGameDir() const
{
	return "svencoop";
}