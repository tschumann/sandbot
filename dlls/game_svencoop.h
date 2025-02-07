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

#ifndef __GAME_SVENCOOP_H__
#define __GAME_SVENCOOP_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class SvenCoopGame : public Game
{
public:
	SvenCoopGame(GameId gameId);
	SvenCoopGame(const SvenCoopGame&) = delete;
	SvenCoopGame(SvenCoopGame&&) = delete;
	SvenCoopGame& operator=(const SvenCoopGame other) = delete;
	SvenCoopGame& operator=(SvenCoopGame&& other) = delete;

	virtual const char* GetGameDir() const;

	virtual bool CanChoosePlayerModel() const
	{
		return true;
	}
};

#endif // __GAME_SVENCOOP_H__