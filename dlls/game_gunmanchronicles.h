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

#ifndef __GAME_GUNMANCHRONICLES_H__
#define __GAME_GUNMANCHRONICLES_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class RewolfGame : public Game
{
public:
	RewolfGame(GameId gameId);
	RewolfGame(const RewolfGame&) = delete;
	RewolfGame(RewolfGame&&) = delete;
	RewolfGame& operator=(const RewolfGame other) = delete;
	RewolfGame& operator=(RewolfGame&& other) = delete;
};

#endif // __GAME_GUNMANCHRONICLES_H__