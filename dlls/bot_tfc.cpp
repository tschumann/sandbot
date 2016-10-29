#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

void TFCBot::Build()
{
	int value = RANDOM_LONG(1, 100);
	int team = UTIL_GetTeam(pEdict);

	if (((value <= 70) && (this->sentrygun_level < 3)) || (value <= 40))
	{
		// build or upgrade a sentry gun...
		index = -1;

		// do we need more metal to build a sentry gun?
		if (this->m_rgAmmo[weapon_defs[TF_WEAPON_SPANNER].iAmmo1] < 130)
		{
			// go find some metal...
			index = WaypointFindNearestGoal(pEdict, this->curr_waypoint_index, team, W_FL_ARMOR);
		}
		else  // otherwise we have enough metal...
		{
			if (this->sentrygun_waypoint == -1)
			{
				// find a random sentry gun waypoint...
				index = WaypointFindRandomGoal(pEdict, team, W_FL_SENTRYGUN);
			}
			else
			{
				index = this->sentrygun_waypoint;
			}
		}

		if (index != -1)
			this->waypoint_goal = index;
	}
	else
	{
		// build a dispenser...
		index = -1;

		// do we need more metal to build a dispenser?
		if (this->m_rgAmmo[weapon_defs[TF_WEAPON_SPANNER].iAmmo1] < 100)
		{
			// go find some metal...
			index = WaypointFindNearestGoal(pEdict, this->curr_waypoint_index, team, W_FL_ARMOR);
		}
		else  // otherwise we have enough metal...
		{
			if (this->dispenser_waypoint == -1)
			{
				index = WaypointFindRandomGoal(pEdict, team, W_FL_DISPENSER);
			}
			else
			{
				index = this->dispenser_waypoint;
			}
		}

		if (index != -1)
			this->waypoint_goal = index;
	}
}