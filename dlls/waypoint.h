//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// waypoint.h
//

#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <limits.h>

const int WAYPOINT_NOT_FOUND = -1;

const float REACHABLE_RANGE = 400.0;

extern int num_waypoints; // number of waypoints currently in use
extern waypoint_t waypoints[MAX_WAYPOINTS];

#define WAYPOINT_UNREACHABLE   USHRT_MAX
#define WAYPOINT_MAX_DISTANCE (USHRT_MAX-1)

// waypoint function prototypes...
void WaypointInit();
int  WaypointFindPath(path_t **pPath, int *path_index, int waypoint_index, int team);
int  WaypointFindNearest(const edict_t *pEntity, float distance, int team);
int  WaypointFindNearest(const edict_t *pEntity, float range, int team, const Vector& v_src);
int  WaypointFindNearest(const edict_t *pEntity, float range, int team, const Vector& v_src, uint64_t flags);

int  WaypointFindNearestGoal(const edict_t *pEntity, int src, int team, uint64_t flags);
int  WaypointFindNearestGoal(const edict_t *pEntity, int src, int team, uint64_t flags, int exclude[]);
int  WaypointFindNearestGoal(const Vector& v_src, const edict_t *pEntity, float range, int team, uint64_t flags);

int  WaypointFindRandomGoal(const edict_t *pEntity, int team, uint64_t flags);
int  WaypointFindRandomGoal(const edict_t *pEntity, int team, uint64_t flags, int exclude[]);
int  WaypointFindRandomGoal(const Vector& v_src, const edict_t *pEntity, float range, int team, uint64_t flags);
int  WaypointFindNearestAiming(const Vector& v_origin);
int  WaypointFindNearestWaypoint(const edict_t *pEntity, uint64_t type);
void WaypointSearchItems(edict_t *pEntity, const Vector& origin, int wpt_index);
void WaypointAdd(edict_t *pEntity, int flags);
void WaypointAddAiming(edict_t *pEntity);
void WaypointDelete(edict_t *pEntity);
void WaypointUpdate(const edict_t *pEntity);
void WaypointCreatePath(edict_t *pEntity, int cmd);
void WaypointRemovePath(edict_t *pEntity, int cmd);
bool WaypointLoad(edict_t *pEntity);
void WaypointSave();
bool WaypointReachable(const Vector& v_srv, const Vector& v_dest, const edict_t *pEntity);
int  WaypointFindReachable(edict_t *pEntity, float range, int team);
void WaypointPrintInfo(edict_t *pEntity);
void WaypointThink(edict_t *pEntity);
void WaypointFloyds(unsigned short *shortest_path, unsigned short *from_to);
void WaypointRouteInit();
unsigned short WaypointRouteFromTo(int src, int dest, int team);
int  WaypointDistanceFromTo(int src, int dest, int team);

bool ShouldSkip( const edict_t *pPlayer, int index );

float DistanceToNearest(const Vector& point, const char *szClassname);

#endif // WAYPOINT_H
