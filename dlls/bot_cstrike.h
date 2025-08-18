//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Counter-Strike specific bot code
//
//=============================================================================

#ifndef __BOT_CSTRIKE_H__
#define __BOT_CSTRIKE_H__

#include "bot.h"
#include "game.h"

class CStrikeBot : public bot_t
{
public:
	CStrikeBot();

	virtual void Join();

	virtual float GetMaxSpeed() const;
};

#endif // __BOT_CSTRIKE_H__