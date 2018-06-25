#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

TFCBot::TFCBot()
{
}

void TFCBot::Join()
{
	extern int max_teams;
	extern int max_team_players[4];
	extern int team_class_limits[4];

	if ((this->start_action == MSG_TFC_IDLE) && (this->create_time + 3.0 <= gpGlobals->time))
	{
		this->start_action = MSG_TFC_TEAM_SELECT;  // force team selection
	}

	// handle Team Fortress Classic stuff here...

	if (this->start_action == MSG_TFC_TEAM_SELECT)
	{
		this->start_action = MSG_TFC_IDLE;  // switch back to idle
		this->create_time = gpGlobals->time;  // reset

		if ((this->bot_team != 1) && (this->bot_team != 2) && (this->bot_team != 3) && (this->bot_team != 4) && (this->bot_team != 5))
			this->bot_team = -1;

		if (this->bot_team == -1)
			this->bot_team = RANDOM_LONG(1, max_teams);

		int retry_count = 0;

		while ((retry_count < 4) && (max_team_players[this->bot_team-1] > 0))  // not unlimited?
		{
			int count = 0;

			// count number of players on this team...
			for (index = 1; index <= gpGlobals->maxClients; index++)
			{
				edict_t *pPlayer = INDEXENT(index);

				if (pPlayer && !pPlayer->free)
				{
					if (UTIL_GetTeam(pPlayer) == (this->bot_team - 1))
						count++;
				}
			}

			if (count < max_team_players[this->bot_team-1])
				break;  // haven't reached limit yet, continue
			else
			{
				this->bot_team++;

				if (this->bot_team > max_teams)
					this->bot_team = 1;

				retry_count++;
			}
		}

		// select the team the bot wishes to join...
		if( this->bot_team < 1 && this->bot_team > 5 )
			this->bot_team = 5;

		FakeClientCommand(pEdict, "jointeam %d", this->bot_team);

		return;
	}

	if (this->start_action == MSG_TFC_CLASS_SELECT)
	{
		this->start_action = MSG_TFC_IDLE;  // switch back to idle
		this->create_time = gpGlobals->time;  // reset

		if ((this->bot_class < 0) || (this->bot_class > 10))
			this->bot_class = -1;

		if (this->bot_class == -1)
			this->bot_class = RANDOM_LONG(1, 10);

		int team = UTIL_GetTeam(pEdict);

		if (team_class_limits[team] == -1)  // civilian only?
		{
			this->bot_class = 0;  // civilian
		}
		else
		{
			// TODO: this all looks unused - part of something that was half done?
			int class_not_allowed;

			if (this->bot_class == 10)
				class_not_allowed = team_class_limits[team] & (1<<7);
			else if (this->bot_class <= 7)
				class_not_allowed = team_class_limits[team] & (1<<(this->bot_class-1));
			else
				class_not_allowed = team_class_limits[team] & (1<<(this->bot_class));

			while (class_not_allowed)
			{
				this->bot_class = RANDOM_LONG(1, 10);

				if (this->bot_class == 10)
					class_not_allowed = team_class_limits[team] & (1<<7);
				else if (this->bot_class <= 7)
					class_not_allowed = team_class_limits[team] & (1<<(this->bot_class-1));
				else
					class_not_allowed = team_class_limits[team] & (1<<(this->bot_class));
			}
		}

		// select the class the bot wishes to use...
		if (this->bot_class == 0)
			FakeClientCommand(pEdict, "civilian");
		else if (this->bot_class == 1)
			FakeClientCommand(pEdict, "scout");
		else if (this->bot_class == 2)
			FakeClientCommand(pEdict, "sniper");
		else if (this->bot_class == 3)
			FakeClientCommand(pEdict, "soldier");
		else if (this->bot_class == 4)
			FakeClientCommand(pEdict, "demoman");
		else if (this->bot_class == 5)
			FakeClientCommand(pEdict, "medic");
		else if (this->bot_class == 6)
			FakeClientCommand(pEdict, "hwguy");
		else if (this->bot_class == 7)
			FakeClientCommand(pEdict, "pyro");
		else if (this->bot_class == 8)
			FakeClientCommand(pEdict, "spy");
		else if (this->bot_class == 9)
			FakeClientCommand(pEdict, "engineer");
		else
			FakeClientCommand(pEdict, "randompc");

		// bot has now joined the game (doesn't need to be started)
		this->not_started = 0;

		return;
	}
}

