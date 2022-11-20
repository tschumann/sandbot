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

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "wpt.h"
#include "waypoint.h"

HungerBot::HungerBot()
{
}

int HungerBot::GetPistol()
{
	return VALVE_WEAPON_GLOCK;
}
