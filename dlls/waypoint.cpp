// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// waypoint.cpp
//

#ifndef __linux__
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "game.h"
#include "wpt.h"
#include "waypoint.h"

extern int m_spriteTexture;

extern int num_backpacks;
extern BACKPACK_S backpacks[TFCGame::MAX_BACKPACKS];

// waypoints with information bits (flags)
waypoint_t waypoints[MAX_WAYPOINTS];

// number of waypoints currently in use
int num_waypoints;

// declare the array of head pointers to the path structures...
path_t *paths[MAX_WAYPOINTS];

// time that this waypoint was displayed (while editing)
float wp_display_time[MAX_WAYPOINTS];

bool g_waypoint_paths = false;  // have any paths been allocated?
bool g_waypoint_on = false;
bool g_auto_waypoint = false;
bool g_path_waypoint_enable = true;
Vector last_waypoint;
float f_path_time = 0.0;

const unsigned int SHORTEST_PATH_ARRAY_LENGTH = 4;

unsigned int route_num_waypoints;
unsigned short *shortest_path[SHORTEST_PATH_ARRAY_LENGTH] = {NULL, NULL, NULL, NULL};
unsigned short *from_to[SHORTEST_PATH_ARRAY_LENGTH] = {NULL, NULL, NULL, NULL};

void WaypointDebug(void)
{
	int x = 1;
	int y = 1;

	FILE *fp = fopen( "HPB_bot.txt", "a" );
	fprintf( fp,"WaypointDebug: LINKED LIST ERROR!!!\n" );
	fclose( fp );

	// cause a divide by zero exception
	x = x - 1;
	y = y / x;

	return;
}


// free the linked list of waypoint path nodes...
void WaypointFree(void)
{
   for (int i=0; i < MAX_WAYPOINTS; i++)
   {
      int count = 0;

      if (paths[i])
      {
         path_t *p = paths[i];
         path_t *p_next;

         while (p)  // free the linked list
         {
            p_next = p->next;  // save the link to next
            free(p);
            p = p_next;

#ifdef _DEBUG
            count++;
            if (count > 1000) WaypointDebug();
#endif
         }

         paths[i] = NULL;
      }
   }
}


// initialize the waypoint structures...
void WaypointInit()
{
	// have any waypoint path nodes been allocated yet?
   if (g_waypoint_paths)
      WaypointFree();  // must free previously allocated path memory

   for (unsigned int i=0; i < SHORTEST_PATH_ARRAY_LENGTH; i++)
   {
      if (shortest_path[i] != NULL)
         free(shortest_path[i]);

      if (from_to[i] != NULL)
         free(from_to[i]);
   }

   for (unsigned int i=0; i < MAX_WAYPOINTS; i++)
   {
      waypoints[i].flags = 0;
      waypoints[i].origin = Vector(0,0,0);

      wp_display_time[i] = 0.0;

      paths[i] = NULL;  // no paths allocated yet
   }

   f_path_time = 0.0;  // reset waypoint path display time

   num_waypoints = 0;

   last_waypoint = Vector(0,0,0);

   for (unsigned int i=0; i < SHORTEST_PATH_ARRAY_LENGTH; i++)
   {
      shortest_path[i] = NULL;
      from_to[i] = NULL;
   }
}


void WaypointAddPath(short int add_index, short int path_index)
{
   path_t *p, *prev;
   int i;
   int count = 0;

   p = paths[add_index];
   prev = NULL;

   // find an empty slot for new path_index...
   while (p != NULL)
   {
      i = 0;

      while (i < MAX_PATH_INDEX)
      {
         if (p->index[i] == WAYPOINT_NOT_FOUND)
         {
            p->index[i] = path_index;

            return;
         }

         i++;
      }

      prev = p;     // save the previous node in linked list
      p = p->next;  // go to next node in linked list

#ifdef _DEBUG
      count++;
      if (count > 100) WaypointDebug();
#endif
   }

   p = (path_t *)malloc(sizeof(path_t));

   if (p == NULL)
   {
      ALERT(at_error, "Error allocating memory for path!\n");
   }

   p->index[0] = path_index;
   p->index[1] = WAYPOINT_NOT_FOUND;
   p->index[2] = WAYPOINT_NOT_FOUND;
   p->index[3] = WAYPOINT_NOT_FOUND;
   p->next = NULL;

   if (prev != NULL)
      prev->next = p;  // link new node into existing list

   if (paths[add_index] == NULL)
      paths[add_index] = p;  // save head point if necessary
}


void WaypointDeletePath(short int del_index)
{
   path_t *p;
   int index, i;

   // search all paths for this index...
   for (index=0; index < num_waypoints; index++)
   {
      p = paths[index];

      int count = 0;

      // search linked list for del_index...
      while (p != NULL)
      {
         i = 0;

         while (i < MAX_PATH_INDEX)
         {
            if (p->index[i] == del_index)
            {
               p->index[i] = WAYPOINT_NOT_FOUND;  // unassign this path
            }

            i++;
         }

         p = p->next;  // go to next node in linked list

#ifdef _DEBUG
         count++;
         if (count > 100) WaypointDebug();
#endif
      }
   }
}


void WaypointDeletePath(short int path_index, short int del_index)
{
   path_t *p;
   int i;
   int count = 0;

   p = paths[path_index];

   // search linked list for del_index...
   while (p != NULL)
   {
      i = 0;

      while (i < MAX_PATH_INDEX)
      {
         if (p->index[i] == del_index)
         {
            p->index[i] = WAYPOINT_NOT_FOUND;  // unassign this path
         }

         i++;
      }

      p = p->next;  // go to next node in linked list

#ifdef _DEBUG
      count++;
      if (count > 100) WaypointDebug();
#endif
   }
}


// find a path from the current waypoint. (pPath MUST be NULL on the
// initial call. subsequent calls will return other paths if they exist.)
int WaypointFindPath(path_t **pPath, int *path_index, int waypoint_index, int team)
{
   int index;
   int count = 0;

   if (*pPath == NULL)
   {
      *pPath = paths[waypoint_index];
      *path_index = 0;
   }

   if (*path_index == MAX_PATH_INDEX)
   {
      *path_index = 0;

      *pPath = (*pPath)->next;  // go to next node in linked list
   }

   while (*pPath != NULL)
   {
      while (*path_index < MAX_PATH_INDEX)
      {
         if ((*pPath)->index[*path_index] != WAYPOINT_NOT_FOUND)  // found a path?
         {
            // save the return value
            index = (*pPath)->index[*path_index];

            // skip this path if next waypoint is team specific and NOT this team
            if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) &&
                ((waypoints[index].flags & W_FL_TEAM) != team))
            {
               (*path_index)++;
               continue;
            }

            // set up stuff for subsequent calls...
            (*path_index)++;

            return index;
         }

         (*path_index)++;
      }

      *path_index = 0;

      *pPath = (*pPath)->next;  // go to next node in linked list

#ifdef _DEBUG
      count++;
      if (count > 100) WaypointDebug();
#endif
   }

   return WAYPOINT_NOT_FOUND;
}

// find the nearest waypoint to the player and return the index (-1 if not found)
int WaypointFindNearest(const edict_t *pEntity, float range, int team )
{
   int i, min_index;
   float distance;
   float min_distance;
   TraceResult tr;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find the nearest waypoint...

   min_index = WAYPOINT_NOT_FOUND;
   min_distance = 9999.0;

   for (i=0; i < num_waypoints; i++)
   {
      if (waypoints[i].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[i].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[i].flags & W_FL_TEAM_SPECIFIC) && ((waypoints[i].flags & W_FL_TEAM) != team))
         continue;

      distance = (waypoints[i].origin - pEntity->v.origin).Length();

      if ((distance < min_distance) && (distance < range))
      {
         // if waypoint is visible from current position (even behind head)...
         UTIL_TraceLine( pEntity->v.origin + pEntity->v.view_ofs, waypoints[i].origin,
                         ignore_monsters, pEntity->v.pContainingEntity, &tr );

         if (tr.flFraction >= 1.0)
         {
            min_index = i;
            min_distance = distance;
         }
      }
   }

   return min_index;
}


// find the nearest waypoint to the source postition and return the index
int WaypointFindNearest(const edict_t *pEntity, float range, int team, const Vector& v_src )
{
   int index, min_index;
   float distance;
   float min_distance;
   TraceResult tr;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find the nearest waypoint...

   min_index = WAYPOINT_NOT_FOUND;
   min_distance = 9999.0;

   for (index=0; index < num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[index].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) && ((waypoints[index].flags & W_FL_TEAM) != team))
         continue;

      distance = (waypoints[index].origin - v_src).Length();

      if ((distance < min_distance) && (distance < range))
      {
         // if waypoint is visible from source position...
         UTIL_TraceLine( v_src, waypoints[index].origin, ignore_monsters, pEntity->v.pContainingEntity, &tr );

         if (tr.flFraction >= 1.0)
         {
            min_index = index;
            min_distance = distance;
         }
      }
   }

   return min_index;
}


