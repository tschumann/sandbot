//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Half-Life specific bot code
//
//=============================================================================

#ifndef __BOT_HALFLIFE_H__
#define __BOT_HALFLIFE_H__

#include "bot.h"
#include "game.h"

class HalfLifeBot : public bot_t
{
public:
	HalfLifeBot();

	virtual int GetPistol() const;

	virtual bool CanUseCrowbar(bool really) const;
	virtual bool CanUseGlock(bool really) const;
	virtual bool CanUseMP5Primary(bool really) const;

	virtual bool CanUseEgon(bool really) const;
};

#endif // __BOT_HALFLIFE_H__