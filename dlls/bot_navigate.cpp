//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_navigate.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "wpt.h"
#include "waypoint.h"

extern int mod_id;
extern waypoint_t waypoints[MAX_WAYPOINTS];
extern int num_waypoints;  // number of waypoints currently in use

float BotChangePitch( bot_t *pBot, float speed )
{
   edict_t *pEdict = pBot->pEdict;
   float ideal;
   float current;
   float current_180;  // current +/- 180 degrees
   float diff;

   // turn from the current v_angle pitch to the idealpitch by selecting
   // the quickest way to turn to face that direction
   
   current = pEdict->v.v_angle.x;

   ideal = pEdict->v.idealpitch;

   // find the difference in the current and ideal angle
   diff = abs(current - ideal);

   // check if the bot is already facing the idealpitch direction...
   if (diff <= 1)
      return diff;  // return number of degrees turned

   // check if difference is less than the max degrees per turn
   if (diff < speed)
      speed = diff;  // just need to turn a little bit (less than max)

   // here we have four cases, both angle positive, one positive and
   // the other negative, one negative and the other positive, or
   // both negative.  handle each case separately...

   if ((current >= 0) && (ideal >= 0))  // both positive
   {
      if (current > ideal)
         current -= speed;
      else
         current += speed;
   }
   else if ((current >= 0) && (ideal < 0))
   {
      current_180 = current - 180;

      if (current_180 > ideal)
         current += speed;
      else
         current -= speed;
   }
   else if ((current < 0) && (ideal >= 0))
   {
      current_180 = current + 180;
      if (current_180 > ideal)
         current += speed;
      else
         current -= speed;
   }
   else  // (current < 0) && (ideal < 0)  both negative
   {
      if (current > ideal)
         current -= speed;
      else
         current += speed;
   }

   // check for wrap around of angle...
   if (current > 180)
      current -= 360;
   if (current < -180)
      current += 360;

   pEdict->v.v_angle.x = current;

   return speed;  // return number of degrees turned
}


void BotFixIdealYaw(edict_t *pEdict)
{
   // check for wrap around of angle...
   if (pEdict->v.ideal_yaw >= 180)
      pEdict->v.ideal_yaw -= 360 * ((int) (pEdict->v.ideal_yaw / 360) + 1);

   if (pEdict->v.ideal_yaw < -180)
      pEdict->v.ideal_yaw += 360 * ((int) (-pEdict->v.ideal_yaw / 360) + 1);
}


float BotChangeYaw( bot_t *pBot, float speed )
{
   edict_t *pEdict = pBot->pEdict;
   float ideal;
   float current;
   float current_180;  // current +/- 180 degrees
   float diff;

   // turn from the current v_angle yaw to the ideal_yaw by selecting
   // the quickest way to turn to face that direction
   
   current = pEdict->v.v_angle.y;

   ideal = pEdict->v.ideal_yaw;

   // find the difference in the current and ideal angle
   diff = abs(current - ideal);

   // check if the bot is already facing the ideal_yaw direction...
   if (diff <= 1)
      return diff;  // return number of degrees turned

   // check if difference is less than the max degrees per turn
   if (diff < speed)
      speed = diff;  // just need to turn a little bit (less than max)

   // here we have four cases, both angle positive, one positive and
   // the other negative, one negative and the other positive, or
   // both negative.  handle each case separately...

   if ((current >= 0) && (ideal >= 0))  // both positive
   {
      if (current > ideal)
         current -= speed;
      else
         current += speed;
   }
   else if ((current >= 0) && (ideal < 0))
   {
      current_180 = current - 180;

      if (current_180 > ideal)
         current += speed;
      else
         current -= speed;
   }
   else if ((current < 0) && (ideal >= 0))
   {
      current_180 = current + 180;
      if (current_180 > ideal)
         current += speed;
      else
         current -= speed;
   }
   else  // (current < 0) && (ideal < 0)  both negative
   {
      if (current > ideal)
         current -= speed;
      else
         current += speed;
   }

   // check for wrap around of angle...
   if (current > 180)
      current -= 360;
   if (current < -180)
      current += 360;

   pEdict->v.v_angle.y = current;

   return speed;  // return number of degrees turned
}


void BotFixBodyAngles(edict_t *pEdict)
{
   // check for wrap around of angle...
   if (pEdict->v.angles.x >= 180)
      pEdict->v.angles.x -= 360 * ((int) (pEdict->v.angles.x / 360) + 1);

   if (pEdict->v.angles.x < -180)
      pEdict->v.angles.x += 360 * ((int) (-pEdict->v.angles.x / 360) + 1);

   if (pEdict->v.angles.y >= 180)
      pEdict->v.angles.y -= 360 * ((int) (pEdict->v.angles.y / 360) + 1);

   if (pEdict->v.angles.y < -180)
      pEdict->v.angles.y += 360 * ((int) (-pEdict->v.angles.y / 360) + 1);

   if (pEdict->v.angles.z >= 180)
      pEdict->v.angles.z -= 360 * ((int) (pEdict->v.angles.z / 360) + 1);

   if (pEdict->v.angles.z < -180)
      pEdict->v.angles.z += 360 * ((int) (-pEdict->v.angles.z / 360) + 1);
}


void BotFixViewAngles(edict_t *pEdict)
{
   // check for wrap around of angle...
   if (pEdict->v.v_angle.x >= 180)
      pEdict->v.v_angle.x -= 360 * ((int) (pEdict->v.v_angle.x / 360) + 1);

   if (pEdict->v.v_angle.x < -180)
      pEdict->v.v_angle.x += 360 * ((int) (-pEdict->v.v_angle.x / 360) + 1);

   if (pEdict->v.v_angle.y >= 180)
      pEdict->v.v_angle.y -= 360 * ((int) (pEdict->v.v_angle.y / 360) + 1);

   if (pEdict->v.v_angle.y < -180)
      pEdict->v.v_angle.y += 360 * ((int) (-pEdict->v.v_angle.y / 360) + 1);

   if (pEdict->v.v_angle.z >= 180)
      pEdict->v.v_angle.z -= 360 * ((int) (pEdict->v.v_angle.z / 360) + 1);

   if (pEdict->v.v_angle.z < -180)
      pEdict->v.v_angle.z += 360 * ((int) (-pEdict->v.v_angle.z / 360) + 1);
}


