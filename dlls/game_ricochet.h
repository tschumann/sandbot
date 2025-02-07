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

#ifndef __GAME_RICOCHET_H__
#define __GAME_RICOCHET_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class RicochetGame : public Game
{
public:
	RicochetGame(GameId gameId);
	RicochetGame(const RicochetGame&) = delete;
	RicochetGame(RicochetGame&&) = delete;
	RicochetGame& operator=(const RicochetGame other) = delete;
	RicochetGame& operator=(RicochetGame&& other) = delete;

	virtual const char* GetGameDir() const;

	virtual bool CanChoosePlayerModel() const
	{
		return true;
	}
};

#endif // __GAME_RICOCHET_H__