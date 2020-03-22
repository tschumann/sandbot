#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "wpt.h"
#include "waypoint.h"

GunmanBot::GunmanBot()
{
	weapon_t fists = {GUNMAN_WEAPON_FISTS, "weapon_fists", static_cast<CanUseWeapon>(&GunmanBot::CanUseFists)};
	weapon_t gausspistol_pulse = {GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", static_cast<CanUseWeapon>(&GunmanBot::CanUseGaussPistolPulse)};
	weapon_t gausspistol_charge = {GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", static_cast<CanUseWeapon>(&GunmanBot::CanUseGaussPistolCharge)};
	weapon_t gausspistol_rapid = {GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", static_cast<CanUseWeapon>(&GunmanBot::CanUseGaussPistolRapid)};
	weapon_t shotgun = {GUNMAN_WEAPON_SHOTGUN, "weapon_shotgun", static_cast<CanUseWeapon>(&GunmanBot::CanUseShotgun)};
	weapon_t minigun = {GUNMAN_WEAPON_MINIGUN, "weapon_minigun", static_cast<CanUseWeapon>(&GunmanBot::CanUseMinigun)};
	weapon_t beamgun = {GUNMAN_WEAPON_BEAMGUN, "weapon_beamgun", static_cast<CanUseWeapon>(&GunmanBot::CanUseBeamgun)};
	weapon_t chemgun = {GUNMAN_WEAPON_CHEMGUN, "weapon_chemgun", static_cast<CanUseWeapon>(&GunmanBot::CanUseChemgun)};
	weapon_t dml = {GUNMAN_WEAPON_DML, "weapon_dml", static_cast<CanUseWeapon>(&GunmanBot::CanUseDML)};
	weapon_t dmlgrenade = {GUNMAN_WEAPON_DMLGRENADE, "weapon_dmlgrenade", static_cast<CanUseWeapon>(&GunmanBot::CanUseDMLGrenade)};
	weapon_t aicore = {GUNMAN_WEAPON_AICORE, "weapon_aicore", static_cast<CanUseWeapon>(&GunmanBot::CanUseAICore)};

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
	bot_t::OnSpawn();

	this->iPistolMode = RANDOM_LONG(GunmanBot::PISTOL_PULSE, GunmanBot::PISTOL_RAPID);

	// currently the bots don't know that the charge requires 10 ammo so would get stuck using it
	if( this->iPistolMode == PISTOL_CHARGE )
	{
		this->iPistolMode = GunmanBot::PISTOL_PULSE;
	}
}

void GunmanBot::Think()
{
	bot_t::PreThink();

	if( this->GetPistolMode() == GunmanBot::PISTOL_PULSE )
	{
		this->UseGaussPistolPulse();
	}
	else if( this->GetPistolMode() == GunmanBot::PISTOL_CHARGE )
	{
		this->UseGaussPistolCharge();
	}
	else if( this->GetPistolMode() == GunmanBot::PISTOL_RAPID )
	{
		this->UseGaussPistolRapid();
	}
	else if( this->GetPistolMode() == GunmanBot::PISTOL_SNIPER )
	{
		this->UseGaussPistolSniper();
	}

	bot_t::PostThink();
}

bool GunmanBot::ShouldReload() const
{
	return false;
}

bool GunmanBot::IsSniping() const
{
	// TODO: this->charging_weapon_id == GUNMAN_WEAPON_GAUSSPISTOL && this->GetPistolMode() == GunmanBot::PISTOL_SNIPER etc
	return false;
}

int GunmanBot::GetPistolMode()
{
	return this->iPistolMode;
}

void GunmanBot::UseGaussPistolPulse()
{
	FakeClientCommand( this->pEdict, "cust_11" );
}

void GunmanBot::UseGaussPistolCharge()
{
	// gauss_bolt in logs
	FakeClientCommand( this->pEdict, "cust_12" );
}

void GunmanBot::UseGaussPistolRapid()
{
	// gauss_charged in logs
	FakeClientCommand( this->pEdict, "cust_13" );
}

void GunmanBot::UseGaussPistolSniper()
{
	FakeClientCommand( this->pEdict, "cust_14" );
}

bool GunmanBot::CanUseFists( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 32.0 || really;
}

bool GunmanBot::CanUseGaussPistolPulse( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_GAUSSPISTOL].iAmmo1] < 1 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 1024.0 || really;
}

bool GunmanBot::CanUseGaussPistolCharge( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_GAUSSPISTOL].iAmmo1] < 10 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 256.0 || really;
}

bool GunmanBot::CanUseGaussPistolRapid( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_GAUSSPISTOL].iAmmo1] < 1 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 512.0 || really;
}

bool GunmanBot::CanUseShotgun( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_SHOTGUN].iAmmo1] < 2 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 150.0 || really;
}

bool GunmanBot::CanUseMinigun( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_MINIGUN].iAmmo1] < 1 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 1024.0 || really;
}

bool GunmanBot::CanUseBeamgun( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_BEAMGUN].iAmmo1] < 1 )
	{
		return false;
	}

	return this->GetDistanceToEnemy() < 512.0 || really;
}

bool GunmanBot::CanUseChemgun( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_CHEMGUN].iAmmo1] < 1 )
	{
		return false;
	}

	return( this->GetDistanceToEnemy() > 128.0 && this->GetDistanceToEnemy() < 512.0 ) || really;
}

bool GunmanBot::CanUseDML( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_DML].iAmmo1] < 1 )
	{
		return false;
	}

	return ( this->GetDistanceToEnemy() > 256.0 && this->GetDistanceToEnemy() < 9999.0 ) || really;
}

bool GunmanBot::CanUseDMLGrenade( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	if( this->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_DMLGRENADE].iAmmo1] < 1 )
	{
		return false;
	}

	return ( this->GetDistanceToEnemy() > 128.0 && this->GetDistanceToEnemy() < 512.0 ) || really;
}

bool GunmanBot::CanUseAICore( bool really )
{
	if( !bot_t::BaseCanUseWeapon() )
	{
		return false;
	}

	return false || really;
}