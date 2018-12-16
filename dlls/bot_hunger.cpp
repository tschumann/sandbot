#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "waypoint.h"

HungerBot::HungerBot()
{
}

int HungerBot::GetPistol()
{
	return VALVE_WEAPON_GLOCK;
}