bool TFCBot::CanHeal()
{
	return pEdict->v.playerclass == TFCBot::CLASS_MEDIC;
}

bool TFCBot::IsValidEnemy( edict_t *pEnemy )
{
	bool bBaseIsValid = bot_t::IsValidEnemy( pEdict );
	bool bTFCIsValid = bBaseIsValid;

	extern int team_allies[4];

	if( team_allies[pGame->GetTeam( this->pEdict )] & (1 << pGame->GetTeam( pEnemy )) )
	{
		bTFCIsValid = false;
	}

	return bBaseIsValid && bTFCIsValid;
}

bool TFCBot::IsSniping()
{
	return this->charging_weapon_id == TF_WEAPON_SNIPERRIFLE && this->f_primary_charging > 0.0f;
}

int TFCBot::GetGoalType()
{
	if( this->IsSniper() )
	{
		if( RANDOM_LONG(1, 100) <= 10 )
		{
			return W_FL_FLAG;
		}
		else
		{
			return W_FL_SNIPER;
		}
	}
	else if( this->IsEngineer() )
	{
		if( RANDOM_LONG(1, 100) <= 20 )
		{
			return W_FL_FLAG;
		}
		else
		{
			return W_FL_TFC_BUILD_ZONE;
		}
	}
	else
	{
		return W_FL_FLAG;
	}
}

bool TFCBot::IsEngineer()
{
	return this->pEdict->v.playerclass == TFCBot::CLASS_ENGINEER;
}

bool TFCBot::IsSniper()
{
	return this->pEdict->v.playerclass == TFCBot::CLASS_SNIPER;
}

bool TFCBot::HasFlag()
{
	edict_t *pent = NULL;

	while( pent = UTIL_FindEntityByClassname( pent, "item_tfgoal" ) )
	{
		// if the bot has the flag then it has the flag
		if ( pent->v.owner == this->pEdict )
		{
			return true;
		}
	}

	return false;
}

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
			bBotHasFlag = true;
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

bool TFCBot::ShouldBuild()
{
	return this->f_engineer_build_time <= gpGlobals->time;
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
		{
			this->waypoint_goal = index;
		}
	}
}

bool TFCBot::CanBuildDispenser()
{
	return this->m_rgAmmo[weapon_defs[TF_WEAPON_SPANNER].iAmmo1] >= 100;
}

bool TFCBot::CanBuildSentryGun()
{
	return this->m_rgAmmo[weapon_defs[TF_WEAPON_SPANNER].iAmmo1] >= 130;
}

edict_t* TFCBot::FindEnemy()
{
	edict_t *pent = nullptr;
	edict_t *pNewEnemy = nullptr;
	float nearestdistance = 1000.0f;

	extern int team_allies[];

	if( this->CanHeal() )
	{
		pNewEnemy = this->FindEnemyToHeal();
	}

	if (pNewEnemy == nullptr)
	{
		while ((pent = UTIL_FindEntityByClassname( pent, "building_sentrygun" )) != NULL)
		{
			int sentry_team = -1;
			int bot_team = UTIL_GetTeam(this->pEdict);

			if (pent->v.colormap == TFCBot::TEAM_BLUE)
				sentry_team = 0;  // blue team's sentry
			else if (pent->v.colormap == TFCBot::TEAM_RED)
				sentry_team = 1;  // red team's sentry
			else if (pent->v.colormap == TFCBot::TEAM_YELLOW)
				sentry_team = 2;  // yellow team's sentry
			else if (pent->v.colormap == TFCBot::TEAM_GREEN)
				sentry_team = 3;  // green team's sentry

			// don't target your own team's sentry guns...
			if (bot_team == sentry_team)
				continue;

			// don't target your allie's sentry guns either...
			if (team_allies[bot_team] & (1<<sentry_team))
				continue;

			Vector vecEnd = pent->v.origin + pent->v.view_ofs;

			// is this sentry gun visible?
			if (FInViewCone( &vecEnd, this->pEdict ) && FVisible( vecEnd, this->pEdict ))
			{
				float distance = (pent->v.origin - this->pEdict->v.origin).Length();

				// is this the closest sentry gun?
				if (distance < nearestdistance)
				{
					nearestdistance = distance;
					pNewEnemy = pent;

					this->pBotUser = NULL;  // don't follow user when enemy found
				}
			}
		}
	}

	return pNewEnemy;
}
