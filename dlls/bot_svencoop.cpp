//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Sven Coop specific bot code
//
//=============================================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot_svencoop.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "wpt.h"
#include "waypoint.h"

SvenCoopBot::SvenCoopBot()
{
}

bool SvenCoopBot::ShouldTargetMonsters() const
{
	return true;
}