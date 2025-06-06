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

#include "game_gunmanchronicles.h"

RewolfGame::RewolfGame(GameId gameId) : Game(gameId)
{
}

const char* RewolfGame::GetGameDir() const
{
	return "rewolf";
}

bool RewolfGame::HasWeaponCustomisation() const
{
	return true;
}