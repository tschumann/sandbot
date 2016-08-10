#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

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