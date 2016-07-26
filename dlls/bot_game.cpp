#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

bool Game::IsTeamPlay()
{
	return CVAR_GET_FLOAT( "mp_teamplay" ) > 0;
}