// find the nearest waypoint to the source postition and return the index
int WaypointFindNearest( const edict_t *pEntity, float range, int team, const Vector& v_src, uint64_t flags )
{
	int index, min_index;
	float distance;
	float min_distance;
	TraceResult tr;

	if (num_waypoints < 1)
		return WAYPOINT_NOT_FOUND;

	// find the nearest waypoint...

	min_index = WAYPOINT_NOT_FOUND;
	min_distance = 9999.0;

	for (index = 0; index < num_waypoints; index++)
	{
		if (waypoints[index].flags & W_FL_DELETED)
			continue;  // skip any deleted waypoints

		if (waypoints[index].flags & W_FL_AIMING)
			continue;  // skip any aiming waypoints

					   // skip this waypoint if it's team specific and teams don't match...
		if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) && ((waypoints[index].flags & W_FL_TEAM) != team))
			continue;

		// if it's not the type of waypoint requested
		if (!(waypoints[index].flags & flags))
		{
			continue;
		}

		distance = (waypoints[index].origin - v_src).Length();

		if ((distance < min_distance) && (distance < range))
		{
			// if waypoint is visible from source position...
			UTIL_TraceLine(v_src, waypoints[index].origin, ignore_monsters, pEntity->v.pContainingEntity, &tr);

			if (tr.flFraction >= 1.0)
			{
				min_index = index;
				min_distance = distance;
			}
		}
	}

	return min_index;
}

edict_t *FindNearest( const Vector point, const char *szClassname, float fMinimumDistance = 9999.99 )
{
	edict_t *pent = NULL;
	edict_t *pNearest = NULL;

	while( (pent = UTIL_FindEntityByClassname( pent, szClassname )) != NULL )
	{
		float fDistance = (pent->v.origin - point).Length();

		// is this the closest?
		if (fDistance < fMinimumDistance)
		{
			fMinimumDistance = fDistance;
			pNearest = pent;
		}
	}

	return pNearest;
}

edict_t *FindNearestTriggerCtfGeneric( const Vector point, const char *szClassname, float fMinimumDistance = 9999.99 )
{
	edict_t *pent = nullptr;
	edict_t *pNearest = nullptr;

	while( (pent = UTIL_FindEntityByClassname( pent, szClassname )) != nullptr)
	{
		// skip trigger_ctfgeneric with a targetname/without a triggerstate
		if( !strcmp( szClassname, "trigger_ctfgeneric" ) && strlen( STRING(pent->v.targetname) ) > 0 )
		{
			ALERT( at_console, "Skipping %s as it probably doesn't have a triggerstate attribute\n", STRING(pent->v.targetname) );
			continue;
		}

		float fDistance = (pent->v.origin - point).Length();

		// is this the closest?
		if (fDistance < fMinimumDistance)
		{
			fMinimumDistance = fDistance;
			pNearest = pent;
		}
	}

	return pNearest;
}

float DistanceToNearest(const Vector& point, const char *szClassname)
{
	edict_t *pent = NULL;
	float fMinimumDistance = 9999.99;

	while( (pent = UTIL_FindEntityByClassname( pent, szClassname )) != NULL )
	{
		float fDistance = (pent->v.origin - point).Length();

		// is this the closest?
		if (fDistance < fMinimumDistance)
		{
			fMinimumDistance = fDistance;
		}
	}

	return fMinimumDistance;
}

/**
 * index - the waypoint index
 * this should really be in bot classes
 */
bool ShouldSkip( const edict_t *pPlayer, int index )
{
	bot_t *pBot = UTIL_GetBotPointer( pPlayer );

	if( !pBot )
	{
		ALERT( at_console, "Got NULL pointer in ShouldSkip\n" );

		return false;
	}

	if( mod_id == GEARBOX_DLL && pGame->IsCTF() )
	{
		if( waypoints[index].flags == W_FL_FLAG )
		{
			edict_t *pNearestFlag = FindNearest(waypoints[index].origin, "item_ctfflag", 75.0);

			// if there's not a nearby item_ctfflag
			if( !pNearestFlag )
			{
				return false;
			}

			if( UTIL_GetTeam(pBot->pEdict) == OpposingForceBot::TEAM_BLACK_MESA && pNearestFlag->v.skin == OpposingForceBot::OPPOSING_FORCE_FLAG_SKIN )
			{
				return false;
			}
			else if( UTIL_GetTeam(pBot->pEdict) == OpposingForceBot::TEAM_OPPOSING_FORCE && pNearestFlag->v.skin == OpposingForceBot::BLACK_MESA_FLAG_SKIN )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if( waypoints[index].flags == W_FL_FLAG_GOAL )
		{
			edict_t *pNearestBase = FindNearest(waypoints[index].origin, "item_ctfbase");

			// if there's not a nearby item_ctfbase
			if (!pNearestBase)
			{
				return false;
			}

			if ( UTIL_GetTeam(pBot->pEdict) == OpposingForceBot::TEAM_BLACK_MESA && !strcmp(STRING(pNearestBase->v.model), "models/civ_stand.mdl") )
			{
				return false;
			}
			else if ( UTIL_GetTeam(pBot->pEdict) == OpposingForceBot::TEAM_OPPOSING_FORCE && !strcmp(STRING(pNearestBase->v.model), "models/mil_stand.mdl") )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else if( mod_id == GEARBOX_DLL && pGame->IsCapturePoint() && (waypoints[index].flags == W_FL_OP4_CAPTURE_POINT) )
	{
		edict_t *pNearestCapturePoint = FindNearestTriggerCtfGeneric(waypoints[index].origin, "trigger_ctfgeneric");

		ALERT(at_console, "near a trigger_ctfgeneric\n");

		// if there's not a nearby trigger_ctfgeneric
		if( !pNearestCapturePoint )
		{
			return false;
		}

		ALERT(at_console, "really near a trigger_ctfgeneric\n");

		// if the trigger_ctfgeneric nearest this waypoint already belongs to the same team as the player
		if( !pBot->ShouldCapturePoint( pNearestCapturePoint ) )
		{
			return true;
		}
	}
	else if( mod_id == DOD_DLL && waypoints[index].flags == W_FL_DOD_CAP )
	{
		edict_t *pNearestControlPoint = FindNearest(waypoints[index].origin, "dod_control_point");

		// if there's not a nearby dod_control_point
		if( !pNearestControlPoint )
		{
			return false;
		}

		// if the dod_control_point nearest this waypoint already belongs to the same team as the player
		if( !pBot->ShouldCapturePoint( pNearestControlPoint ) )
		{
			return true;
		}
	}
	else if( mod_id == NS_DLL && waypoints[index].flags == W_FL_NS_HIVE )
	{
		edict_t *pNearestHive = FindNearest( waypoints[index].origin, "team_hive" );

		if( !((NSBot *)pBot)->ShouldAttackHive( pNearestHive ) )
		{
			return true;
		}
	}
	else if( mod_id == NS_DLL && waypoints[index].flags == W_FL_NS_RESNODE && ((NSGame *)pGame.get())->IsClassic() )
	{
		if( ((NSBot *)pBot)->IsMarine() )
		{
			// get the resource tower nearest this waypoint
			edict_t *pNearestResourceTower = FindNearest( waypoints[index].origin, "resourcetower", 5.0f );

			// if there is a resource tower near this waypoint
			if( pNearestResourceTower )
			{
				return UTIL_IsBuilt( pNearestResourceTower );
			}
		}
		else if( ((NSBot *)pBot)->IsAlien() && ((NSBot *)pBot)->IsGorge() )
		{
			// get the resource tower nearest this waypoint
			edict_t *pNearestResourceTower = FindNearest( waypoints[index].origin, "alienresourcetower", 5.0f );

			// if there is a resource tower near this waypoint and it's built, skip it
			// TODO: if it's damaged heal it?
			if( pNearestResourceTower && UTIL_IsBuilt( pNearestResourceTower ) )
			{
				return false;
			}
			// go and build one
			else
			{
				return true;
			}
		}
	}

	return false;
}


int WaypointFindNearestGoal(const edict_t *pEntity, int src, int team, uint64_t flags)
{
   int index, min_index;
   int distance, min_distance;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find the nearest waypoint with the matching flags...

   min_index = WAYPOINT_NOT_FOUND;
   min_distance = 99999;

   for (index=0; index < num_waypoints; index++)
   {
      if (index == src)
         continue;  // skip the source waypoint

      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[index].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) && ((waypoints[index].flags & W_FL_TEAM) != team))
         continue;

      if ((waypoints[index].flags & flags) != flags)
         continue;  // skip this waypoint if the flags don't match

		if (ShouldSkip(pEntity, index))
		{
			continue;
		}

      distance = WaypointDistanceFromTo(src, index, team);

      if (distance < min_distance)
      {
         min_index = index;
         min_distance = distance;
      }
   }

   return min_index;
}


int WaypointFindNearestGoal(const edict_t *pEntity, int src, int team, uint64_t flags, int exclude[])
{
   int index, min_index;
   int distance, min_distance;
   int exclude_index;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find the nearest waypoint with the matching flags...

   min_index = WAYPOINT_NOT_FOUND;
   min_distance = 99999;

   for (index=0; index < num_waypoints; index++)
   {
      if (index == src)
         continue;  // skip the source waypoint

      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[index].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) && ((waypoints[index].flags & W_FL_TEAM) != team))
         continue;

      if ((waypoints[index].flags & flags) != flags)
         continue;  // skip this waypoint if the flags don't match

		if (ShouldSkip(pEntity, index))
		{
			continue;
		}

      exclude_index = 0;
      while (exclude[exclude_index])
      {
         if (index == exclude[exclude_index])
            break;  // found a match, break out of while loop

         exclude_index++;
      }

      if (index == exclude[exclude_index])
         continue;  // skip any index that matches exclude list

      distance = WaypointDistanceFromTo(src, index, team);

      if (distance < min_distance)
      {
         min_index = index;
         min_distance = distance;
      }
   }

   return min_index;
}

