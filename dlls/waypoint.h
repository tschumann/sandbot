//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// waypoint.h
//

#ifndef WAYPOINT_H
#define WAYPOINT_H

#ifdef _MSC_VER
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#include <limits.h>

const int WAYPOINT_NOT_FOUND = -1;

#define MAX_WAYPOINTS 2048

#define REACHABLE_RANGE 400.0

// defines for waypoint flags field (32 bits are available)
const int W_FL_TEAM = ((1<<0) + (1<<1)); // allow for 4 teams (0-3)
const int W_FL_TEAM_SPECIFIC = (1<<2); // waypoint only for specified team
const int W_FL_CROUCH = (1<<3); // must crouch to reach this waypoint
const int W_FL_LADDER = (1<<4); // waypoint on a ladder
const int W_FL_LIFT = (1<<5); // wait for lift to be down before approaching this waypoint
const int W_FL_DOOR = (1<<6); // wait for door to open
const int W_FL_HEALTH = (1<<7); // health kit (or wall mounted) location
const int W_FL_ARMOR = (1<<8); // armor (or HEV) location
const int W_FL_AMMO = (1<<9); // ammo location
const int W_FL_SNIPER = (1<<10); // sniper waypoint (a good sniper spot)

const int W_FL_FLAG = (1<<11); // flag position (or hostage or president)
const int W_FL_TFC_FLAG = (1<<11); // flag position (or hostage or president)

const int W_FL_FLAG_GOAL = (1<<12); // flag return position (or rescue zone)
const int W_FL_TFC_FLAG_GOAL = (1<<12); // flag return position (or rescue zone)

const int W_FL_PRONE = (1<<13); // go prone (laying down)
const int W_FL_AIMING = (1<<14); // aiming waypoint

const int W_FL_TFC_SENTRYGUN = (1<<15); // sentry gun waypoint for TFC
const int W_FL_TFC_DISPENSER = (1<<16); // dispenser waypoint for TFC

const int W_FL_WEAPON = (1<<17); // weapon_ entity location
const int W_FL_JUMP = (1<<18); // jump waypoint

const int W_FL_CAPTURE_POINT = (1<<19); // Opposing Force CTF capture point

const int W_FL_NS_HIVE = (1<<20);
const int W_FL_NS_COMMAND_CHAIR = (1<<21);
const int W_FL_NS_RESNODE = (1<<22);

const int W_FL_TFC_BUILD_ZONE = (1<<25);

const int W_FL_DOD_OBJ = (1<<29);
const int W_FL_DOD_CAP = (1<<30);

const int W_FL_DELETED = (1<<31); // used by waypoint allocation code


#define WAYPOINT_HEADER "Sandbot"
const int WAYPOINT_VERSION = 1;

// define the waypoint file header structure...
typedef struct {
   char filetype[8];
   int  waypoint_file_version;
   int  waypoint_file_flags;
   int  number_of_waypoints;
   char mapname[32];	// name of map for these waypoints
} WAYPOINT_HDR;

// define the structure for waypoints...
typedef struct {
   uint64_t flags;	// button, lift, flag, health, ammo, etc.
   // int unused;
   Vector origin;	// location
#if __GNUC__
   uint32_t iPack;
#endif
} WAYPOINT;


#define WAYPOINT_UNREACHABLE   USHRT_MAX
#define WAYPOINT_MAX_DISTANCE (USHRT_MAX-1)

#define MAX_PATH_INDEX 4

// define the structure for waypoint paths (paths are connections between
// two waypoint nodes that indicates the bot can get from point A to point B.
// note that paths DON'T have to be two-way.  sometimes they are just one-way
// connections between two points.  There is an array called "paths" that
// contains head pointers to these structures for each waypoint index.
typedef struct path {
   int16_t index[MAX_PATH_INDEX];  // indexes of waypoints (index -1 means not used)
   struct path *next;   // link to next structure
} PATH;


// waypoint function prototypes...
void WaypointInit(void);
int  WaypointFindPath(PATH **pPath, int *path_index, int waypoint_index, int team);
int  WaypointFindNearest(edict_t *pEntity, float distance, int team);
int  WaypointFindNearest(edict_t *pEntity, float range, int team, Vector v_src);
int  WaypointFindNearest(edict_t *pEntity, float range, int team, Vector v_src, uint64_t flags);

int  WaypointFindNearestGoal(edict_t *pEntity, int src, int team, uint64_t flags);
int  WaypointFindNearestGoal(edict_t *pEntity, int src, int team, uint64_t flags, int exclude[]);
int  WaypointFindNearestGoal(Vector v_src, edict_t *pEntity, float range, int team, uint64_t flags);

int  WaypointFindRandomGoal(edict_t *pEntity, int team, uint64_t flags);
int  WaypointFindRandomGoal(edict_t *pEntity, int team, uint64_t flags, int exclude[]);
int  WaypointFindRandomGoal(Vector v_src, edict_t *pEntity, float range, int team, uint64_t flags);
int  WaypointFindNearestAiming(Vector v_origin);
int  WaypointFindNearestWaypoint(edict_t *pEntity, uint64_t type);
void WaypointSearchItems(edict_t *pEntity, Vector origin, int wpt_index);
void WaypointAdd(edict_t *pEntity, int flags);
void WaypointAddAiming(edict_t *pEntity);
void WaypointDelete(edict_t *pEntity);
void WaypointUpdate(edict_t *pEntity);
void WaypointCreatePath(edict_t *pEntity, int cmd);
void WaypointRemovePath(edict_t *pEntity, int cmd);
bool WaypointLoad(edict_t *pEntity);
void WaypointSave(void);
bool WaypointReachable(Vector v_srv, Vector v_dest, edict_t *pEntity);
int  WaypointFindReachable(edict_t *pEntity, float range, int team);
void WaypointPrintInfo(edict_t *pEntity);
void WaypointThink(edict_t *pEntity);
void WaypointFloyds(unsigned short *shortest_path, unsigned short *from_to);
void WaypointRouteInit(void);
unsigned short WaypointRouteFromTo(int src, int dest, int team);
int  WaypointDistanceFromTo(int src, int dest, int team);

bool ShouldSkip(edict_t *pPlayer, int index);

float DistanceToNearest(Vector point, const char *szClassname);

#endif // WAYPOINT_H
