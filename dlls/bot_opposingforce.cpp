//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Half-Life Opposing Force specific bot code
//
//=============================================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "game.h"
#include "wpt.h"
#include "waypoint.h"

OpposingForceBot::OpposingForceBot()
{
}

void OpposingForceBot::Join()
{
	if( pGame->IsCTF() || pGame->IsCapturePoint() )
	{
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

bool OpposingForceBot::ShouldSeekEnemy() const
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
		return W_FL_OP4_CAPTURE_POINT;
	}
	else
	{
		// fall back to regular deathmatch rules for deathmatch
		return bot_t::GetGoalType();
	}
}

bool OpposingForceBot::HasFlag() const
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
	// op4cp_ maps are a little confusing
	// there are 16 trigger_ctfgeneric entities in op4cp_park (8 pairs of entities, 4 pairs per team)
	// 8 are to do with scoring (?), 8 are to do with displaying who owns the capture point (?) - these have a triggerstate attribute
	// those with a triggerstate attribute have a target that points to two env_render entities - these target func_walls entities and make them visible/invisible
	// using the above, it should be possible to figure out which capture teams are owned by who
	// that said, on op4cp_park nat least, it seems like points are captured by being near a capture point for 30 or so seconds, not for actually capturing the point
	// so bots could seek to capture all opposing team's points, but it's easiest to just try and capture and hold the nearest point
	// this can be expanded on in future when bots in team games have some sort of role (e.g. attack vs protect vs support) so attack bots can capture anything while protect/support can stay with already held ones
	// see commit ce3bc6dd0412e1a54259fe740c04c58deb59315b for not quite working but close to what it should be logic

	return true;
}