int WaypointFindNearestGoal(const Vector& v_src, const edict_t *pEntity, float range, int team, uint64_t flags)
{
   int index, min_index;
   int distance, min_distance;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find the nearest waypoint with the matching flags...

   min_index = WAYPOINT_NOT_FOUND;
   min_distance = 99999;

   for (index=0; index < num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[index].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) &&
          ((waypoints[index].flags & W_FL_TEAM) != team))
         continue;

      if ((waypoints[index].flags & flags) != flags)
         continue;  // skip this waypoint if the flags don't match

		if (ShouldSkip(pEntity, index))
		{
			continue;
		}

      distance = (waypoints[index].origin - v_src).Length();

      if ((distance < range) && (distance < min_distance))
      {
         min_index = index;
         min_distance = distance;
      }
   }

   return min_index;
}

int WaypointFindRandomGoal(const edict_t *pEntity, int team, uint64_t flags)
{
   int index;
   int indexes[200];
   int count = 0;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find all the waypoints with the matching flags...

   for (index=0; index < num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[index].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) &&
          ((waypoints[index].flags & W_FL_TEAM) != team))
         continue;

      if ((waypoints[index].flags & flags) != flags)
         continue;  // skip this waypoint if the flags don't match

      if (count < 200)
      {
         indexes[count] = index;

         count++;
      }
   }

   if (count == 0)  // no matching waypoints found
      return WAYPOINT_NOT_FOUND;

   index = RANDOM_LONG(1, count) - 1;

   return indexes[index];
}


int WaypointFindRandomGoal(const edict_t *pEntity, int team, uint64_t flags, int exclude[])
{
   int index;
   int indexes[200];
   int count = 0;
   int exclude_index;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find all the waypoints with the matching flags...

   for (index=0; index < num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[index].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) &&
          ((waypoints[index].flags & W_FL_TEAM) != team))
         continue;

      if ((waypoints[index].flags & flags) != flags)
         continue;  // skip this waypoint if the flags don't match

      exclude_index = 0;
      while (exclude[exclude_index])
      {
         if (index == exclude[exclude_index])
            break;  // found a match, break out of while loop

         exclude_index++;
      }

      if (index == exclude[exclude_index])
         continue;  // skip any index that matches exclude list

      if (count < 200)
      {
         indexes[count] = index;

         count++;
      }
   }

   if (count == 0)  // no matching waypoints found
      return WAYPOINT_NOT_FOUND;

   index = RANDOM_LONG(1, count) - 1;

   return indexes[index];
}


int WaypointFindRandomGoal(const Vector& v_src, const edict_t *pEntity, float range, int team, uint64_t flags)
{
   int index;
   int indexes[200];
   int count = 0;
   float distance;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // find all the waypoints with the matching flags...

   for (index=0; index < num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[index].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[index].flags & W_FL_TEAM_SPECIFIC) &&
          ((waypoints[index].flags & W_FL_TEAM) != team))
         continue;

      if ((waypoints[index].flags & flags) != flags)
         continue;  // skip this waypoint if the flags don't match

      distance = (waypoints[index].origin - v_src).Length();

      if ((distance < range) && (count < 200))
      {
         indexes[count] = index;

         count++;
      }
   }

   if (count == 0)  // no matching waypoints found
      return WAYPOINT_NOT_FOUND;

   index = RANDOM_LONG(1, count) - 1;

   return indexes[index];
}


int WaypointFindNearestAiming(const Vector& v_origin)
{
   int index;
   int min_index = WAYPOINT_NOT_FOUND;
   int min_distance = 9999.0;
   float distance;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // search for nearby aiming waypoint...
   for (index=0; index < num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if ((waypoints[index].flags & W_FL_AIMING) == 0)
         continue;  // skip any NON aiming waypoints

      distance = (v_origin - waypoints[index].origin).Length();

      if ((distance < min_distance) && (distance < 40))
      {
         min_index = index;
         min_distance = distance;
      }
   }

   return min_index;
}

int WaypointFindNearestWaypoint(const edict_t *pEntity, uint64_t type)
{
   int index;
   int min_index = WAYPOINT_NOT_FOUND;
   int min_distance = 9999.0;
   float distance;

   if (num_waypoints < 1)
      return WAYPOINT_NOT_FOUND;

   // search for nearby waypoint...
   for (index=0; index < num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

	  if (!type || !(waypoints[index].flags & type))
	  {
		  continue;
	  }

      distance = (pEntity->v.origin - waypoints[index].origin).Length();

      if (distance < min_distance)
      {
         min_index = index;
         min_distance = distance;
      }
   }

   return min_index;
}


void WaypointDrawBeam( edict_t *pEntity, const Vector& start, const Vector& end, int width, int noise, int red, int green, int blue, int brightness, int speed )
{
	// don't send waypoint render messages to bots
	if( !(pEntity->v.flags & FL_FAKECLIENT) )
	{
		MESSAGE_BEGIN(MSG_ONE, SVC_TEMPENTITY, NULL, pEntity);
			WRITE_BYTE(TE_BEAMPOINTS);
			WRITE_COORD(start.x);
			WRITE_COORD(start.y);
			WRITE_COORD(start.z);
			WRITE_COORD(end.x);
			WRITE_COORD(end.y);
			WRITE_COORD(end.z);
			WRITE_SHORT(m_spriteTexture);
			WRITE_BYTE(1); // framestart
			WRITE_BYTE(10); // framerate
			WRITE_BYTE(10); // life in 0.1's
			WRITE_BYTE(width); // width
			WRITE_BYTE(noise);  // noise

			WRITE_BYTE(red);   // r, g, b
			WRITE_BYTE(green);   // r, g, b
			WRITE_BYTE(blue);   // r, g, b

			WRITE_BYTE(brightness);   // brightness
			WRITE_BYTE(speed);    // speed
		MESSAGE_END();
	}
}

