//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: They Hunger specific bot code
//
//=============================================================================

#ifndef __BOT_HUNGER_H__
#define __BOT_HUNGER_H__

#include "bot_halflife.h"
#include "game.h"

class HungerBot : public HalfLifeBot
{
public:
	HungerBot();

	virtual int GetPistol();
};

#endif // __BOT_HUNGER_H__