bool BotFindWaypoint( bot_t *pBot )
{
   int index, select_index;
   int team;
   path_t *pPath = NULL;
   int path_index;
   float distance, min_distance[3];
   int min_index[3];

   edict_t *pEdict = pBot->pEdict;

   team = UTIL_GetTeam(pEdict);

   for (index=0; index < 3; index++)
   {
      min_distance[index] = 9999.0;
      min_index[index] = -1;
   }

   index = WaypointFindPath(&pPath, &path_index, pBot->curr_waypoint_index, team);

   while (index != -1)
   {
      // if index is not a current or recent previous waypoint...
      if ((index != pBot->curr_waypoint_index) &&
          (index != pBot->prev_waypoint_index[0]) &&
          (index != pBot->prev_waypoint_index[1]) &&
          (index != pBot->prev_waypoint_index[2]) &&
          (index != pBot->prev_waypoint_index[3]) &&
          (index != pBot->prev_waypoint_index[4]))
      {
         // find the distance from the bot to this waypoint
         distance = (pEdict->v.origin - waypoints[index].origin).Length();

         if (distance < min_distance[0])
         {
            min_distance[2] = min_distance[1];
            min_index[2] = min_index[1];

            min_distance[1] = min_distance[0];
            min_index[1] = min_index[0];

            min_distance[0] = distance;
            min_index[0] = index;
         }
         else if (distance < min_distance [1])
         {
            min_distance[2] = min_distance[1];
            min_index[2] = min_index[1];

            min_distance[1] = distance;
            min_index[1] = index;
         }
         else if (distance < min_distance[2])
         {
            min_distance[2] = distance;
            min_index[2] = index;
         }
      }

      // find the next path to a waypoint
      index = WaypointFindPath(&pPath, &path_index, pBot->curr_waypoint_index, team);
   }

   select_index = -1;

   // about 20% of the time choose a waypoint at random
   // (don't do this any more often than every 10 seconds)

   if ((RANDOM_LONG(1, 100) <= 20) && (pBot->f_random_waypoint_time <= gpGlobals->time))
   {
      pBot->f_random_waypoint_time = gpGlobals->time + 10.0;

      if (min_index[2] != -1)
         index = RANDOM_LONG(0, 2);
      else if (min_index[1] != -1)
         index = RANDOM_LONG(0, 1);
      else if (min_index[0] != -1)
         index = 0;
      else
         return FALSE;  // no waypoints found!

      select_index = min_index[index];
   }
   else
   {
      // use the closest waypoint that has been recently used
      select_index = min_index[0];
   }

   if (select_index != -1)  // was a waypoint found?
   {
      pBot->prev_waypoint_index[4] = pBot->prev_waypoint_index[3];
      pBot->prev_waypoint_index[3] = pBot->prev_waypoint_index[2];
      pBot->prev_waypoint_index[2] = pBot->prev_waypoint_index[1];
      pBot->prev_waypoint_index[1] = pBot->prev_waypoint_index[0];
      pBot->prev_waypoint_index[0] = pBot->curr_waypoint_index;

      pBot->curr_waypoint_index = select_index;
      pBot->waypoint_origin = waypoints[select_index].origin;

      pBot->f_waypoint_time = gpGlobals->time;

      return TRUE;
   }

   return FALSE;  // couldn't find a waypoint
}