// pEntity is the player
void WaypointSearchItems( edict_t *pEntity, const Vector& origin, int wpt_index )
{
	edict_t *pent = nullptr;
	float radius = 300.0f; // needs to be big as team_hive hangs from the ceiling so won't be that close to the player
	TraceResult tr;
	float distance;
	float min_distance = 9999.0f;
	char item_name[64];
	char nearest_name[64] = "";
	edict_t *nearest_pent = nullptr;
	int tfc_backpack_index = -1;  // "null" out backpack index

   //********************************************************
   // look for the nearest health, armor, ammo, weapon, etc.
   //********************************************************
   while( (pent = UTIL_FindEntityInSphere( pent, origin, radius )) != nullptr )
   {
      if ( pEntity )
	  {
         UTIL_TraceLine( origin, pent->v.origin, ignore_monsters, pEntity->v.pContainingEntity, &tr );
	  }
      else
	  {
         UTIL_TraceLine( origin, pent->v.origin, ignore_monsters, nullptr, &tr );
	  }

	  strcpy(item_name, STRING(pent->v.classname));

      // check that the entity is visible
      if( tr.flFraction >= 1.0 || !strcmp("trigger_ctfgeneric", item_name) )
      {
         strcpy(item_name, STRING(pent->v.classname));

         if ((!strncmp("item_health", item_name, 11) || !strncmp("item_armor", item_name, 10) ||
			!strncmp("ammo_", item_name, 5) || !strcmp("item_cells", item_name) || !strcmp("item_shells", item_name) ||
			!strcmp("item_spikes", item_name) || !strcmp("item_rockets", item_name) ||
			!strcmp("team_hive", item_name) || !strcmp("team_command", item_name) || !strcmp("dod_control_point", item_name) ||
			!strncmp("weapon_", item_name, 7) || !strcmp("item_ctfbase", item_name) || !strcmp("item_ctfflag", item_name) ||
			 !strcmp("trigger_ctfgeneric", item_name)) &&
			 (pent->v.owner == nullptr))
         {
            distance = (pent->v.origin - origin).Length();

			UTIL_LogPrintf("%f dist\n", distance);

            if (distance < min_distance)
            {
               strcpy(nearest_name, item_name);

			   UTIL_LogPrintf("%s is closer\n", item_name);

               tfc_backpack_index = -1;  // "null" out backpack index

               nearest_pent = pent;

               min_distance = distance;
            }
         }

         if (mod_id == TFC_DLL)
         {
            for (int bck_index=0; bck_index < num_backpacks; bck_index++)
            {
               distance = (pent->v.origin - origin).Length();

               if ((backpacks[bck_index].edict == pent) && (distance < min_distance))
               {
                  tfc_backpack_index = bck_index;

                  nearest_pent = pent;

                  nearest_name[0] = 0;  // null out nearest_name string

                  min_distance = distance;

                  break;
               }
            }
         }
      }
   }

   UTIL_LogPrintf( "%s is the closest entity\n", nearest_name );

   if (nearest_name[0])  // found an entity name
   {
      if (strncmp("item_health", nearest_name, 11) == 0)
      {
         if (pEntity)
            ClientPrint(pEntity, HUD_PRINTCONSOLE, "found a healthkit!\n");
         waypoints[wpt_index].flags |= W_FL_HEALTH;
      }

      if (strncmp("item_armor", nearest_name, 10) == 0)
      {
         if (pEntity)
            ClientPrint(pEntity, HUD_PRINTCONSOLE, "found some armor!\n");
         waypoints[wpt_index].flags |= W_FL_ARMOR;
      }

      if ((strncmp("ammo_", nearest_name, 5) == 0) || (strcmp("item_cells", nearest_name) == 0) ||
          (strcmp("item_shells", nearest_name) == 0) || (strcmp("item_spikes", nearest_name) == 0) ||
          (strcmp("item_rockets", nearest_name) == 0))
      {
         if (pEntity)
            ClientPrint(pEntity, HUD_PRINTCONSOLE, "found some ammo!\n");
         waypoints[wpt_index].flags |= W_FL_AMMO;
      }

      if ((strncmp("weapon_", nearest_name, 7) == 0) && (nearest_pent->v.owner == NULL))
      {
         if (pEntity)
            ClientPrint(pEntity, HUD_PRINTCONSOLE, "found a weapon!\n");
         waypoints[wpt_index].flags |= W_FL_WEAPON;
      }

		if ((strcmp("item_ctfbase", nearest_name) == 0))
		{
			if (pEntity)
				ClientPrint(pEntity, HUD_PRINTCONSOLE, "found an item_ctfbase\n");
			waypoints[wpt_index].flags |= W_FL_FLAG_GOAL;
		}

		if( !strcmp("item_ctfflag", nearest_name) )
		{
			if (pEntity)
				ClientPrint(pEntity, HUD_PRINTCONSOLE, "found an item_ctfflag\n");
			waypoints[wpt_index].flags |= W_FL_FLAG;
		}

		if( !strcmp("trigger_ctfgeneric", nearest_name) )
		{
			if( pEntity )
			{
				ClientPrint( pEntity, HUD_PRINTCONSOLE, "Found a trigger_ctfgeneric\n" );
			}
			waypoints[wpt_index].flags |= W_FL_OP4_CAPTURE_POINT;
		}

		if ((strcmp("dod_object", nearest_name) == 0))
		{
			if (pEntity)
				ClientPrint(pEntity, HUD_PRINTCONSOLE, "found a dod_object\n");
			waypoints[wpt_index].flags |= W_FL_DOD_OBJ;
		}

		if ((strcmp("dod_control_point", nearest_name) == 0))
		{
			if (pEntity)
				ClientPrint(pEntity, HUD_PRINTCONSOLE, "found a control point!\n");
			waypoints[wpt_index].flags |= W_FL_DOD_CAP;
		}

		if ((strcmp("team_hive", nearest_name) == 0))
		{
			if (pEntity)
				ClientPrint(pEntity, HUD_PRINTCONSOLE, "found a hive!\n");
			waypoints[wpt_index].flags |= W_FL_NS_HIVE;
		}

		if ((strcmp("team_command", nearest_name) == 0))
		{
			if (pEntity)
				ClientPrint(pEntity, HUD_PRINTCONSOLE, "found a command chair!\n");
			waypoints[wpt_index].flags |= W_FL_NS_COMMAND_CHAIR;
		}
   }

   if ((mod_id == TFC_DLL) && (tfc_backpack_index != -1))  // found a TFC backpack
   {
      if (backpacks[tfc_backpack_index].health)
      {
         if (pEntity)
            ClientPrint(pEntity, HUD_PRINTCONSOLE, "found health!\n");
         waypoints[wpt_index].flags |= W_FL_HEALTH;
      }

      if (backpacks[tfc_backpack_index].armor)
      {
         if (pEntity)
            ClientPrint(pEntity, HUD_PRINTCONSOLE, "found some armor!\n");
         waypoints[wpt_index].flags |= W_FL_ARMOR;
      }

      if (backpacks[tfc_backpack_index].ammo)
      {
         if (pEntity)
            ClientPrint(pEntity, HUD_PRINTCONSOLE, "found some ammo!\n");
         waypoints[wpt_index].flags |= W_FL_AMMO;
      }
   }
}


void WaypointAdd(edict_t *pEntity, int flags = 0)
{
   int index = 0;

   if (num_waypoints >= MAX_WAYPOINTS)
   {
      return;
   }

   // find the next available slot for the new waypoint...
   while (index < num_waypoints)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         break;

      index++;
   }

   waypoints[index].flags = 0;

   // store the origin (location) of this waypoint (use entity origin)
   waypoints[index].origin = pEntity->v.origin;

   // store the last used waypoint for the auto waypoint code...
   last_waypoint = pEntity->v.origin;

   // set the time that this waypoint was originally displayed...
   wp_display_time[index] = gpGlobals->time;


   Vector start, end;

   start = pEntity->v.origin - Vector(0, 0, 34);
   end = start + Vector(0, 0, 68);

   if ((pEntity->v.flags & FL_DUCKING) == FL_DUCKING)
   {
      waypoints[index].flags |= W_FL_CROUCH;  // crouching waypoint

      start = pEntity->v.origin - Vector(0, 0, 17);
      end = start + Vector(0, 0, 34);
   }

   if (pEntity->v.movetype == MOVETYPE_FLY)
      waypoints[index].flags |= W_FL_LADDER;  // waypoint on a ladder

   if (flags)
   {
	   waypoints[index].flags = flags;
   }
   else
   {
	   // search the area near the waypoint for items (HEALTH, AMMO, WEAPON, etc.)
	   WaypointSearchItems(pEntity, waypoints[index].origin, index);
   }

   // TODO: this isn't working - the colours that is
   if( waypoints[index].flags & W_FL_NS_HIVE )
   {
	   WaypointDrawBeam(pEntity, start, end, 30, 0, 0, 0, 255, 0, 0);
   }
   else if( waypoints[index].flags & 2097152 )
   {
	   WaypointDrawBeam(pEntity, start, end, 30, 0, 0, 0, 0, 255, 0);
   }
   else
   {
	   // draw a blue waypoint
		WaypointDrawBeam(pEntity, start, end, 30, 0, 0, 0, 255, 250, 5);
   }

   EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "weapons/xbow_hit1.wav", 1.0, ATTN_NORM, 0, 100);

   // increment total number of waypoints if adding at end of array...
   if (index == num_waypoints)
      num_waypoints++;

   // calculate all the paths to this new waypoint
   for (int i=0; i < num_waypoints; i++)
   {
      if (i == index)
         continue;  // skip the waypoint that was just added

      if (waypoints[i].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // check if the waypoint is reachable from the new one (one-way)
      if ( WaypointReachable(pEntity->v.origin, waypoints[i].origin, pEntity) && g_path_waypoint_enable)
      {
         WaypointAddPath(index, i);
      }

      // check if the new one is reachable from the waypoint (other way)
      if ( WaypointReachable(waypoints[i].origin, pEntity->v.origin, pEntity) && g_path_waypoint_enable)
      {
         WaypointAddPath(i, index);
      }
   }
}


