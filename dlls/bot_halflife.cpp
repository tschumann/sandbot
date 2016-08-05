#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

HalfLifeBot::HalfLifeBot()
{
	// TODO: somehow assign these to this->weapons[]
	weapon_t crowbar = {VALVE_WEAPON_CROWBAR, "weapon_crowbar", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseCrowbar)};
	weapon_t glock = {VALVE_WEAPON_GLOCK, "weapon_glock", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseGlock)};
}

bool HalfLifeBot::CanUseCrowbar()
{
	// TODO: add this check at a higher level? it will be basically the same for every weapon
	if( !(this->pEdict->v.weapons & (1<<VALVE_WEAPON_CROWBAR)) )
	{
		return false;
	}

	return this->DistanceToEnemy() < 32.0;
}

bool HalfLifeBot::CanUseGlock()
{
	extern bot_weapon_t weapon_defs[MAX_WEAPONS];

	// TODO: add this check at a higher level? it will be basically the same for every weapon
	if( !(this->pEdict->v.weapons & (1<<VALVE_WEAPON_GLOCK)) )
	{
		return false;
	}

	return this->DistanceToEnemy() < 8192.0 && this->m_rgAmmo[weapon_defs[VALVE_WEAPON_GLOCK].iAmmo1] >= 1;
}