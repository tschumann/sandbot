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

#ifndef __GAME_DEATHMATCHCLASSIC_H__
#define __GAME_DEATHMATCHCLASSIC_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class DeathmatchClassicGame : public Game
{
public:
	DeathmatchClassicGame(GameId gameId);
	DeathmatchClassicGame(const DeathmatchClassicGame&) = delete;
	DeathmatchClassicGame(DeathmatchClassicGame&&) = delete;
	DeathmatchClassicGame& operator=(const DeathmatchClassicGame other) = delete;
	DeathmatchClassicGame& operator=(DeathmatchClassicGame&& other) = delete;

	virtual bool CanChoosePlayerModel() const
	{
		return true;
	}
};

#endif // __GAME_DEATHMATCHCLASSIC_H__