void WaypointAddAiming(edict_t *pEntity)
{
	int index = 0;

   if (num_waypoints >= MAX_WAYPOINTS)
      return;

   // find the next available slot for the new waypoint...
   while (index < num_waypoints)
   {
      if (waypoints[index].flags & W_FL_DELETED)
         break;

      index++;
   }

   waypoints[index].flags = W_FL_AIMING;  // aiming waypoint

   Vector v_angle = pEntity->v.v_angle;

   v_angle.x = 0;  // reset pitch to horizontal
   v_angle.z = 0;  // reset roll to level

   MAKE_VECTORS(v_angle);

   // store the origin (location) of this waypoint (use entity origin)
   waypoints[index].origin = pEntity->v.origin + gpGlobals->v_forward * 25;

   // set the time that this waypoint was originally displayed...
   wp_display_time[index] = gpGlobals->time;


   Vector start, end;

   start = pEntity->v.origin - Vector(0, 0, 10);
   end = start + Vector(0, 0, 14);

   // draw a blue waypoint
   WaypointDrawBeam(pEntity, start, end, 30, 0, 0, 0, 255, 250, 5);

   EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "weapons/xbow_hit1.wav", 1.0, ATTN_NORM, 0, 100);

   // increment total number of waypoints if adding at end of array...
   if (index == num_waypoints)
      num_waypoints++;
}


void WaypointDelete(edict_t *pEntity)
{
   int index;
   int count = 0;

   if (num_waypoints < 1)
      return;

   index = WaypointFindNearest(pEntity, 50.0, -1);

   if (index == WAYPOINT_NOT_FOUND)
      return;

   if ((waypoints[index].flags & W_FL_SNIPER) || (waypoints[index].flags & W_FL_TFC_SENTRYGUN) ||
       (waypoints[index].flags & W_FL_TFC_DISPENSER) || (waypoints[index].flags & W_FL_JUMP))
   {
      int i;
      int min_index = WAYPOINT_NOT_FOUND;
      int min_distance = 9999.0;
      float distance;

      // search for nearby aiming waypoint and delete it also...
      for (i=0; i < num_waypoints; i++)
      {
         if (waypoints[i].flags & W_FL_DELETED)
            continue;  // skip any deleted waypoints

         if ((waypoints[i].flags & W_FL_AIMING) == 0)
            continue;  // skip any NON aiming waypoints

         distance = (waypoints[i].origin - waypoints[index].origin).Length();

         if ((distance < min_distance) && (distance < 40))
         {
            min_index = i;
            min_distance = distance;
         }
      }

      if (min_index != WAYPOINT_NOT_FOUND)
      {
         waypoints[min_index].flags = W_FL_DELETED;  // not being used
         waypoints[min_index].origin = Vector(0,0,0);

         wp_display_time[min_index] = 0.0;
      }
   }

   // delete any paths that lead to this index...
   WaypointDeletePath(index);

   // free the path for this index...

   if (paths[index] != NULL)
   {
      path_t *p = paths[index];
      path_t *p_next;

      while (p)  // free the linked list
      {
         p_next = p->next;  // save the link to next
         free(p);
         p = p_next;

#ifdef _DEBUG
         count++;
         if (count > 100) WaypointDebug();
#endif
      }

      paths[index] = NULL;
   }

   waypoints[index].flags = W_FL_DELETED;  // not being used
   waypoints[index].origin = Vector(0,0,0);

   wp_display_time[index] = 0.0;

   EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "weapons/mine_activate.wav", 1.0,  ATTN_NORM, 0, 100);
}


void WaypointUpdate( const edict_t *pEntity )
{
	int mask = W_FL_HEALTH | W_FL_ARMOR | W_FL_AMMO | W_FL_WEAPON;

	for( int i = 0; i < num_waypoints; i++ )
	{
		waypoints[i].flags &= ~mask;  // clear the mask bits

		WaypointSearchItems( nullptr, waypoints[i].origin, i );
	}
}


// allow player to manually create a path from one waypoint to another
void WaypointCreatePath(edict_t *pEntity, int cmd)
{
   static int waypoint1 = WAYPOINT_NOT_FOUND;  // initialized to unassigned
   static int waypoint2 = WAYPOINT_NOT_FOUND;  // initialized to unassigned

   if (cmd == 1)  // assign source of path
   {
      waypoint1 = WaypointFindNearest(pEntity, 50.0, -1);

      if (waypoint1 == WAYPOINT_NOT_FOUND)
      {
         // play "cancelled" sound...
         EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_moveselect.wav", 1.0, ATTN_NORM, 0, 100);

         return;
      }

      // play "start" sound...
      EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_hudoff.wav", 1.0, ATTN_NORM, 0, 100);

      return;
   }

   if (cmd == 2)  // assign dest of path and make path
   {
      waypoint2 = WaypointFindNearest(pEntity, 50.0, -1);

      if ((waypoint1 == WAYPOINT_NOT_FOUND) || (waypoint2 == WAYPOINT_NOT_FOUND))
      {
         // play "error" sound...
         EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_denyselect.wav", 1.0, ATTN_NORM, 0, 100);

         return;
      }

      WaypointAddPath(waypoint1, waypoint2);

      // play "done" sound...
      EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_hudon.wav", 1.0, ATTN_NORM, 0, 100);
   }
}


// allow player to manually remove a path from one waypoint to another
void WaypointRemovePath(edict_t *pEntity, int cmd)
{
   static int waypoint1 = WAYPOINT_NOT_FOUND;  // initialized to unassigned
   static int waypoint2 = WAYPOINT_NOT_FOUND;  // initialized to unassigned

   if (cmd == 1)  // assign source of path
   {
      waypoint1 = WaypointFindNearest(pEntity, 50.0, -1);

      if (waypoint1 == WAYPOINT_NOT_FOUND)
      {
         // play "cancelled" sound...
         EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_moveselect.wav", 1.0, ATTN_NORM, 0, 100);

         return;
      }

      // play "start" sound...
      EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_hudoff.wav", 1.0, ATTN_NORM, 0, 100);

      return;
   }

   if (cmd == 2)  // assign dest of path and make path
   {
      waypoint2 = WaypointFindNearest(pEntity, 50.0, -1);

      if ((waypoint1 == WAYPOINT_NOT_FOUND) || (waypoint2 == WAYPOINT_NOT_FOUND))
      {
         // play "error" sound...
         EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_denyselect.wav", 1.0, ATTN_NORM, 0, 100);

         return;
      }

      WaypointDeletePath(waypoint1, waypoint2);

      // play "done" sound...
      EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/wpn_hudon.wav", 1.0, ATTN_NORM, 0, 100);
   }
}


