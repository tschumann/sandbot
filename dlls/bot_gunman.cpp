#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

GunmanBot::GunmanBot()
{
	weapon_t fists = {GUNMAN_WEAPON_FISTS, "weapon_fists", static_cast<CanUseWeapon>(&GunmanBot::CanUseFists), 1};
	weapon_t gausspistol_pulse = {GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", static_cast<CanUseWeapon>(&GunmanBot::CanUseGaussPistolPulse), 5};
	weapon_t gausspistol_charge = {GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", static_cast<CanUseWeapon>(&GunmanBot::CanUseGaussPistolCharge), 4};
	weapon_t gausspistol_rapid = {GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", static_cast<CanUseWeapon>(&GunmanBot::CanUseGaussPistolRapid), 3};
	weapon_t shotgun = {GUNMAN_WEAPON_SHOTGUN, "weapon_shotgun", static_cast<CanUseWeapon>(&GunmanBot::CanUseShotgun), 9};
	weapon_t minigun = {GUNMAN_WEAPON_MINIGUN, "weapon_minigun", static_cast<CanUseWeapon>(&GunmanBot::CanUseMinigun), 10};
	weapon_t beamgun = {GUNMAN_WEAPON_BEAMGUN, "weapon_beamgun", static_cast<CanUseWeapon>(&GunmanBot::CanUseBeamgun), 11};
	weapon_t chemgun = {GUNMAN_WEAPON_CHEMGUN, "weapon_chemgun", static_cast<CanUseWeapon>(&GunmanBot::CanUseChemgun)};
	weapon_t dml = {GUNMAN_WEAPON_DML, "weapon_dml", static_cast<CanUseWeapon>(&GunmanBot::CanUseDML), 8};
	weapon_t dmlgrenade = {GUNMAN_WEAPON_DMLGRENADE, "weapon_dmlgrenade", static_cast<CanUseWeapon>(&GunmanBot::CanUseDMLGrenade), 2};
	weapon_t aicore = {GUNMAN_WEAPON_AICORE, "weapon_aicore", static_cast<CanUseWeapon>(&GunmanBot::CanUseAICore), 0};

	this->weapons.push_back(fists);
	this->weapons.push_back(gausspistol_pulse);
	this->weapons.push_back(gausspistol_charge);
	this->weapons.push_back(gausspistol_rapid);
	this->weapons.push_back(shotgun);
	this->weapons.push_back(minigun);
	this->weapons.push_back(beamgun);
	this->weapons.push_back(chemgun);
	this->weapons.push_back(dml);
	this->weapons.push_back(dmlgrenade);
	this->weapons.push_back(aicore);
}

void GunmanBot::OnSpawn()
{
	this->iPistolMode = RANDOM_LONG(GunmanBot::PISTOL_PULSE, GunmanBot::PISTOL_RAPID);

	// currently the bots don't know that the charge requires 10 ammo
	if( this->iPistolMode == PISTOL_CHARGE )
	{
		this->iPistolMode = GunmanBot::PISTOL_PULSE;
	}
}

int GunmanBot::GetPistolMode()
{
	return this->iPistolMode;
}

void GunmanBot::UseGaussPistolPulse()
{
	FakeClientCommand( this->pEdict, "cust_11", NULL, NULL );
}

void GunmanBot::UseGaussPistolCharge()
{
	// gauss_bolt in logs
	FakeClientCommand( this->pEdict, "cust_12", NULL, NULL );
}

void GunmanBot::UseGaussPistolRapid()
{
	// gauss_charged in logs
	FakeClientCommand( this->pEdict, "cust_13", NULL, NULL );
}

void GunmanBot::UseGaussPistolSniper()
{
	FakeClientCommand( this->pEdict, "cust_14", NULL, NULL );
}

bool GunmanBot::CanUseFists()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	return this->DistanceToEnemy() < 32.0;
}

bool GunmanBot::CanUseGaussPistolPulse()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_GAUSSPISTOL].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 1024.0;
}

bool GunmanBot::CanUseGaussPistolCharge()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_GAUSSPISTOL].iAmmo1] < 10 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 256.0;
}

bool GunmanBot::CanUseGaussPistolRapid()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_GAUSSPISTOL].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 512.0;
}

bool GunmanBot::CanUseShotgun()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_SHOTGUN].iAmmo1] < 2 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 150.0;
}

bool GunmanBot::CanUseMinigun()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_MINIGUN].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 1024.0;
}

bool GunmanBot::CanUseBeamgun()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_BEAMGUN].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() < 512.0;
}

bool GunmanBot::CanUseChemgun()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_CHEMGUN].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() > 128.0 && this->DistanceToEnemy() < 512.0;
}

bool GunmanBot::CanUseDML()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_DML].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() > 256.0 && this->DistanceToEnemy() < 9999.0;
}

bool GunmanBot::CanUseDMLGrenade()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_DMLGRENADE].iAmmo1] < 1 )
	{
		return false;
	}

	return this->DistanceToEnemy() > 128.0 && this->DistanceToEnemy() < 512.0;
}

bool GunmanBot::CanUseAICore()
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	return false;
}