bool BotHeadTowardWaypoint( bot_t *pBot )
{
	int i;
	Vector v_src, v_dest;
	TraceResult tr;
	int index;
	bool status;
	float waypoint_distance, min_distance;
	int team;
	float pause_time = 0.0;
	edict_t *pent;
	bool touching;

	edict_t *pEdict = pBot->pEdict;

	// check if team play is enabled
	// TODO: make all waypoints valid for everyone?
	if (pGame->IsTeamPlay())
	{
		team = UTIL_GetTeam(pEdict);
	}
	else
	{
		// not team play (all waypoints are valid for everyone)
		team = -1;
	}

	// check if the bot has been trying to get to this waypoint for a while...
	if ((pBot->f_waypoint_time + 5.0) < gpGlobals->time)
	{
		pBot->curr_waypoint_index = -1;  // forget about this waypoint
		pBot->waypoint_goal = -1;  // also forget about a goal
	}

   // check if a goal item exists...
   if (mod_id == TFC_DLL)
   {
		bool success = ((TFCBot *)pBot)->FindFlag();

		if( success )
		{
			return true;
		}
   }
   else if ((mod_id == GEARBOX_DLL) && pGame->IsCTF())
   {
	   bool success = ((OpposingForceBot *)pBot)->FindFlag();

		if( success )
		{
			return true;
		}
   }
   else if( mod_id == NS_DLL )
   {
	   if( !pBot->HasEnemy() )
	   {
		   if( UTIL_GetTeam( pBot->pEdict ) == NSBot::TEAM_ALIEN )
		   {
			   pent = NULL;

			   while( (pent = UTIL_FindEntityByClassname( pent, "team_command" )) != NULL )
				{
					Vector vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this command chair visible?
					if (/*FInViewCone( &vecEnd, pEdict ) && */FVisible( vecEnd, pEdict ))
					{
						UTIL_LogDPrintf("seen a command chair\n" );
						float fdistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest command chair?
						if (fdistance < 300.0)
						{
							UTIL_LogDPrintf( "bot seeing a command chair; making it the enemy\n" );
							pBot->pBotEnemy = pent;
						}
					}
				}
		   }
		   else if( UTIL_GetTeam( pBot->pEdict ) == NSBot::TEAM_MARINE )
		   {
			   pent = NULL;

			   while( (pent = UTIL_FindEntityByClassname( pent, "team_hive" )) != NULL )
				{
					Vector vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this hive visible?
					if (FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) && pent->v.solid != SOLID_NOT)
					{
						float fdistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest hive?
						if (fdistance < 300.0)
						{
							UTIL_LogDPrintf( "bot seeing a hive; making it the enemy\n" );
							pBot->pBotEnemy = pent;
						}
					}
				}
		   }
	   }
   }

   // check if we need to find a waypoint...
   if (pBot->curr_waypoint_index == -1)
   {
      pBot->waypoint_top_of_ladder = FALSE;

      // did we just come off of a ladder or are we underwater?
      if (((pBot->f_end_use_ladder_time + 2.0) > gpGlobals->time) || (pBot->IsUnderWater()))
      {
         // find the nearest visible waypoint
         i = WaypointFindNearest(pEdict, REACHABLE_RANGE, team);
      }
      else
      {
         i = WaypointFindReachable(pEdict, REACHABLE_RANGE, team);
      }

      if (i == -1)
      {
         pBot->curr_waypoint_index = -1;
         return FALSE;
      }

      pBot->curr_waypoint_index = i;
      pBot->waypoint_origin = waypoints[i].origin;

      pBot->f_waypoint_time = gpGlobals->time;
   }
   // the bot has a waypoint
   else
   {
      // skip this part if bot is trying to get out of water...
      if (pBot->f_exit_water_time < gpGlobals->time)
      {
         // check if we can still see our target waypoint...

         v_src = pEdict->v.origin + pEdict->v.view_ofs;
         v_dest = waypoints[pBot->curr_waypoint_index].origin;

         // trace a line from bot's eyes to destination...
         UTIL_TraceLine( v_src, v_dest, ignore_monsters, pEdict->v.pContainingEntity, &tr );

         // check if line of sight to object is blocked (i.e. not visible)
         if (tr.flFraction < 1.0)
         {
            // did we just come off of a ladder or are we under water?
            if (((pBot->f_end_use_ladder_time + 2.0) > gpGlobals->time) || (pBot->IsUnderWater()))
            {
               // find the nearest visible waypoint
               i = WaypointFindNearest(pEdict, REACHABLE_RANGE, team);
            }
            else
            {
               // find the nearest reachable waypoint
               i = WaypointFindReachable(pEdict, REACHABLE_RANGE, team);
            }

            if (i == -1)
            {
               pBot->curr_waypoint_index = -1;
               return FALSE;
            }

            pBot->curr_waypoint_index = i;
            pBot->waypoint_origin = waypoints[i].origin;

            pBot->f_waypoint_time = gpGlobals->time;
         }
      }
   }

   // find the distance to the target waypoint
   waypoint_distance = (pEdict->v.origin - pBot->waypoint_origin).Length();

   // set the minimum distance from waypoint to be considered "touching" it
   min_distance = 50.0;

   // if this is a crouch waypoint, bot must be fairly close...
   if (waypoints[pBot->curr_waypoint_index].flags & W_FL_CROUCH)
      min_distance = 20.0;

   if (waypoints[pBot->curr_waypoint_index].flags & W_FL_JUMP)
      min_distance = 25.0;

   if (waypoints[pBot->curr_waypoint_index].flags & W_FL_TFC_SENTRYGUN)
      min_distance = 20.0;

   if (waypoints[pBot->curr_waypoint_index].flags & W_FL_TFC_DISPENSER)
      min_distance = 20.0;

   // if this is a ladder waypoint, bot must be fairly close to get on ladder
   if (waypoints[pBot->curr_waypoint_index].flags & W_FL_LADDER)
      min_distance = 20.0;

   // if trying to get out of water, need to get very close to waypoint...
   if (pBot->f_exit_water_time >= gpGlobals->time)
      min_distance = 20.0;

   touching = FALSE;

   // did the bot run past the waypoint? (prevent the loop-the-loop problem)
   if ((pBot->prev_waypoint_distance > 1.0) && (waypoint_distance > pBot->prev_waypoint_distance))
      touching = TRUE;

   // are we close enough to a target waypoint...
   if (waypoint_distance < min_distance)
      touching = TRUE;

   // save current distance as previous
   pBot->prev_waypoint_distance = waypoint_distance;

   // if the bot is on/at the waypoint
   if (touching)
   {
      bool waypoint_found = FALSE;

      pBot->prev_waypoint_distance = 0.0;

	  // if the current waypoint is the goal and it's a Day of Defeat capture point
	  if (mod_id == DOD_DLL && (waypoints[pBot->curr_waypoint_index].flags & W_FL_DOD_CAP) &&
		  pBot->waypoint_goal == pBot->curr_waypoint_index && !ShouldSkip(pBot->pEdict, pBot->waypoint_goal))
	  {
		  UTIL_LogDPrintf( "stopping near waypoint\n" );
		  pBot->SetSpeed( 0.0 );
		  ((DODBot *)pBot)->bCapturing = true;
		  pBot->iGoalIndex = pBot->waypoint_goal;
	  }
	  else if (mod_id == DOD_DLL && (waypoints[pBot->curr_waypoint_index].flags & W_FL_DOD_CAP) &&
		  pBot->waypoint_goal == pBot->curr_waypoint_index && ShouldSkip(pBot->pEdict, pBot->waypoint_goal))
	  {
		  UTIL_LogDPrintf( "moving away from waypoint\n" );
		  pBot->SetSpeed( pEdict->v.maxspeed );
		  ((DODBot *)pBot)->bCapturing = false;
	  }

      // check if the waypoint is a door waypoint
      if (waypoints[pBot->curr_waypoint_index].flags & W_FL_DOOR)
      {
         pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
      }

      // check if the next waypoint is a jump waypoint...
      if (waypoints[pBot->curr_waypoint_index].flags & W_FL_JUMP)
      {
         pEdict->v.button |= IN_JUMP;  // jump here
      }

      // check if the waypoint is a sniper waypoint...
      if (waypoints[pBot->curr_waypoint_index].flags & W_FL_SNIPER)
      {
         if (pBot->IsSniper())
         {
            int aim_index;

            aim_index = WaypointFindNearestAiming(waypoints[pBot->curr_waypoint_index].origin);

            if (aim_index != -1)
            {
               Vector v_aim = waypoints[aim_index].origin - waypoints[pBot->curr_waypoint_index].origin;

               Vector aim_angles = UTIL_VecToAngles( v_aim );

               pEdict->v.ideal_yaw = aim_angles.y;

               BotFixIdealYaw(pEdict);
            }

            pBot->f_pause_time = gpGlobals->time + RANDOM_FLOAT(20.0, 30.0);

            // fix f_waypoint_time so bot won't think it is stuck
            pBot->f_waypoint_time = pBot->f_pause_time;

            return TRUE;
         }
      }

      // check if the bot has reached the goal waypoint...
      if (pBot->curr_waypoint_index == pBot->waypoint_goal)
      {
		  UTIL_LogDPrintf( "bot is at goal waypoint %d\n", pBot->curr_waypoint_index );
         pBot->waypoint_goal = -1;  // forget this goal waypoint

         if (pBot->waypoint_near_flag)
         {
            pBot->waypoint_near_flag = FALSE;

            // just head towards the flag/card/ball
            Vector v_flag = pBot->waypoint_flag_origin - pEdict->v.origin;

            Vector bot_angles = UTIL_VecToAngles( v_flag );

            pEdict->v.ideal_yaw = bot_angles.y;

            BotFixIdealYaw(pEdict);

            return TRUE;
         }

		 if (mod_id == TFC_DLL)
		 {
			 // see if this waypoint is a sentry gun waypoint...
			 if ((waypoints[pBot->curr_waypoint_index].flags & W_FL_TFC_SENTRYGUN) && (pEdict->v.playerclass == TFCBot::CLASS_ENGINEER))
			 {
				if (((TFCBot *)pBot)->CanBuildSentryGun())
				{
				   int aim_index;

				   aim_index = WaypointFindNearestAiming(waypoints[pBot->curr_waypoint_index].origin);

				   if (aim_index != -1)
				   {
					  Vector v_aim = waypoints[aim_index].origin - waypoints[pBot->curr_waypoint_index].origin;

					  Vector aim_angles = UTIL_VecToAngles( v_aim );

					  pEdict->v.ideal_yaw = aim_angles.y;

					  BotFixIdealYaw(pEdict);

					  pBot->b_build_sentrygun = TRUE;

					  pBot->sentrygun_waypoint = pBot->curr_waypoint_index;

					  pBot->f_look_for_waypoint_time = gpGlobals->time + 5.0;

					  return TRUE;
				   }
				}
			 }

			 // see if this waypoint is a dispenser waypoint...
			 if ((waypoints[pBot->curr_waypoint_index].flags & W_FL_TFC_DISPENSER) && (pEdict->v.playerclass == TFCBot::CLASS_ENGINEER))
			 {
				// does bot have enough metal to build a dispenser?
				if (((TFCBot *)pBot)->CanBuildDispenser())
				{
				   int aim_index;

				   aim_index = WaypointFindNearestAiming(waypoints[pBot->curr_waypoint_index].origin);

				   if (aim_index != -1)
				   {
					  Vector v_aim = waypoints[aim_index].origin - waypoints[pBot->curr_waypoint_index].origin;

					  Vector aim_angles = UTIL_VecToAngles( v_aim );

					  pEdict->v.ideal_yaw = aim_angles.y;

					  BotFixIdealYaw(pEdict);

					  pBot->b_build_dispenser = TRUE;

					  pBot->dispenser_waypoint = pBot->curr_waypoint_index;

					  pBot->f_look_for_waypoint_time = gpGlobals->time + 5.0;

					  return TRUE;
				   }
				}
			 }
		 }
      }

      // check if the bot is carrying the flag/card/ball...
      if( pBot->HasFlag() )
      {
         // find the nearest flag goal waypoint...
         index = WaypointFindNearestGoal( pEdict, pBot->curr_waypoint_index, team, W_FL_FLAG_GOAL );

         pBot->waypoint_goal = index;  // goal index or -1

         pBot->waypoint_near_flag = FALSE;
      }
	  else
	  {
		  // find the nearest flag goal waypoint...
		  index = WaypointFindNearestGoal( pEdict, pBot->curr_waypoint_index, team, pBot->GetGoalType() );

		  pBot->waypoint_goal = index;  // goal index or -1
	  }

      // test special case of bot underwater and close to surface...
      if (pBot->IsUnderWater())
      {
         Vector v_src, v_dest;
         TraceResult tr;
         int contents;

         // trace a line straight up 100 units...
         v_src = pEdict->v.origin;
         v_dest = v_src;
         v_dest.z = v_dest.z + 100.0;

         // trace a line to destination...
         UTIL_TraceLine( v_src, v_dest, ignore_monsters, pEdict->v.pContainingEntity, &tr );

         if (tr.flFraction >= 1.0)
         {
            // find out what the contents is of the end of the trace...
            contents = POINT_CONTENTS( tr.vecEndPos );
   
            // check if the trace endpoint is in open space...
            if (contents == CONTENTS_EMPTY)
            {
               // find the nearest visible waypoint
               i = WaypointFindNearest(pEdict, 100, team, tr.vecEndPos);

               if (i != -1)
               {
                  waypoint_found = TRUE;
                  pBot->curr_waypoint_index = i;
                  pBot->waypoint_origin = waypoints[i].origin;

                  pBot->f_waypoint_time = gpGlobals->time;

                  // keep trying to exit water for next 3 seconds
                  pBot->f_exit_water_time = gpGlobals->time + 3.0;
               }
            }
         }
      }

      // if the bot doesn't have a goal waypoint then pick one...
      if ((pBot->waypoint_goal == -1) && (pBot->f_waypoint_goal_time < gpGlobals->time))
      {
         // don't pick a goal more often than every 10 seconds...
         pBot->f_waypoint_goal_time = gpGlobals->time + 10.0;

         pBot->waypoint_near_flag = FALSE;


// IF HEALTH LESS THAN CRITICAL LEVEL, GO FIND HEALTH!!!

// IF AMMO LESS THAN CRITICAL LEVEL, GO FIND AMMO!!!

// GO FIND WEAPONS HERE!!!


         if( pGame->IsDeathmatch() )
         {
            if (RANDOM_LONG(1, 100) <= 50)
            {
               index = WaypointFindNearestGoal(pEdict, pBot->curr_waypoint_index, team, W_FL_WEAPON, pBot->weapon_points);
            }
            else
            {
               int count = 0;

               index = -1;

               while ((index == -1) && (count < 3))
               {
                  index = WaypointFindRandomGoal(pEdict, team, W_FL_WEAPON, pBot->weapon_points);
                  count++;
               }
            }

            if (index != -1)
            {
               pBot->waypoint_goal = index;

               pBot->weapon_points[4] = pBot->weapon_points[3];
               pBot->weapon_points[3] = pBot->weapon_points[2];
               pBot->weapon_points[2] = pBot->weapon_points[1];
               pBot->weapon_points[1] = pBot->weapon_points[0];
               pBot->weapon_points[0] = pBot->waypoint_goal;
            }
         }
         else if (mod_id == TFC_DLL)
         {
            if (((TFCBot *)pBot)->IsEngineer() && ((TFCBot *)pBot)->ShouldBuild())
            {
				((TFCBot *)pBot)->Build();
            }
            else
            {
				index = WaypointFindNearestGoal(pEdict, pBot->curr_waypoint_index, team, pBot->GetGoalType());

				if (index != WAYPOINT_NOT_FOUND)
				{
					pBot->waypoint_goal = index;
				}
            }
         }
			else if( ( mod_id == GEARBOX_DLL && pGame->IsCTF() ) || mod_id == DOD_DLL || mod_id == NS_DLL )
			{
				index = WaypointFindNearestGoal(pEdict, pBot->curr_waypoint_index, team, pBot->GetGoalType());

				if( index != WAYPOINT_NOT_FOUND )
				{
					UTIL_LogDPrintf("a bot (team %d) is heading for a goal (wpt %d)\n", UTIL_GetTeam( pBot->pEdict ), index);
					pBot->waypoint_goal = index;
				}
			}
      }

      // check if the bot has a goal waypoint...
      if (pBot->waypoint_goal != -1)
      {
         // get the next waypoint to reach goal...
         i = WaypointRouteFromTo(pBot->curr_waypoint_index, pBot->waypoint_goal, team);

         if (i != WAYPOINT_UNREACHABLE)  // can we get to the goal from here?
         {
            waypoint_found = TRUE;
            pBot->curr_waypoint_index = i;
            pBot->waypoint_origin = waypoints[i].origin;
            pBot->f_waypoint_time = gpGlobals->time;
         }
		 else
		 {
			 UTIL_LogDPrintf("Bot %s unable to get route from waypoint %d to waypoint %d\n", STRING(pBot->pEdict->v.netname), pBot->curr_waypoint_index, pBot->waypoint_goal);
		 }
      }

      if (waypoint_found == FALSE)
      {
         index = 4;

         // try to find the next waypoint
         while (((status = BotFindWaypoint( pBot )) == FALSE) && (index > 0))
         {
            // if waypoint not found, clear oldest prevous index and try again

            pBot->prev_waypoint_index[index] = -1;
            index--;
         }

         if (status == FALSE)
         {
            pBot->curr_waypoint_index = -1;  // indicate no waypoint found

            // clear all previous waypoints...
            for (index=0; index < 5; index++)
               pBot->prev_waypoint_index[index] = -1;

            return FALSE;
         }
      }
   }

   // check if the next waypoint is on a ladder AND
   // the bot it not currently on a ladder...
   if ((waypoints[pBot->curr_waypoint_index].flags & W_FL_LADDER) && (pEdict->v.movetype != MOVETYPE_FLY))
   {
      // if it's origin is lower than the bot...
      if (waypoints[pBot->curr_waypoint_index].origin.z < pEdict->v.origin.z)
      {
         pBot->waypoint_top_of_ladder = TRUE;
      }
   }
   else
   {
      pBot->waypoint_top_of_ladder = FALSE;
   }

   // keep turning towards the waypoint...

   Vector v_direction = pBot->waypoint_origin - pEdict->v.origin;

   Vector v_angles = UTIL_VecToAngles(v_direction);

   // if the bot is NOT on a ladder, change the yaw...
   if (pEdict->v.movetype != MOVETYPE_FLY)
   {
      pEdict->v.idealpitch = -v_angles.x;
      pBot->FixIdealPitch();

      pEdict->v.ideal_yaw = v_angles.y;
      BotFixIdealYaw(pEdict);
   }

   return TRUE;
}


