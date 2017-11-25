#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

HungerBot::HungerBot()
{
}

int HungerBot::GetPistol()
{
	return VALVE_WEAPON_GLOCK;
}
