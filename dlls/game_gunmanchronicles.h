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

	virtual const char* GetGameDir() const;

	virtual bool CanChoosePlayerModel() const
	{
		return true;
	}

	virtual bool HasWeaponCustomisation() const;
};

#endif // __GAME_GUNMANCHRONICLES_H__