void BotOnLadder( bot_t *pBot, float moved_distance )
{
   Vector v_src, v_dest, view_angles;
   TraceResult tr;
   float angle = 0.0;
   bool done = FALSE;

   edict_t *pEdict = pBot->pEdict;

   // check if the bot has JUST touched this ladder...
   if (pBot->ladder_dir == LADDER_UNKNOWN)
   {
      // try to square up the bot on the ladder...
      while ((!done) && (angle < 180.0))
      {
         // try looking in one direction (forward + angle)
         view_angles = pEdict->v.v_angle;
         view_angles.y = pEdict->v.v_angle.y + angle;

         if (view_angles.y < 0.0)
            view_angles.y += 360.0;
         if (view_angles.y > 360.0)
            view_angles.y -= 360.0;

         UTIL_MakeVectors( view_angles );

         v_src = pEdict->v.origin + pEdict->v.view_ofs;
         v_dest = v_src + gpGlobals->v_forward * 30;

         UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

         if (tr.flFraction < 1.0)  // hit something?
         {
            if (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0)
            {
               // square up to the wall...
               view_angles = UTIL_VecToAngles(tr.vecPlaneNormal);

               // Normal comes OUT from wall, so flip it around...
               view_angles.y += 180;

               if (view_angles.y > 180)
                  view_angles.y -= 360;

               pEdict->v.ideal_yaw = view_angles.y;

               BotFixIdealYaw(pEdict);

               done = TRUE;
            }
         }
         else
         {
            // try looking in the other direction (forward - angle)
            view_angles = pEdict->v.v_angle;
            view_angles.y = pEdict->v.v_angle.y - angle;

            if (view_angles.y < 0.0)
               view_angles.y += 360.0;
            if (view_angles.y > 360.0)
               view_angles.y -= 360.0;

            UTIL_MakeVectors( view_angles );

            v_src = pEdict->v.origin + pEdict->v.view_ofs;
            v_dest = v_src + gpGlobals->v_forward * 30;

            UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

            if (tr.flFraction < 1.0)  // hit something?
            {
               if (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0)
               {
                  // square up to the wall...
                  view_angles = UTIL_VecToAngles(tr.vecPlaneNormal);

                  // Normal comes OUT from wall, so flip it around...
                  view_angles.y += 180;

                  if (view_angles.y > 180)
                     view_angles.y -= 360;

                  pEdict->v.ideal_yaw = view_angles.y;

                  BotFixIdealYaw(pEdict);

                  done = TRUE;
               }
            }
         }

         angle += 10;
      }

      if (!done)  // if didn't find a wall, just reset ideal_yaw...
      {
         // set ideal_yaw to current yaw (so bot won't keep turning)
         pEdict->v.ideal_yaw = pEdict->v.v_angle.y;

         BotFixIdealYaw(pEdict);
      }
   }

   // moves the bot up or down a ladder.  if the bot can't move
   // (i.e. get's stuck with someone else on ladder), the bot will
   // change directions and go the other way on the ladder.

   if (pBot->ladder_dir == LADDER_UP)  // is the bot currently going up?
   {
      pEdict->v.v_angle.x = -60;  // look upwards

      // check if the bot hasn't moved much since the last location...
      if ((moved_distance <= 1) && (pBot->prev_speed >= 1.0))
      {
         // the bot must be stuck, change directions...

         pEdict->v.v_angle.x = 60;  // look downwards
         pBot->ladder_dir = LADDER_DOWN;
      }
   }
   else if (pBot->ladder_dir == LADDER_DOWN)  // is the bot currently going down?
   {
      pEdict->v.v_angle.x = 60;  // look downwards

      // check if the bot hasn't moved much since the last location...
      if ((moved_distance <= 1) && (pBot->prev_speed >= 1.0))
      {
         // the bot must be stuck, change directions...

         pEdict->v.v_angle.x = -60;  // look upwards
         pBot->ladder_dir = LADDER_UP;
      }
   }
   else  // the bot hasn't picked a direction yet, try going up...
   {
      pEdict->v.v_angle.x = -60;  // look upwards
      pBot->ladder_dir = LADDER_UP;
   }

   // move forward (i.e. in the direction the bot is looking, up or down)
   pEdict->v.button |= IN_FORWARD;
}


