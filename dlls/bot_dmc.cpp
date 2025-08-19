//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Deathmatch Classic specific bot code
//
//=============================================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot_dmc.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "game_deathmatchclassic.h"
#include "wpt.h"
#include "waypoint.h"

DMCBot::DMCBot()
{
}

void DMCBot::Join()
{
	this->not_started = 0;
	// g_engfuncs.pfnClientCommand(pEdict, "_firstspawn");
	//this->pEdict->v.effects &= ~EF_NODRAW;
	//this->pEdict->v.solid = SOLID_SLIDEBOX;
	//this->pEdict->v.takedamage = DAMAGE_AIM;
	//this->pEdict->v.movetype = MOVETYPE_WALK;
}

bool DMCBot::HasWeapon( int iWeaponId ) const
{
	return this->pEdict->v.weapons & iWeaponId;
}