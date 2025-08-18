//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: The Ship specific bot code
//
//=============================================================================

#ifndef __BOT_SHIP_H__
#define __BOT_SHIP_H__

#include "bot.h"
#include "game.h"

class ShipBot : public bot_t
{
public:
	ShipBot();

	virtual bool IsValidEnemy(const edict_t* pEnemy);
	virtual void PickUpItem();

	virtual bool ShouldJumpAfterDeath();

	virtual void SetQuarry(int iEntIndex);
	virtual edict_t* GetQuarry();
	virtual bool HasQuarry();
protected:
	edict_t* pQuarry;
};

#endif // __BOT_SHIP_H__