void BotUnderWater( bot_t *pBot )
{
   bool found_waypoint = FALSE;

   edict_t *pEdict = pBot->pEdict;

   // are there waypoints in this level (and not trying to exit water)?
   if ((num_waypoints > 0) && (pBot->f_exit_water_time < gpGlobals->time))
   {
      // head towards a waypoint
      found_waypoint = BotHeadTowardWaypoint(pBot);
   }

   if (found_waypoint == FALSE)
   {
      // handle movements under water.  right now, just try to keep from
      // drowning by swimming up towards the surface and look to see if
      // there is a surface the bot can jump up onto to get out of the
      // water.  bots DON'T like water!

      Vector v_src, v_forward;
      TraceResult tr;
      int contents;
   
      // swim up towards the surface
      pEdict->v.v_angle.x = -60;  // look upwards
   
      // move forward (i.e. in the direction the bot is looking, up or down)
      pEdict->v.button |= IN_FORWARD;
   
      // set gpGlobals angles based on current view angle (for TraceLine)
      UTIL_MakeVectors( pEdict->v.v_angle );
   
      // look from eye position straight forward (remember: the bot is looking
      // upwards at a 60 degree angle so TraceLine will go out and up...
   
      v_src = pEdict->v.origin + pEdict->v.view_ofs;  // EyePosition()
      v_forward = v_src + gpGlobals->v_forward * 90;
   
      // trace from the bot's eyes straight forward...
      UTIL_TraceLine( v_src, v_forward, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);
   
      // check if the trace didn't hit anything (i.e. nothing in the way)...
      if (tr.flFraction >= 1.0)
      {
         // find out what the contents is of the end of the trace...
         contents = UTIL_PointContents( tr.vecEndPos );
   
         // check if the trace endpoint is in open space...
         if (contents == CONTENTS_EMPTY)
         {
            // ok so far, we are at the surface of the water, continue...
   
            v_src = tr.vecEndPos;
            v_forward = v_src;
            v_forward.z -= 90;
   
            // trace from the previous end point straight down...
            UTIL_TraceLine( v_src, v_forward, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);
   
            // check if the trace hit something...
            if (tr.flFraction < 1.0)
            {
               contents = UTIL_PointContents( tr.vecEndPos );
   
               // if contents isn't water then assume it's land, jump!
               if (contents != CONTENTS_WATER)
               {
                  pEdict->v.button |= IN_JUMP;
               }
            }
         }
      }
   }
}


