//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Deathmatch Classic specific bot code
//
//=============================================================================

#ifndef __BOT_DMC_H__
#define __BOT_DMC_H__

#include "bot.h"
#include "bot_halflife.h"
#include "game.h"

class DMCBot : public HalfLifeBot
{
public:
	DMCBot();

	virtual void Join();

	virtual bool HasWeapon( int iWeaponId ) const;
};

#endif // __BOT_DMC_H__