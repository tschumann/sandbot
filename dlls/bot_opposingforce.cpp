#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

void OpposingForceBot::Join()
{
	if (pGame->IsCTF())
	{
		// handle Opposing Force CTF stuff here...

		if (this->start_action == MSG_OPFOR_TEAM_SELECT)
		{
			this->start_action = MSG_OPFOR_IDLE;  // switch back to idle

			// TODO: team 3? there are only two teams?
			if ((this->bot_team != 1) && (this->bot_team != 2) && (this->bot_team != 3))
				this->bot_team = RANDOM_LONG(1, 2);

			FakeClientCommand(pEdict, "jointeam %d", this->bot_team);

			return;
		}

		if (this->start_action == MSG_OPFOR_CLASS_SELECT)
		{
			this->start_action = MSG_OPFOR_IDLE;  // switch back to idle

			if ((this->bot_class < 0) || (this->bot_class > 10))
				this->bot_class = RANDOM_LONG(1, 10);

			// select the class the bot wishes to use...
			if (this->bot_class > 7)
				this->bot_class = 7;

			FakeClientCommand(pEdict, "selectchar %d", this->bot_class);

			// bot has now joined the game (doesn't need to be started)
			this->not_started = 0;

			return;
		}
	}
	else
	{
		this->not_started = 0;
	}
}

int OpposingForceBot::GetPistol()
{
	return GEARBOX_WEAPON_GLOCK;
}

bool OpposingForceBot::FindFlag()
{
	extern WAYPOINT waypoints[MAX_WAYPOINTS];

	int skin = 0;
	int team = UTIL_GetTeam(pEdict);
	float waypoint_distance = 0.0;
	float distance = 0.0;
	edict_t *pent = NULL;

	while ((pent = UTIL_FindEntityByClassname( pent, "item_ctfflag" )) != NULL)
	{
		// is this bot carrying the item? (after capture bug fix by Whistler)
		if ((pent->v.owner == pEdict) && (pent->v.origin == this->GetOrigin()))
		{
			// we are carrying the flag
			bot_has_flag = TRUE;

			break;  // break out of while loop
		}
		// if the flag is visible
		else if (FInViewCone( &pent->v.origin, pEdict ) && FVisible( pent->v.origin, pEdict))
		{
			// the bot can see it, check what type of model it is...
			skin = pent->v.skin;

			if (skin == 0) // Opposing Force team (these are BACKASSWARDS!)
				skin = 1;
			else if (skin == 1) // Black Mesa team
				skin = 0;

			// see if the flag matches the bot's team...
			if (skin == team)
			{
				// is and enemy carrying our flag/card?
				if (pent->v.owner != NULL)
				{
					// kill the man with the flag/card!
					this->pBotEnemy = pent->v.owner;

					this->waypoint_goal = -1;  // forget the goal (if there is one)

					return TRUE;
				}
			}
			else  // flag/card is for another team!
			{
				// check if someone is NOT carrying the flag/card...
				if (pent->v.owner == NULL)
				{
					// find the nearest waypoint to the flag/card...
					index = WaypointFindNearest(pEdict, 500, team, pent->v.origin);

					if (index == -1)
					{
						// no waypoint is close enough, just head straight towards the flag/card
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

						// is the bot closer to the flag/card than the waypoint is?
						if (distance < waypoint_distance)
						{
							// just head towards the flag/card
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

							// remember where the flag/card is located...
							this->waypoint_near_flag = TRUE;
							this->waypoint_flag_origin = pent->v.origin;
						}
					}
				}
			}
		}
	}

	return false;
}