void BotUseLift( bot_t *pBot, float moved_distance )
{
   edict_t *pEdict = pBot->pEdict;

   // just need to press the button once, when the flag gets set...
   if (pBot->f_use_button_time == gpGlobals->time)
   {
      pEdict->v.button = IN_USE;

      // face opposite from the button
      pEdict->v.ideal_yaw += 180;  // rotate 180 degrees

      BotFixIdealYaw(pEdict);
   }

   // check if the bot has waited too long for the lift to move...
   if (((pBot->f_use_button_time + 2.0) < gpGlobals->time) && !pBot->b_lift_moving)
   {
      // clear use button flag
      pBot->b_use_button = FALSE;

      // bot doesn't have to set f_find_item since the bot
      // should already be facing away from the button

      pBot->SetSpeed( pBot->GetMaxSpeed() );
   }

   // check if lift has started moving...
   if ((moved_distance > 1) && !pBot->b_lift_moving)
   {
      pBot->b_lift_moving = TRUE;
   }

   // check if lift has stopped moving...
   if ((moved_distance <= 1) && pBot->b_lift_moving)
   {
      TraceResult tr1, tr2;
      Vector v_src, v_forward, v_right, v_left;
      Vector v_down, v_forward_down, v_right_down, v_left_down;

      pBot->b_use_button = FALSE;

      // TraceLines in 4 directions to find which way to go...

      UTIL_MakeVectors( pEdict->v.v_angle );

      v_src = pEdict->v.origin + pEdict->v.view_ofs;
      v_forward = v_src + gpGlobals->v_forward * 90;
      v_right = v_src + gpGlobals->v_right * 90;
      v_left = v_src + gpGlobals->v_right * -90;

      v_down = pEdict->v.v_angle;
      v_down.x = v_down.x + 45;  // look down at 45 degree angle

      UTIL_MakeVectors( v_down );

      v_forward_down = v_src + gpGlobals->v_forward * 100;
      v_right_down = v_src + gpGlobals->v_right * 100;
      v_left_down = v_src + gpGlobals->v_right * -100;

      // try tracing forward first...
      UTIL_TraceLine( v_src, v_forward, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr1);
      UTIL_TraceLine( v_src, v_forward_down, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr2);

      // check if we hit a wall or didn't find a floor...
      if ((tr1.flFraction < 1.0) || (tr2.flFraction >= 1.0))
      {
         // try tracing to the RIGHT side next...
         UTIL_TraceLine( v_src, v_right, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr1);
         UTIL_TraceLine( v_src, v_right_down, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr2);

         // check if we hit a wall or didn't find a floor...
         if ((tr1.flFraction < 1.0) || (tr2.flFraction >= 1.0))
         {
            // try tracing to the LEFT side next...
            UTIL_TraceLine( v_src, v_left, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr1);
            UTIL_TraceLine( v_src, v_left_down, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr2);

            // check if we hit a wall or didn't find a floor...
            if ((tr1.flFraction < 1.0) || (tr2.flFraction >= 1.0))
            {
               // only thing to do is turn around...
               pEdict->v.ideal_yaw += 180;  // turn all the way around
            }
            else
            {
               pEdict->v.ideal_yaw += 90;  // turn to the LEFT
            }
         }
         else
         {
            pEdict->v.ideal_yaw -= 90;  // turn to the RIGHT
         }

         BotFixIdealYaw(pEdict);
      }

      BotChangeYaw( pBot, pEdict->v.yaw_speed );

      pBot->SetSpeed( pBot->GetMaxSpeed() );
   }
}


bool BotStuckInCorner( bot_t *pBot )
{
	TraceResult tr;
	Vector v_src, v_dest;
	edict_t *pEdict = pBot->pEdict;

	UTIL_MakeVectors( pEdict->v.v_angle );

	// trace 45 degrees to the right...
	v_src = pEdict->v.origin;
	v_dest = v_src + gpGlobals->v_forward*20 + gpGlobals->v_right*20;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

	if (tr.flFraction >= 1.0)
	{
		return false;  // no wall, so not in a corner
	}

	// trace 45 degrees to the left...
	v_src = pEdict->v.origin;
	v_dest = v_src + gpGlobals->v_forward*20 - gpGlobals->v_right*20;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

	if (tr.flFraction >= 1.0)
	{
		return false;  // no wall, so not in a corner
	}

	return true;  // bot is in a corner
}


