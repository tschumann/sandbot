//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Sven Coop specific bot code
//
//=============================================================================

#ifndef __BOT_SVENCOOP_H__
#define __BOT_SVENCOOP_H__

#include "bot.h"
#include "game.h"

class SvenCoopBot : public bot_t
{
public:
	SvenCoopBot();

	virtual bool ShouldTargetMonsters() const;
};

#endif // __BOT_SVENCOOP_H__