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
	weapon_t mp5 = {VALVE_WEAPON_MP5, "weapon_mp5", static_cast<CanUseWeapon>(&HalfLifeBot::CanUseMP5Primary)};

	this->weapons.push_back(crowbar);
	this->weapons.push_back(glock);
	this->weapons.push_back(mp5);
}

std::vector<weapon_t> HalfLifeBot::GetUsableWeapons()
{
	std::vector<weapon_t> usableWeapons;

	for( unsigned int i = 0; i < this->weapons.size(); i++ )
	{
		CanUseWeapon pfnCanUseWeapon = this->weapons[i].pfnCanUseWeapon;
		if( (this->*pfnCanUseWeapon)() )
		{
			ALERT( at_console, "can use %s\n", this->weapons[i].szWeaponName );
		}
	}

	return usableWeapons;
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

	if( this->m_rgAmmo[weapon_defs[VALVE_WEAPON_GLOCK].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 8192.0;
}

bool HalfLifeBot::CanUseMP5Primary()
{
	extern bot_weapon_t weapon_defs[MAX_WEAPONS];

	// TODO: add this check at a higher level? it will be basically the same for every weapon
	if( !(this->pEdict->v.weapons & (1<<VALVE_WEAPON_MP5)) )
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

	return this->DistanceToEnemy() < 8192.0;
}