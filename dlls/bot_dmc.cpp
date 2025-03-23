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

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "game_deathmatchclassic.h"
#include "wpt.h"
#include "waypoint.h"

DMCBot::DMCBot()
{
}

void DMCBot::Join()
{
	this->not_started = 0;
	// FakeClientCommand(pEdict, "_firstspawn");
}