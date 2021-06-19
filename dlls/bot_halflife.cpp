#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "wpt.h"
#include "waypoint.h"

HalfLifeBot::HalfLifeBot()
{
	weapon_t crowbar = {VALVE_WEAPON_CROWBAR, "weapon_crowbar", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseCrowbar)};
	weapon_t glock = {VALVE_WEAPON_GLOCK, "weapon_glock", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseGlock)};
	weapon_t mp5 = {VALVE_WEAPON_MP5, "weapon_mp5", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseMP5Primary)};

	weapon_t egon = {VALVE_WEAPON_EGON, "weapon_egon", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseEgon)};

	this->weapons.push_back(crowbar);
	this->weapons.push_back(glock);
	this->weapons.push_back(mp5);
}

int HalfLifeBot::GetPistol() const
{
	return VALVE_WEAPON_GLOCK;
}

bool HalfLifeBot::CanUseCrowbar( bool really ) const
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 32.0 || really;
}

bool HalfLifeBot::CanUseGlock( bool really ) const
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[VALVE_WEAPON_GLOCK].iAmmo1] < 1 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 8192.0 || really;
}

bool HalfLifeBot::CanUseMP5Primary( bool really ) const
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->IsUnderWater() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[VALVE_WEAPON_MP5].iAmmo1] < 1 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 8192.0 || really;
}

bool HalfLifeBot::CanUseEgon( bool really ) const
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->IsUnderWater() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[VALVE_WEAPON_EGON].iAmmo1] < 1 )
	{
		return false;
	}

	return ( this->GetDistanceToEnemy() > 128.0 && this->GetDistanceToEnemy() < 8192.0 ) || really;
}