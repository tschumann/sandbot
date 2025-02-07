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

#ifndef __GAME_HALFLIFE_H__
#define __GAME_HALFLIFE_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class ValveGame : public Game
{
public:
	ValveGame(GameId gameId);
	ValveGame(const ValveGame&) = delete;
	ValveGame(ValveGame&&) = delete;
	ValveGame& operator=(const ValveGame other) = delete;
	ValveGame& operator=(ValveGame&& other) = delete;

	virtual const char* GetGameDir() const;

	virtual bool CanChoosePlayerModel() const
	{
		return true;
	}

	virtual void GetSaveGameComment(char* pBuffer, int iMaxLength) const;
};

#endif // __GAME_HALFLIFE_H__