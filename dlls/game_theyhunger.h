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

#ifndef __GAME_THEYHUNGER_H__
#define __GAME_THEYHUNGER_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class HungerGame : public Game
{
public:
	HungerGame(GameId gameId);
	HungerGame(const HungerGame&) = delete;
	HungerGame(HungerGame&&) = delete;
	HungerGame& operator=(const HungerGame other) = delete;
	HungerGame& operator=(HungerGame&& other) = delete;
};

#endif // __GAME_THEYHUNGER_H__