#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

// http://www.unknownworlds.com/oldwebsite/manuals/Natural_Selection_Manual.html
// http://www.unknownworlds.com/oldwebsite/manuals/comm_manual/basic/index.htm

bool NSBot::IsNearHive()
{
	edict_t *pent = NULL;

	// TODO: cache the results of this in NSGame
	while( pent = UTIL_FindEntityByClassname( pent, "team_hive" ) )
	{
		float distance = (pent->v.origin - this->pEdict->v.origin).Length();

		if( distance <= 600 )
		{
			return true;
		}
	}

	return false;
}

bool NSBot::HasWeaponDamage1()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_1;
}

bool NSBot::HasShotgun()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_SHOTGUN));
}

bool NSBot::HasHMG()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_HEAVYMACHINEGUN));
}

void NSBot::UpgradeToWeaponDamage1()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_WEAPON_DAMAGE_1;
}

void NSBot::UpgradeToShotgun()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_SHOTGUN;
}

void NSBot::UpgradeToHMG()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_HMG;
}

bool NSBot::HasCarapace()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_1;
}

void NSBot::UpgradeToCarapace()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_CARAPACE;
}

void NSBot::EvolveToFade()
{
	if( ((NSGame *)pGame)->IsCombat() )
	{
		this->pEdict->v.impulse = NSBot::EVOLVE_TO_FADE;
	}
	else
	{
		// TODO:
	}
}

bool NSBot::IsFade()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER4;
}