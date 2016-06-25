#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

// TODO: this should be in its own subclass
void bot_t::_Gunman_OnSpawn()
{
	this->iPistolMode = RANDOM_LONG(1, 3);
}

int bot_t::GetPistolMode()
{
	return this->iPistolMode;
}

void bot_t::UseGaussPistolPulse()
{
	FakeClientCommand( this->pEdict, "cust_11", NULL, NULL );
}

void bot_t::UseGaussPistolCharge()
{
	// gauss_bolt in logs
	FakeClientCommand( this->pEdict, "cust_12", NULL, NULL );
}

void bot_t::UseGaussPistolRapid()
{
	// gauss_charged in logs
	FakeClientCommand( this->pEdict, "cust_13", NULL, NULL );
}

void bot_t::UseGaussPistolSniper()
{
	FakeClientCommand( this->pEdict, "cust_14", NULL, NULL );
}