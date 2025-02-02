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

#ifndef __GAME_THESHIP_H__
#define __GAME_THESHIP_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class ShipGame : public Game
{
public:
	ShipGame( GameId gameId );
	ShipGame(const ShipGame&) = delete;
	ShipGame(ShipGame&&) = delete;
	ShipGame& operator=(const ShipGame other) = delete;
	ShipGame& operator=(ShipGame&& other) = delete;

	virtual bool CanChoosePlayerModel() const
	{
		return false;
	}

	virtual bool UseToOpenDoor() const
	{
		return true;
	}
};

#endif // __GAME_THESHIP_H__