bool WaypointLoad(edict_t *pEntity)
{
	FILE *bfp;
	char mapname[64];
	char filename[256];
	char new_filename[256];
	#ifdef __linux__
	char cmd[512];
	#endif
	WAYPOINT_HDR header;
	char msg[80];
	int index, i;
	short int num;
	short int path_index;
	bool need_rename;

// only supported in newer versions of C++ (which Visual Studio doesn't say it supports, but is only supported by
// Visual Studio 2010 onwards: https://msdn.microsoft.com/en-us/library/dd293588.aspx)
#if (_MSC_VER >= 1600) || (__cplusplus >= 201103L)
	// compiler-specific packing sucks - some day it should all be 0 packing but I don't want to recreate the waypoint files
	static_assert(sizeof(waypoint_t) == 24, "waypoint_t should be 24 bytes");
	static_assert(sizeof(Vector) == 12, "Vector should be 12 bytes");
	static_assert(offsetof(waypoint_t, origin) == 8, "origin should be 8 bytes into waypoint_t");

	static_assert(sizeof(path_t) == 12, "path_t should be 12 bytes");
	static_assert(offsetof(path_t, next) == 8, "next should be 8 bytes into PATH");
#else
	#warning No checking of waypoint structure layouts - there may be problems
#endif

	strcpy(mapname, STRING(gpGlobals->mapname));
	strcat(mapname, ".wpt");

	UTIL_BuildFileName(filename, "maps", mapname);

	if (IS_DEDICATED_SERVER())
		printf("loading waypoint file: %s\n", filename);

	bfp = fopen(filename, "rb");

	need_rename = FALSE;

   // if .HBP_wpt files doesn't exist, check .wpt file...
   if (bfp == NULL)
   {
      need_rename = TRUE;

      strcpy(mapname, STRING(gpGlobals->mapname));
      strcat(mapname, ".wpt");

      UTIL_BuildFileName(filename, "maps", mapname);

      if (IS_DEDICATED_SERVER())
         printf("loading waypoint file: %s\n", filename);

      bfp = fopen(filename, "rb");
   }

   // if file exists, read the waypoint structure from it
   if (bfp != NULL)
   {
      fread(&header, sizeof(header), 1, bfp);

      header.filetype[7] = 0;
      if (strcmp(header.filetype, WAYPOINT_HEADER) == 0)
      {
         if (header.waypoint_file_version != WAYPOINT_VERSION)
         {
            if (pEntity)
               ClientPrint(pEntity, HUD_PRINTNOTIFY, "Incompatible Sandbot waypoint file version!\nWaypoints not loaded!\n");

            fclose(bfp);
            return FALSE;
         }

         header.mapname[31] = 0;

         if (strcmp(header.mapname, STRING(gpGlobals->mapname)) == 0)
         {
            WaypointInit();  // remove any existing waypoints

            for (i=0; i < header.number_of_waypoints; i++)
            {
               fread(&waypoints[i], sizeof(waypoints[0]), 1, bfp);
               num_waypoints++;
            }

            // read and add waypoint paths...
            for (index=0; index < num_waypoints; index++)
            {
               // read the number of paths from this node...
               fread(&num, sizeof(num), 1, bfp);

               for (i=0; i < num; i++)
               {
                  fread(&path_index, sizeof(path_index), 1, bfp);

                  WaypointAddPath(index, path_index);
               }
            }

            g_waypoint_paths = TRUE;  // keep track so path can be freed
         }
         else
         {
            if (pEntity)
            {
               sprintf(msg, "%s HPB bot waypoints are not for this map!\n", filename);
               ClientPrint(pEntity, HUD_PRINTNOTIFY, msg);
            }

            fclose(bfp);
            return FALSE;
         }
      }
      else
      {
         if (pEntity)
         {
            sprintf(msg, "%s is not a HPB bot waypoint file!\n", filename);
            ClientPrint(pEntity, HUD_PRINTNOTIFY, msg);
         }

         fclose(bfp);
         return FALSE;
      }

      fclose(bfp);

      if (need_rename)
      {
         strcpy(mapname, STRING(gpGlobals->mapname));
         strcat(mapname, ".wpt");

         UTIL_BuildFileName(new_filename, "maps", mapname);

#ifndef __linux__
         rename(filename, new_filename);
#else
         sprintf(cmd, "/bin/mv -f %s %s", filename, new_filename);
         system(cmd);
#endif
      }

      WaypointRouteInit();
   }
   else
   {
      if (pEntity)
      {
         sprintf(msg, "Waypoint file %s does not exist!\n", filename);
         ClientPrint(pEntity, HUD_PRINTNOTIFY, msg);
      }

      if (IS_DEDICATED_SERVER())
         printf("waypoint file %s not found!\n", filename);

      return FALSE;
   }

   return TRUE;
}


void WaypointSave()
{
	char filename[256];
	char mapname[64];
	WAYPOINT_HDR header;
	int index, i;
	short int num;
	path_t *p;

	strcpy(header.filetype, WAYPOINT_HEADER);

	header.waypoint_file_version = WAYPOINT_VERSION;
	header.waypoint_file_flags = mod_id;
	header.number_of_waypoints = num_waypoints;

	memset(header.mapname, 0, sizeof(header.mapname));
	strncpy(header.mapname, STRING(gpGlobals->mapname), 31);
	header.mapname[31] = 0;

	strcpy(mapname, STRING(gpGlobals->mapname));
	strcat(mapname, ".wpt");

	UTIL_BuildFileName(filename, "maps", mapname);

	FILE *pFile = fopen(filename, "wb");

	// write the waypoint header to the file
	fwrite( &header, sizeof(header), 1, pFile );

	// write the waypoint data to the file
	for( int i = 0; i < num_waypoints; i++)
	{
		fwrite( &waypoints[i], sizeof(waypoints[0]), 1, pFile );
	}

	// save the waypoint paths...
	for (index=0; index < num_waypoints; index++)
	{
		// count the number of paths from this node...
		p = paths[index];
		num = 0;

		while (p != nullptr)
		{
			i = 0;

			while (i < MAX_PATH_INDEX)
			{
				if (p->index[i] != WAYPOINT_NOT_FOUND)
					num++;  // count path node if it's used

				i++;
			}

			p = p->next;  // go to next node in linked list
		}

		// write the count
		fwrite( &num, sizeof(num), 1, pFile );

		// now write out each path index
		p = paths[index];

		while (p != nullptr)
		{
			i = 0;

			while (i < MAX_PATH_INDEX)
			{
				if (p->index[i] != WAYPOINT_NOT_FOUND)  // save path node if it's used
					fwrite(&p->index[i], sizeof(p->index[0]), 1, pFile);

				i++;
			}

			// go to next node in linked list
			p = p->next;
		}
	}

	fclose(pFile);
}


bool WaypointReachable(const Vector& v_src, const Vector& v_dest, const edict_t *pEntity)
{
   TraceResult tr;
   float curr_height, last_height;

   float distance = (v_dest - v_src).Length();

   // is the destination close enough?
   if (distance < REACHABLE_RANGE)
   {
      // check if this waypoint is "visible"...

      UTIL_TraceLine( v_src, v_dest, ignore_monsters, pEntity->v.pContainingEntity, &tr );

      // if waypoint is visible from current position (even behind head)...
      if (tr.flFraction >= 1.0)
      {
         // check for special case of both waypoints being underwater...
         if ((POINT_CONTENTS( v_src ) == CONTENTS_WATER) && (POINT_CONTENTS( v_dest ) == CONTENTS_WATER))
         {
            return TRUE;
         }

         // check for special case of waypoint being suspended in mid-air...

         // is dest waypoint higher than src? (45 is max jump height)
         if (v_dest.z > (v_src.z + 45.0))
         {
            Vector v_new_src = v_dest;
            Vector v_new_dest = v_dest;

            v_new_dest.z = v_new_dest.z - 50;  // straight down 50 units

            UTIL_TraceLine(v_new_src, v_new_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

            // check if we didn't hit anything, if not then it's in mid-air
            if (tr.flFraction >= 1.0)
            {
               return FALSE;  // can't reach this one
            }
         }

         // check if distance to ground increases more than jump height
         // at points between source and destination...

         Vector v_direction = (v_dest - v_src).Normalize();  // 1 unit long
         Vector v_check = v_src;
         Vector v_down = v_src;

         v_down.z = v_down.z - 1000.0;  // straight down 1000 units

         UTIL_TraceLine(v_check, v_down, ignore_monsters, pEntity->v.pContainingEntity, &tr);

         last_height = tr.flFraction * 1000.0;  // height from ground

         distance = (v_dest - v_check).Length();  // distance from goal

         while (distance > 10.0)
         {
            // move 10 units closer to the goal...
            v_check = v_check + (v_direction * 10.0);

            v_down = v_check;
            v_down.z = v_down.z - 1000.0;  // straight down 1000 units

            UTIL_TraceLine(v_check, v_down, ignore_monsters, pEntity->v.pContainingEntity, &tr);

            curr_height = tr.flFraction * 1000.0;  // height from ground

            // is the difference in the last height and the current height
            // higher that the jump height?
            if ((last_height - curr_height) > 45.0)
            {
               // can't get there from here...
               return FALSE;
            }

            last_height = curr_height;

            distance = (v_dest - v_check).Length();  // distance from goal
         }

         return TRUE;
      }
   }

   return FALSE;
}


// find the nearest reachable waypoint
int WaypointFindReachable(edict_t *pEntity, float range, int team)
{
   int i, min_index;
   float distance;
   float min_distance;
   TraceResult tr;

   // find the nearest waypoint...

   min_distance = 9999.0;

   for (i=0; i < num_waypoints; i++)
   {
      if (waypoints[i].flags & W_FL_DELETED)
         continue;  // skip any deleted waypoints

      if (waypoints[i].flags & W_FL_AIMING)
         continue;  // skip any aiming waypoints

      // skip this waypoint if it's team specific and teams don't match...
      if ((team != -1) && (waypoints[i].flags & W_FL_TEAM_SPECIFIC) && ((waypoints[i].flags & W_FL_TEAM) != team))
         continue;

      distance = (waypoints[i].origin - pEntity->v.origin).Length();

      if (distance < min_distance)
      {
         // if waypoint is visible from current position (even behind head)...
         UTIL_TraceLine( pEntity->v.origin + pEntity->v.view_ofs, waypoints[i].origin, ignore_monsters, pEntity->v.pContainingEntity, &tr );

         if (tr.flFraction >= 1.0)
         {
            if (WaypointReachable(pEntity->v.origin, waypoints[i].origin, pEntity))
            {
               min_index = i;
               min_distance = distance;
            }
         }
      }
   }

   // if not close enough to a waypoint then just return
   if (min_distance > range)
      return WAYPOINT_NOT_FOUND;

   return min_index;
}


void WaypointPrintInfo(edict_t *pEntity)
{
	char msg[80];
	int index;
	int flags;

	// find the nearest waypoint...
	index = WaypointFindNearest(pEntity, 300.0, -1);

	if (index == WAYPOINT_NOT_FOUND)
		return;

	sprintf(msg,"Waypoint %d of %d total\n", index, num_waypoints);
	ClientPrint(pEntity, HUD_PRINTNOTIFY, msg);

	flags = waypoints[index].flags;

	if (flags & W_FL_TEAM_SPECIFIC)
	{
		if ((flags & W_FL_TEAM) == 0)
			strcpy(msg, "Waypoint is for TEAM 1\n");
		else if ((flags & W_FL_TEAM) == 1)
			strcpy(msg, "Waypoint is for TEAM 2\n");
		else if ((flags & W_FL_TEAM) == 2)
			strcpy(msg, "Waypoint is for TEAM 3\n");
		else if ((flags & W_FL_TEAM) == 3)
			strcpy(msg, "Waypoint is for TEAM 4\n");

		ClientPrint(pEntity, HUD_PRINTNOTIFY, msg);
	}

	if (flags & W_FL_LIFT)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "Bot will wait for lift before approaching\n");

	if (flags & W_FL_LADDER)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "This waypoint is on a ladder\n");

	if (flags & W_FL_DOOR)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "This is a door waypoint\n");

	if (flags & W_FL_HEALTH)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is health near this waypoint\n");

	if (flags & W_FL_ARMOR)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is armor near this waypoint\n");

	if (flags & W_FL_AMMO)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is ammo near this waypoint\n");

	if (flags & W_FL_WEAPON)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is a weapon near this waypoint\n");

	if (flags & W_FL_JUMP)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "Bot will jump here\n");

	if (flags & W_FL_SNIPER)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "This is a sniper waypoint\n");

	if( flags & W_FL_FLAG )
	{
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is a flag near this waypoint\n");

		edict_t *pFlag = FindNearest(waypoints[index].origin, "item_ctfflag");
		ALERT( at_console, "item_ctfflag with body %d, skin %d\n", pFlag->v.body, pFlag->v.skin );
	}

	if( flags & W_FL_FLAG_GOAL )
	{
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is a flag goal near this waypoint\n");

		edict_t *pBase = FindNearest(waypoints[index].origin, "item_ctfbase");
		ALERT( at_console, "item_ctfbase with model %s\n", STRING(pBase->v.model) );
	}

	if( flags & W_FL_OP4_CAPTURE_POINT )
	{
		ClientPrint( pEntity, HUD_PRINTNOTIFY, "There is a capture point near this waypoint\n");
	}

	if (flags & W_FL_PRONE)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "Bot will go prone here\n");

	if (flags & W_FL_TFC_SENTRYGUN)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "Engineers will build a sentry gun here\n");

	if (flags & W_FL_TFC_DISPENSER)
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "Engineers will build a dispenser here\n");

	if( flags & W_FL_NS_HIVE )
	{
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is a hive near this waypoint\n");
	}
	if( flags & W_FL_NS_COMMAND_CHAIR )
	{
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is a command chair near this waypoint\n");
	}

	if( flags & W_FL_DOD_OBJ )
	{
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is an object near this waypoint\n");
	}

	if( flags & W_FL_DOD_CAP )
	{
		ClientPrint(pEntity, HUD_PRINTNOTIFY, "There is a control point near this waypoint\n");
		edict_t *pControlPoint = FindNearest(waypoints[index].origin, "dod_control_point");
		ALERT( at_console, "Body %d\n", pControlPoint->v.body);
	}

	ALERT( at_console, "Distance to waypoint %f\n", (pEntity->v.origin - waypoints[index].origin).Length());
}


