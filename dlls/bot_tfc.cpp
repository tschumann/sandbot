#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

bool TFCBot::FindFlag()
{
	extern WAYPOINT waypoints[MAX_WAYPOINTS];
	extern FLAG_S flags[MAX_FLAGS];
	extern int num_flags;
	extern int team_allies[4];

	int team = UTIL_GetTeam(pEdict);
	float waypoint_distance = 0.0;
	float distance = 0.0;
	edict_t *pent = NULL;

	while ((pent = UTIL_FindEntityByClassname( pent, "item_tfgoal" )) != NULL)
	{
		if (pent->v.owner == pEdict)  // is this bot carrying the item?
		{
			// we are carrying the flag/card/ball
			bot_has_flag = TRUE;
			break;  // break out of while loop
		}
		// can bot see it?
		else if (FInViewCone( &pent->v.origin, pEdict ) && FVisible( pent->v.origin, pEdict))
		{
			// check if the flag has an owner...
			if (pent->v.owner != NULL)
			{
				// get the team for the owner of the flag...
				int player_team = UTIL_GetTeam(pent->v.owner);

				// attack if not our team and not allies team...
				if ((player_team != team) && !(team_allies[team] & (1<<player_team)))
				{
					// kill the man with the flag!
					this->pBotEnemy = pent->v.owner;
					this->waypoint_goal = -1;  // forget the goal (if there is one)

					return TRUE;
				}
			}
			else
			{
				// check if it matches one of the flags...
				for (int i=0; i < num_flags; i++)
				{
					// is the flag for this team (or any team)?
					if ((flags[i].edict == pent) && ((flags[i].team_no == (team+1)) || (flags[i].team_no == 0)))
					{
						// find the nearest waypoint to the ball...
						index = WaypointFindNearest(pEdict, 500, team, pent->v.origin);

						if (index == -1)
						{
							// no waypoint is close enough, just head straight towards the ball
							Vector v_flag = pent->v.origin - pEdict->v.origin;
							Vector bot_angles = UTIL_VecToAngles( v_flag );

							pEdict->v.ideal_yaw = bot_angles.y;

							BotFixIdealYaw(pEdict);

							return TRUE;
						}
						else
						{
							waypoint_distance = (waypoints[index].origin - pent->v.origin).Length();
							distance = (pent->v.origin - pEdict->v.origin).Length();

							// is the bot closer to the ball than the waypoint is?
							if (distance < waypoint_distance)
							{
								// just head towards the ball
								Vector v_flag = pent->v.origin - pEdict->v.origin;
								Vector bot_angles = UTIL_VecToAngles( v_flag );

								pEdict->v.ideal_yaw = bot_angles.y;

								BotFixIdealYaw(pEdict);

								return TRUE;
							}
							else
							{
								// head towards this waypoint
								this->waypoint_goal = index;

								// remember where the ball is located...
								this->waypoint_near_flag = TRUE;
								this->waypoint_flag_origin = pent->v.origin;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

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