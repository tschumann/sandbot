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
	// there are 16 trigger_ctfgeneric entities in op4cp_park (8 pairs of entities, 4 pairs per team)
	// 8 are to do with scoring (?), 8 are to with displaying who owns the capture point (?) - these have a triggerstate attribute
	// those with a triggerstate attribute have a target that is an env_render which displays if that team has the point?
	// pev->skin or pev->body should have the team name but doesn't - only one type might have this?

	// what happens - the trigger_ctfgeneric with a triggerstate attribute controls the rendering of who owns the capture point
	// it will target (among other things) two env_render entities - their target names will contain _bm_ or _op_ and a renderamt of 255 to show or renderamt of 0 to hide
	// if the _bm_ env_render has renderamt of 255 then it probably means that Black Mesa owns that entity

	edict_t* pEntity = nullptr;

	// look at each entity that is targeted by this trigger_ctfgeneric
	while( (pEntity = UTIL_FindEntityByString( pEntity, "targetname", STRING(pControlPoint->v.target) ) ) != nullptr )
	{
		if( !strcmp( STRING(pEntity->v.classname), "env_render" ) )
		{
			ALERT( at_console, "Found an env_render with name %s for %s\n", STRING(pEntity->v.globalname), STRING(pControlPoint->v.globalname) );

			// if the entity is visible and it's Black Mesa
			if( pEntity->v.renderamt == 255 && strstr( STRING(pEntity->v.target), "_bm_" ) )
			{
				// capture it if this is an Opposing Force player
				return pGame->GetTeam( this->pEdict ) == OpposingForceBot::TEAM_OPPOSING_FORCE;
			}
			// if the entity is visible and it's Opposing Force
			else if( pEntity->v.renderamt == 255 && strstr( STRING(pEntity->v.target), "_op_" ) )
			{
				// capture it if this is a Black Mesa player
				return pGame->GetTeam( this->pEdict ) == OpposingForceBot::TEAM_BLACK_MESA;
			}
			// if the entity is invisible
			else if( pEntity->v.renderamt == 0 )
			{
				// TODO: does this mean no one has captured it yet?
				return true;
			}
			else
			{
				ALERT( at_error, "Cannot work out whether to capture a trigger_ctfgeneric or not: renderamt is %d and target is %s\n", pEntity->v.renderamt, STRING(pEntity->v.target) );
			}
		}
	}

	return false;
}