void WaypointThink(edict_t *pEntity)
{
   float distance, min_distance;
   Vector start, end;
   int i, index;

   if (g_auto_waypoint)  // is auto waypoint on?
   {
      // find the distance from the last used waypoint
      distance = (last_waypoint - pEntity->v.origin).Length();

      if (distance > 200)
      {
         min_distance = 9999.0;

         // check that no other reachable waypoints are nearby...
         for (i=0; i < num_waypoints; i++)
         {
            if (waypoints[i].flags & W_FL_DELETED)
               continue;

            if (waypoints[i].flags & W_FL_AIMING)
               continue;

            if (WaypointReachable(pEntity->v.origin, waypoints[i].origin, pEntity))
            {
               distance = (waypoints[i].origin - pEntity->v.origin).Length();

               if (distance < min_distance)
                  min_distance = distance;
            }
         }

         // make sure nearest waypoint is far enough away...
         if (min_distance >= 200)
            WaypointAdd(pEntity);  // place a waypoint here
      }
   }

   min_distance = 9999.0;

   if (g_waypoint_on)  // display the waypoints if turned on...
   {
      for (i=0; i < num_waypoints; i++)
      {
         if ((waypoints[i].flags & W_FL_DELETED) == W_FL_DELETED)
            continue;

         distance = (waypoints[i].origin - pEntity->v.origin).Length();

         if (distance < 400)
         {
            if (distance < min_distance)
            {
               index = i; // store index of nearest waypoint
               min_distance = distance;
            }

            if ((wp_display_time[i] + 1.0) < gpGlobals->time)
            {
               if (waypoints[i].flags & W_FL_CROUCH)
               {
                  start = waypoints[i].origin - Vector(0, 0, 17);
                  end = start + Vector(0, 0, 34);
               }
               else if (waypoints[i].flags & W_FL_AIMING)
               {
                  start = waypoints[i].origin + Vector(0, 0, 10);
                  end = start + Vector(0, 0, 14);
               }
               else
               {
                  start = waypoints[i].origin - Vector(0, 0, 34);
                  end = start + Vector(0, 0, 68);
               }

               // draw a blue waypoint
               WaypointDrawBeam(pEntity, start, end, 30, 0, 0, 0, 255, 250, 5);

               wp_display_time[i] = gpGlobals->time;
            }
         }
      }

      // check if waypointing is on...
      if (g_waypoint_on)
      {
         // check if player is close enough to a waypoint and time to draw path...
         if ((min_distance <= 50) && (f_path_time <= gpGlobals->time))
         {
            path_t *p;

            f_path_time = gpGlobals->time + 1.0;

            p = paths[index];

            while (p != nullptr)
            {
               i = 0;

               while (i < MAX_PATH_INDEX)
               {
                  if (p->index[i] != WAYPOINT_NOT_FOUND)
                  {
                     Vector v_src = waypoints[index].origin;
                     Vector v_dest = waypoints[p->index[i]].origin;

                     // draw a white line to this index's waypoint
                     WaypointDrawBeam(pEntity, v_src, v_dest, 10, 2, 250, 250, 250, 200, 10);
                  }

                  i++;
               }

               p = p->next;  // go to next node in linked list
            }
         }
      }
   }
}


void WaypointFloyds(unsigned short *shortest_path, unsigned short *from_to)
{
   unsigned int x, y, z;
   int changed = 1;
   int distance;

   for (y=0; y < route_num_waypoints; y++)
   {
      for (z=0; z < route_num_waypoints; z++)
      {
         from_to[y * route_num_waypoints + z] = z;
      }
   }

   while (changed)
   {
      changed = 0;

      for (x=0; x < route_num_waypoints; x++)
      {
         for (y=0; y < route_num_waypoints; y++)
         {
            for (z=0; z < route_num_waypoints; z++)
            {
               if ((shortest_path[y * route_num_waypoints + x] == WAYPOINT_UNREACHABLE) ||
                   (shortest_path[x * route_num_waypoints + z] == WAYPOINT_UNREACHABLE))
                  continue;

               distance = shortest_path[y * route_num_waypoints + x] +
                          shortest_path[x * route_num_waypoints + z];

               if (distance > WAYPOINT_MAX_DISTANCE)
                  distance = WAYPOINT_MAX_DISTANCE;

               if ((distance < shortest_path[y * route_num_waypoints + z]) ||
                   (shortest_path[y * route_num_waypoints + z] == WAYPOINT_UNREACHABLE))
               {
                  shortest_path[y * route_num_waypoints + z] = distance;
                  from_to[y * route_num_waypoints + z] = from_to[y * route_num_waypoints + x];
                  changed = 1;
               }
            }
         }
      }
   }
}


