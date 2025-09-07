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

#include "game_deathmatchclassic.h"

DeathmatchClassicGame::DeathmatchClassicGame(GameId gameId) : Game(gameId)
{
}

const char* DeathmatchClassicGame::GetGameDir() const
{
	return "dmc";
}

Game::bot_player_t* DeathmatchClassicGame::GetBotProfiles() const
{
	return g_valveBots;
}