void BotTurnAtWall( bot_t *pBot, TraceResult *tr )
{
   edict_t *pEdict = pBot->pEdict;
   Vector Normal;
   float Y, Y1, Y2, D1, D2, Z;

   // Find the normal vector from the trace result.  The normal vector will
   // be a vector that is perpendicular to the surface from the TraceResult.

   Normal = UTIL_VecToAngles(tr->vecPlaneNormal);

   // Since the bot keeps it's view angle in -180 < x < 180 degrees format,
   // and since TraceResults are 0 < x < 360, we convert the bot's view
   // angle (yaw) to the same format at TraceResult.

   Y = pEdict->v.v_angle.y;
   Y = Y + 180;
   if (Y > 359)
		Y -= 360;

   // Turn the normal vector around 180 degrees (i.e. make it point towards
   // the wall not away from it.  That makes finding the angles that the
   // bot needs to turn a little easier.

   Normal.y = Normal.y - 180;
   if (Normal.y < 0)
		Normal.y += 360;

   // Here we compare the bots view angle (Y) to the Normal - 90 degrees (Y1)
   // and the Normal + 90 degrees (Y2).  These two angles (Y1 & Y2) represent
   // angles that are parallel to the wall surface, but heading in opposite
   // directions.  We want the bot to choose the one that will require the
   // least amount of turning (saves time) and have the bot head off in that
   // direction.

   Y1 = Normal.y - 90;
   if (RANDOM_LONG(1, 100) <= 50)
   {
      Y1 = Y1 - RANDOM_FLOAT(5.0, 20.0);
   }
   if (Y1 < 0)
	   Y1 += 360;

   Y2 = Normal.y + 90;
   if (RANDOM_LONG(1, 100) <= 50)
   {
      Y2 = Y2 + RANDOM_FLOAT(5.0, 20.0);
   }
   if (Y2 > 359)
	   Y2 -= 360;

   // D1 and D2 are the difference (in degrees) between the bot's current
   // angle and Y1 or Y2 (respectively).

   D1 = abs(Y - Y1);
   if (D1 > 179)
	   D1 = abs(D1 - 360);
   D2 = abs(Y - Y2);
   if (D2 > 179)
	   D2 = abs(D2 - 360);

   // If difference 1 (D1) is more than difference 2 (D2) then the bot will
   // have to turn LESS if it heads in direction Y1 otherwise, head in
   // direction Y2.  I know this seems backwards, but try some sample angles
   // out on some graph paper and go through these equations using a
   // calculator, you'll see what I mean.

   if (D1 > D2)
      Z = Y1;
   else
      Z = Y2;

   // convert from TraceResult 0 to 360 degree format back to bot's
   // -180 to 180 degree format.

   if (Z > 180)
      Z -= 360;

   // set the direction to head off into...
   pEdict->v.ideal_yaw = Z;

   BotFixIdealYaw(pEdict);
}


bool BotCantMoveForward( bot_t *pBot, TraceResult *tr )
{
   edict_t *pEdict = pBot->pEdict;

   // use some TraceLines to determine if anything is blocking the current
   // path of the bot.

   Vector v_src, v_forward;

   UTIL_MakeVectors( pEdict->v.v_angle );

   // first do a trace from the bot's eyes forward...

   v_src = pEdict->v.origin + pEdict->v.view_ofs;  // EyePosition()
   v_forward = v_src + gpGlobals->v_forward * 40;

   // trace from the bot's eyes straight forward...
   UTIL_TraceLine( v_src, v_forward, dont_ignore_monsters, pEdict->v.pContainingEntity, tr );

   // check if the trace hit something...
   if (tr->flFraction < 1.0)
   {
      return true;  // bot's head will hit something
   }

   // bot's head is clear, check at waist level...

   v_src = pEdict->v.origin;
   v_forward = v_src + gpGlobals->v_forward * 40;

   // trace from the bot's waist straight forward...
   UTIL_TraceLine( v_src, v_forward, dont_ignore_monsters, pEdict->v.pContainingEntity, tr );

   // check if the trace hit something...
   if (tr->flFraction < 1.0)
   {
      return true;  // bot's body will hit something
   }

   return false;  // bot can move forward, return false
}


bool BotCanJumpUp( bot_t *pBot )
{
   // What I do here is trace 3 lines straight out, one unit higher than
   // the highest normal jumping distance.  I trace once at the center of
   // the body, once at the right side, and once at the left side.  If all
   // three of these TraceLines don't hit an obstruction then I know the
   // area to jump to is clear.  I then need to trace from head level,
   // above where the bot will jump to, downward to see if there is anything
   // blocking the jump.  There could be a narrow opening that the body
   // will not fit into.  These horizontal and vertical TraceLines seem
   // to catch most of the problems with falsely trying to jump on something
   // that the bot can not get onto.

   TraceResult tr;
   Vector v_jump, v_source, v_dest;
   edict_t *pEdict = pBot->pEdict;

   // convert current view angle to vectors for TraceLine math...

   v_jump = pEdict->v.v_angle;
   v_jump.x = 0;  // reset pitch to 0 (level horizontally)
   v_jump.z = 0;  // reset roll to 0 (straight up and down)

   UTIL_MakeVectors( v_jump );

   // use center of the body first...

   // maximum jump height is 45, so check one unit above that (46)
   v_source = pEdict->v.origin + Vector(0, 0, -36 + 46);
   v_dest = v_source + gpGlobals->v_forward * 24;

   // trace a line forward at maximum jump height...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now check same height to one side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * 16 + Vector(0, 0, -36 + 46);
   v_dest = v_source + gpGlobals->v_forward * 24;

   // trace a line forward at maximum jump height...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now check same height on the other side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * -16 + Vector(0, 0, -36 + 46);
   v_dest = v_source + gpGlobals->v_forward * 24;

   // trace a line forward at maximum jump height...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now trace from head level downward to check for obstructions...

   // start of trace is 24 units in front of bot, 72 units above head...
   v_source = pEdict->v.origin + gpGlobals->v_forward * 24;

   // offset 72 units from top of head (72 + 36)
   v_source.z = v_source.z + 108;

   // end point of trace is 99 units straight down from start...
   // (99 is 108 minus the jump limit height which is 45 - 36 = 9)
   v_dest = v_source + Vector(0, 0, -99);

   // trace a line straight down toward the ground...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now check same height to one side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * 16 + gpGlobals->v_forward * 24;
   v_source.z = v_source.z + 108;
   v_dest = v_source + Vector(0, 0, -99);

   // trace a line straight down toward the ground...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now check same height on the other side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * -16 + gpGlobals->v_forward * 24;
   v_source.z = v_source.z + 108;
   v_dest = v_source + Vector(0, 0, -99);

   // trace a line straight down toward the ground...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   return true;
}