void WaypointRouteInit()
{
   unsigned int index;
   bool build_matrix[4];
   int matrix;
   unsigned int array_size;
   unsigned int row;
   int i, offset;
   unsigned int a, b;
   float distance;
   unsigned short *pShortestPath, *pFromTo;
   char msg[80];
   unsigned int num_items;
   FILE *bfp;
   char filename[256];
   char filename2[256];
   char mapname[64];

   if (num_waypoints == 0)
      return;

   // save number of current waypoints in case waypoints get added later
   route_num_waypoints = num_waypoints;

   strcpy(mapname, STRING(gpGlobals->mapname));
   strcat(mapname, ".wpt");

   UTIL_BuildFileName(filename, "maps", mapname);

   build_matrix[0] = TRUE;  // always build matrix 0 (non-team and team 1)
   build_matrix[1] = FALSE;
   build_matrix[2] = FALSE;
   build_matrix[3] = FALSE;

   // find out how many route matrixes to create...
   for (index=0; index < route_num_waypoints; index++)
   {
      if (waypoints[index].flags & W_FL_TEAM_SPECIFIC)
      {
		  if ((waypoints[index].flags & W_FL_TEAM) == 0x01)  // team 2?
		  {
			  build_matrix[1] = TRUE;
		  }

		  if ((waypoints[index].flags & W_FL_TEAM) == 0x02)  // team 3?
		  {
			  build_matrix[2] = TRUE;
		  }

		  if ((waypoints[index].flags & W_FL_TEAM) == 0x03)  // team 4?
		  {
			  build_matrix[3] = TRUE;
		  }
      }
   }

   array_size = route_num_waypoints * route_num_waypoints;

   for (matrix=0; matrix < 4; matrix++)
   {
      if (build_matrix[matrix])
      {
         char ext_str[16];  // ".wpX\0"
         int file1, file2;
         struct stat stat1, stat2;

         sprintf(ext_str, ".wp%d", matrix+1);

         strcpy(mapname, STRING(gpGlobals->mapname));
         strcat(mapname, ext_str);

         UTIL_BuildFileName(filename2, "maps", mapname);

         if (access(filename2, 0) == 0)  // does the .wpX file exist?
         {
            file1 = open(filename, O_RDONLY);
            file2 = open(filename2, O_RDONLY);

            fstat(file1, &stat1);
            fstat(file2, &stat2);

            close(file1);
            close(file2);

            if (stat1.st_mtime < stat2.st_mtime)  // is .wpt older than .HPB_wpX file?
            {
               sprintf(msg, "loading HPB bot waypoint paths for team %d\n", matrix+1);
               ALERT(at_console, msg);

               shortest_path[matrix] = (unsigned short *)malloc(sizeof(unsigned short) * array_size);

               if (shortest_path[matrix] == NULL)
                  ALERT(at_error, "Error allocating memory for shortest path!\n");

               from_to[matrix] = (unsigned short *)malloc(sizeof(unsigned short) * array_size);

               if (from_to[matrix] == NULL)
                  ALERT(at_error, "Error allocating memory for from to matrix!\n");

               bfp = fopen(filename2, "rb");

               if (bfp != NULL)
               {
                  num_items = fread(shortest_path[matrix], sizeof(unsigned short), array_size, bfp);

                  if (num_items != array_size)
                  {
                     // if couldn't read enough data, free memory to recalculate it

                     ALERT(at_console, "error reading enough path items, recalculating...\n");

                     free(shortest_path[matrix]);
                     shortest_path[matrix] = NULL;

                     free(from_to[matrix]);
                     from_to[matrix] = NULL;
                  }
                  else
                  {
                     num_items = fread(from_to[matrix], sizeof(unsigned short), array_size, bfp);

                     if (num_items != array_size)
                     {
                        // if couldn't read enough data, free memory to recalculate it

                        ALERT(at_console, "error reading enough path items, recalculating...\n");

                        free(shortest_path[matrix]);
                        shortest_path[matrix] = NULL;

                        free(from_to[matrix]);
                        from_to[matrix] = NULL;
                     }
                  }
               }
               else
               {
                  ALERT(at_console, "HPB_bot - Error reading waypoint paths!\n");

                  free(shortest_path[matrix]);
                  shortest_path[matrix] = NULL;

                  free(from_to[matrix]);
                  from_to[matrix] = NULL;
               }

               fclose(bfp);
            }
         }

         if (shortest_path[matrix] == NULL)
         {
            sprintf(msg, "calculating HPB bot waypoint paths for team %d...\n", matrix+1);
            ALERT(at_console, msg);

            shortest_path[matrix] = (unsigned short *)malloc(sizeof(unsigned short) * array_size);

            if (shortest_path[matrix] == NULL)
               ALERT(at_error, "Error allocating memory for shortest path!\n");

            from_to[matrix] = (unsigned short *)malloc(sizeof(unsigned short) * array_size);

            if (from_to[matrix] == NULL)
               ALERT(at_error, "Error allocating memory for from to matrix!\n");

            pShortestPath = shortest_path[matrix];
            pFromTo = from_to[matrix];

            for (index=0; index < array_size; index++)
               pShortestPath[index] = WAYPOINT_UNREACHABLE;

            for (index=0; index < route_num_waypoints; index++)
               pShortestPath[index * route_num_waypoints + index] = 0;  // zero diagonal

            for (row=0; row < route_num_waypoints; row++)
            {
               if (paths[row] != NULL)
               {
                  path_t *p = paths[row];

                  while (p)
                  {
                     i = 0;

                     while (i < MAX_PATH_INDEX)
                     {
                        if (p->index[i] != WAYPOINT_NOT_FOUND)
                        {
                           index = p->index[i];

                           // check if this is NOT team specific OR matches this team
                           if (!(waypoints[index].flags & W_FL_TEAM_SPECIFIC) ||
                               ((waypoints[index].flags & W_FL_TEAM) == matrix))
                           {
                              distance = (waypoints[row].origin - waypoints[index].origin).Length();

                              if (distance > (float)WAYPOINT_MAX_DISTANCE)
                                 distance = (float)WAYPOINT_MAX_DISTANCE;

                              if (distance > REACHABLE_RANGE)
                              {
                                 sprintf(msg, "Waypoint path distance of %f > %4.1f at from %d to %d\n", distance, REACHABLE_RANGE, row, index);
                                 ALERT(at_aiconsole, msg);
                              }
                              else
                              {
                                 offset = row * route_num_waypoints + index;

                                 pShortestPath[offset] = (unsigned short)distance;
                              }
                           }
                        }

                        i++;
                     }

                     p = p->next;  // go to next node in linked list
                  }
               }
            }

            // run Floyd's Algorithm to generate the from_to matrix...
            WaypointFloyds(pShortestPath, pFromTo);

            for (a=0; a < route_num_waypoints; a++)
            {
               for (b=0; b < route_num_waypoints; b++)
                  if (pShortestPath[a * route_num_waypoints + b] == WAYPOINT_UNREACHABLE)
                    pFromTo[a * route_num_waypoints + b] = WAYPOINT_UNREACHABLE;
            }

            bfp = fopen(filename2, "wb");

            if (bfp != NULL)
            {
               num_items = fwrite(shortest_path[matrix], sizeof(unsigned short), array_size, bfp);

               if (num_items != array_size)
               {
                  // if couldn't write enough data, close file and delete it

                  fclose(bfp);
                  unlink(filename2);
               }
               else
               {
                  num_items = fwrite(from_to[matrix], sizeof(unsigned short), array_size, bfp);

                  fclose(bfp);

                  if (num_items != array_size)
                  {
                     // if couldn't write enough data, delete file
                     unlink(filename2);
                  }
               }
            }
            else
            {
               ALERT(at_console, "HPB_bot - Error writing waypoint paths!\n");
            }

            sprintf(msg, "HPB bot waypoint path calculations for team %d complete!\n",matrix+1);
            ALERT(at_console, msg);
         }
      }
   }

}


unsigned short WaypointRouteFromTo(int src, int dest, int team)
{
   unsigned short *pFromTo;

   if ((team < -1) || (team > 3))
      return -1;

   if (team == -1)  // -1 means non-team play
      team = 0;

   if (from_to[team] == NULL)  // if no team specific waypoints use team 0
      team = 0;

   if (from_to[team] == NULL)  // if no route information just return
      return -1;

   pFromTo = from_to[team];

   return pFromTo[src * route_num_waypoints + dest];
}


int WaypointDistanceFromTo(int src, int dest, int team)
{
   unsigned short *pShortestPath;

   if ((team < -1) || (team > 3))
      return -1;

   if (team == -1)  // -1 means non-team play
      team = 0;

   if (from_to[team] == NULL)  // if no team specific waypoints use team 0
      team = 0;

   if (from_to[team] == NULL)  // if no route information just return
      return -1;

   pShortestPath = shortest_path[team];

   return (int)(pShortestPath[src * route_num_waypoints + dest]);
}
