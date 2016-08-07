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
	weapon_t crowbar = {VALVE_WEAPON_CROWBAR, "weapon_crowbar", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseCrowbar)};
	weapon_t glock = {VALVE_WEAPON_GLOCK, "weapon_glock", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseGlock)};
	weapon_t mp5 = {VALVE_WEAPON_MP5, "weapon_mp5", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseMP5Primary)};

	this->weapons.push_back(crowbar);
	this->weapons.push_back(glock);
	this->weapons.push_back(mp5);
}

bool HalfLifeBot::CanUseCrowbar()
{
	return this->DistanceToEnemy() < 32.0;
}

bool HalfLifeBot::CanUseGlock()
{
	if( this->m_rgAmmo[weapon_defs[VALVE_WEAPON_GLOCK].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 8192.0;
}

bool HalfLifeBot::CanUseMP5Primary()
{
	if( this->IsUnderWater() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[VALVE_WEAPON_MP5].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 8192.0;
}