bool BotCanDuckUnder( bot_t *pBot )
{
   // What I do here is trace 3 lines straight out, one unit higher than
   // the ducking height.  I trace once at the center of the body, once
   // at the right side, and once at the left side.  If all three of these
   // TraceLines don't hit an obstruction then I know the area to duck to
   // is clear.  I then need to trace from the ground up, 72 units, to make
   // sure that there is something blocking the TraceLine.  Then we know
   // we can duck under it.

   TraceResult tr;
   Vector v_duck, v_source, v_dest;
   edict_t *pEdict = pBot->pEdict;

   // convert current view angle to vectors for TraceLine math...

   v_duck = pEdict->v.v_angle;
   v_duck.x = 0;  // reset pitch to 0 (level horizontally)
   v_duck.z = 0;  // reset roll to 0 (straight up and down)

   UTIL_MakeVectors( v_duck );

   // use center of the body first...

   // duck height is 36, so check one unit above that (37)
   v_source = pEdict->v.origin + Vector(0, 0, -36 + 37);
   v_dest = v_source + gpGlobals->v_forward * 24;

   // trace a line forward at duck height...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now check same height to one side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * 16 + Vector(0, 0, -36 + 37);
   v_dest = v_source + gpGlobals->v_forward * 24;

   // trace a line forward at duck height...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters,
                   pEdict->v.pContainingEntity, &tr);

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now check same height on the other side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * -16 + Vector(0, 0, -36 + 37);
   v_dest = v_source + gpGlobals->v_forward * 24;

   // trace a line forward at duck height...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace hit something, return FALSE
   if (tr.flFraction < 1.0)
      return false;

   // now trace from the ground up to check for object to duck under...

   // start of trace is 24 units in front of bot near ground...
   v_source = pEdict->v.origin + gpGlobals->v_forward * 24;
   v_source.z = v_source.z - 35;  // offset to feet + 1 unit up

   // end point of trace is 72 units straight up from start...
   v_dest = v_source + Vector(0, 0, 72);

   // trace a line straight up in the air...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace didn't hit something, return FALSE
   if (tr.flFraction >= 1.0)
      return false;

   // now check same height to one side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * 16 + gpGlobals->v_forward * 24;
   v_source.z = v_source.z - 35;  // offset to feet + 1 unit up
   v_dest = v_source + Vector(0, 0, 72);

   // trace a line straight up in the air...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace didn't hit something, return FALSE
   if (tr.flFraction >= 1.0)
      return false;

   // now check same height on the other side of the bot...
   v_source = pEdict->v.origin + gpGlobals->v_right * -16 + gpGlobals->v_forward * 24;
   v_source.z = v_source.z - 35;  // offset to feet + 1 unit up
   v_dest = v_source + Vector(0, 0, 72);

   // trace a line straight up in the air...
   UTIL_TraceLine( v_source, v_dest, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // if trace didn't hit something, return FALSE
   if (tr.flFraction >= 1.0)
      return false;

   return true;
}


void BotRandomTurn( bot_t *pBot )
{
   pBot->SetSpeed( 0.0 );  // don't move while turning
            
   if (RANDOM_LONG(1, 100) <= 10)
   {
      // 10 percent of the time turn completely around...
      pBot->pEdict->v.ideal_yaw += 180;
   }
   else
   {
      // turn randomly between 30 and 60 degress
      if (pBot->wander_dir == WANDER_LEFT)
         pBot->pEdict->v.ideal_yaw += RANDOM_LONG(30, 60);
      else
         pBot->pEdict->v.ideal_yaw -= RANDOM_LONG(30, 60);
   }
            
   BotFixIdealYaw(pBot->pEdict);
}


bool BotFollowUser( bot_t *pBot )
{
   bool user_visible;
   float f_distance;
   edict_t *pEdict = pBot->pEdict;

   Vector vecEnd = pBot->pBotUser->v.origin + pBot->pBotUser->v.view_ofs;

   if (!pBot->IsUnderWater())  // is bot NOT under water?
      pEdict->v.v_angle.x = 0;  // reset pitch to 0 (level horizontally)

   pEdict->v.v_angle.z = 0;  // reset roll to 0 (straight up and down)

   pEdict->v.angles.x = 0;
   pEdict->v.angles.y = pEdict->v.v_angle.y;
   pEdict->v.angles.z = 0;

   if (!IsAlive( pBot->pBotUser ))
   {
      // the bot's user is dead!
      pBot->pBotUser = NULL;
      return FALSE;
   }

   user_visible = FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict );

   // check if the "user" is still visible or if the user has been visible
   // in the last 5 seconds (or the player just starting "using" the bot)

   if (user_visible || (pBot->f_bot_use_time + 5 > gpGlobals->time))
   {
      if (user_visible)
         pBot->f_bot_use_time = gpGlobals->time;  // reset "last visible time"

      // face the user
      Vector v_user = pBot->pBotUser->v.origin - pEdict->v.origin;
      Vector bot_angles = UTIL_VecToAngles( v_user );

      pEdict->v.ideal_yaw = bot_angles.y;

      BotFixIdealYaw(pEdict);

      f_distance = v_user.Length( );  // how far away is the "user"?

      if (f_distance > 200.0)      // run if distance to enemy is far
         pBot->SetSpeed( pBot->GetMaxSpeed() );
      else if (f_distance > 50.0)  // walk if distance is closer
         pBot->SetSpeed( pBot->GetMaxSpeed() / 2.0 );
      else                     // don't move if close enough
         pBot->SetSpeed( 0.0 );

      return TRUE;
   }
   else
   {
      // person to follow has gone out of sight...
      pBot->pBotUser = NULL;

      return FALSE;
   }
}


bool BotCheckWallOnLeft( bot_t *pBot )
{
   edict_t *pEdict = pBot->pEdict;
   Vector v_src, v_left;
   TraceResult tr;

   UTIL_MakeVectors( pEdict->v.v_angle );

   // do a trace to the left...

   v_src = pEdict->v.origin;
   v_left = v_src + gpGlobals->v_right * -40;  // 40 units to the left

   UTIL_TraceLine( v_src, v_left, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // check if the trace hit something...
   if (tr.flFraction < 1.0)
   {
      if (pBot->f_wall_on_left < 1.0)
         pBot->f_wall_on_left = gpGlobals->time;

      return TRUE;
   }

   return FALSE;
}


bool BotCheckWallOnRight( bot_t *pBot )
{
   edict_t *pEdict = pBot->pEdict;
   Vector v_src, v_right;
   TraceResult tr;

   UTIL_MakeVectors( pEdict->v.v_angle );

   // do a trace to the right...

   v_src = pEdict->v.origin;
   v_right = v_src + gpGlobals->v_right * 40;  // 40 units to the right

   UTIL_TraceLine( v_src, v_right, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

   // check if the trace hit something...
   if (tr.flFraction < 1.0)
   {
      if (pBot->f_wall_on_right < 1.0)
         pBot->f_wall_on_right = gpGlobals->time;

      return TRUE;
   }

   return FALSE;
}

void BotUseDoor( bot_t *pBot )
{
	edict_t *pEdict = pBot->pEdict;

	// just need to use the door once, when the flag gets set...
	if( pBot->fUseDoorTime == gpGlobals->time )
	{
		pEdict->v.button = IN_USE;

		// face opposite from the button
		// pEdict->v.ideal_yaw += 180.0;  // rotate 180 degrees

		BotFixIdealYaw( pEdict );
	}

	pBot->bUseDoor = false;

	pBot->SetSpeed( pBot->GetMaxSpeed() );
}
