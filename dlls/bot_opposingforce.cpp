#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "wpt.h"
#include "waypoint.h"

OpposingForceBot::OpposingForceBot()
{
}

void OpposingForceBot::Join()
{
	if( pGame->IsCTF() || pGame->IsCapturePoint() )
	{
		ALERT( at_console, "OpposingForce bot joining a CTF game - action %d\n", this->start_action );
		if (this->start_action == MSG_OPFOR_TEAM_SELECT)
		{
			this->start_action = MSG_OPFOR_IDLE;  // switch back to idle

			if ((this->bot_team != 1) && (this->bot_team != 2))
			{
				this->bot_team = RANDOM_LONG(1, 2);
			}

			FakeClientCommand(pEdict, "jointeam %d", this->bot_team);

			return;
		}
		if (this->start_action == MSG_OPFOR_CLASS_SELECT)
		{
			this->start_action = MSG_OPFOR_IDLE;  // switch back to idle

			if ((this->bot_class < 0) || (this->bot_class > 7))
			{
				this->bot_class = RANDOM_LONG(1, 7);
			}

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

void OpposingForceBot::PreThink()
{
	// this is a bit of hack (although you could argue that bouncing around until you die in multiplayer
	// is a bug) but without this it's almost impossible for bots to get the enemy flag on op4ctf_hairball
	if( pGame->IsCTF() && !strcmp(STRING(gpGlobals->mapname), "op4ctf_hairball") )
	{
		// TODO: possibly could be less than 1.0 and the bots would be okay?
		this->pEdict->v.friction = 1.0f;
	}
}

bool OpposingForceBot::ShouldSeekEnemy()
{
	if( this->HasFlag() )
	{
		// only seek out enemies if health is low
		return this->GetHealth() < 40;
	}
	else
	{
		return true;
	}
}

int OpposingForceBot::GetPistol()
{
	return GEARBOX_WEAPON_GLOCK;
}

int OpposingForceBot::GetGoalType()
{
	if( pGame->IsCTF() )
	{
		// TODO: has flag is probably pretty slow to run each frame for each bot - only check it sometimes?
		if( this->HasFlag() )
		{
			// find a base
			return W_FL_FLAG_GOAL;
		}
		return W_FL_FLAG;
	}
	else if( pGame->IsCapturePoint() )
	{
		if( UTIL_GetTeam(this->pEdict) == OpposingForceBot::TEAM_BLACK_MESA )
		{
			return W_FL_OP4_CAPTURE_POINT_BM;
		}
		else if( UTIL_GetTeam(this->pEdict) == OpposingForceBot::TEAM_OPPOSING_FORCE )
		{
			return W_FL_OP4_CAPTURE_POINT_OF;
		}
		else
		{
			ALERT( at_error, "Bot with unknown team %d\n", UTIL_GetTeam(this->pEdict) );

			return W_FL_DELETED;
		}
	}
	else
	{
		// fall back to regular deathmatch rules for deathmatch
		return bot_t::GetGoalType();
	}
}

bool OpposingForceBot::HasFlag()
{
	edict_t *pent = NULL;

	while( pent = UTIL_FindEntityByClassname( pent, "item_ctfflag" ) )
	{
		// if the bot has the flag and the flag is at the same place as this bot, it has the flag
		if( (pent->v.owner == this->pEdict) && (pent->v.origin == this->GetOrigin()) )
		{
			return true;
		}
	}

	return false;
}

bool OpposingForceBot::FindFlag()
{
	extern waypoint_t waypoints[MAX_WAYPOINTS];

	int skin = 0;
	int team = pGame->GetTeam( this->pEdict );
	float waypoint_distance = 0.0;
	float distance = 0.0;
	edict_t *pent = NULL;

	while ((pent = UTIL_FindEntityByClassname( pent, "item_ctfflag" )) != NULL)
	{
		// is this bot carrying the item? (after capture bug fix by Whistler)
		if ((pent->v.owner == pEdict) && (pent->v.origin == this->GetOrigin()))
		{
			// we are carrying the flag
			this->bBotHasFlag = true;

			break;  // break out of while loop
		}
		// if the flag is visible
		else if (FInViewCone( &pent->v.origin, pEdict ) && FVisible( pent->v.origin, pEdict))
		{
			// the bot can see it, check what type of model it is...
			skin = pent->v.skin;

			// is it the flag we should be protecting
			if ((skin == OpposingForceBot::BLACK_MESA_FLAG_SKIN && team == OpposingForceBot::TEAM_BLACK_MESA) ||
				(skin == OpposingForceBot::OPPOSING_FORCE_FLAG_SKIN && team == OpposingForceBot::TEAM_OPPOSING_FORCE))
			{
				// is and enemy carrying our flag/card?
				if (pent->v.owner != NULL)
				{
					// kill the man with the flag/card!
					this->pBotEnemy = pent->v.owner;

					this->waypoint_goal = WAYPOINT_NOT_FOUND;  // forget the goal (if there is one)

					return TRUE;
				}
			}
			// is it the flag we want to steal
			else
			{
				ALERT(at_console, "Seen flag to take\n");
				// check if someone is NOT carrying the flag/card...
				if (pent->v.owner == NULL)
				{
					// find the nearest waypoint to the flag/card...
					index = WaypointFindNearest(pEdict, 500, team, pent->v.origin, W_FL_FLAG);

					if (index == WAYPOINT_NOT_FOUND)
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

bool OpposingForceBot::ShouldCapturePoint( edict_t * pControlPoint )
{
	// TODO: work this out - there are 16 trigger_ctfgeneric entities in op4cp_park
	// there are 8 pairs - each pair is one per team
	// 8 to do with scoring (?), 8 to with displaying who owns the capture point (?) which have a triggerstate attribute too
	// those with a triggerstate attribute have a target that is an env_render which displays if that team has the point?
	// pev->skin or pev->body should have the team name but doesn't
	return false;
}

