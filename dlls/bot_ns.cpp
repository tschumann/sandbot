#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

bool bot_t::IsNearHive()
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

bool bot_t::HasWeaponDamage1()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_1;
}

bool bot_t::HasShotgun()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_SHOTGUN));
}

bool bot_t::HasHMG()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_HEAVYMACHINEGUN));
}

void bot_t::UpgradeToWeaponDamage1()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_WEAPON_DAMAGE_1;
}

void bot_t::UpgradeToShotgun()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_SHOTGUN;
}

void bot_t::UpgradeToHMG()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_HMG;
}

bool bot_t::HasCarapace()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_1;
}

void bot_t::UpgradeToCarapace()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_CARAPACE;
}

void bot_t::EvolveToFade()
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

bool bot_t::IsFade()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER4;
}