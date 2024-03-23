//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes:
//  - .wpt file format - this file must be included before waypoint.h
//  - don't use preprocessor macros for the symbols as they get namespaced in sandbot-wptgen
//
//=============================================================================

#ifndef _WPT_H_
#define _WPT_H_

#include <stddef.h>

#ifdef _MSC_VER
typedef __int16 int16_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

const char WAYPOINT_HEADER[8] = "Sandbot";
const int WAYPOINT_VERSION = 1;

// defines for waypoint flags field (32 bits are available)
const int W_FL_TEAM = ((1 << 0) + (1 << 1)); // allow for 4 teams (0-3)
const int W_FL_TEAM_SPECIFIC = (1 << 2); // waypoint only for specified team
const int W_FL_CROUCH = (1 << 3); // must crouch to reach this waypoint
const int W_FL_LADDER = (1 << 4); // waypoint on a ladder
const int W_FL_LIFT = (1 << 5); // wait for lift to be down before approaching this waypoint
const int W_FL_DOOR = (1 << 6); // wait for door to open
const int W_FL_HEALTH = (1 << 7); // health kit (or wall mounted) location
const int W_FL_ARMOR = (1 << 8); // armor (or HEV) location
const int W_FL_AMMO = (1 << 9); // ammo location
const int W_FL_SNIPER = (1 << 10); // sniper waypoint (a good sniper spot)

const int W_FL_FLAG = (1 << 11); // flag position (or hostage or president)
const int W_FL_TFC_FLAG = (1 << 11); // flag position (or hostage or president)

const int W_FL_FLAG_GOAL = (1 << 12); // flag return position (or rescue zone)
const int W_FL_TFC_FLAG_GOAL = (1 << 12); // flag return position (or rescue zone)

const int W_FL_PRONE = (1 << 13); // go prone (laying down)
const int W_FL_AIMING = (1 << 14); // aiming waypoint

const int W_FL_TFC_SENTRYGUN = (1 << 15); // sentry gun waypoint for TFC
const int W_FL_TFC_DISPENSER = (1 << 16); // dispenser waypoint for TFC

const int W_FL_WEAPON = (1 << 17); // weapon_ entity location
const int W_FL_JUMP = (1 << 18); // jump waypoint

const int W_FL_NS_HIVE = (1 << 20);
const int W_FL_NS_COMMAND_CHAIR = (1 << 21);
const int W_FL_NS_RESNODE = (1 << 22);

const int W_FL_TFC_BUILD_ZONE = (1 << 25); // Team Fortress Classic engineer build zone

const int W_FL_OP4_CAPTURE_POINT = (1 << 27); // Opposing Force CTF capture point

const int W_FL_DOD_OBJ = (1 << 29); // Day of Defeat objective
const int W_FL_DOD_CAP = (1 << 30); // Day of Defeat capture point

const int W_FL_DELETED = (1 << 31); // used by waypoint allocation code

// define the waypoint file header structure
typedef struct {
	char filetype[8];
	int  waypoint_file_version; // not used
	int  waypoint_file_flags;
	int  number_of_waypoints;
	char mapname[32]; // name of map for these waypoints
} WAYPOINT_HDR;

const int MAX_WAYPOINTS = 2048;

// define the structure for waypoints
typedef struct {
	uint64_t flags; // button, lift, flag, health, ammo, etc.
	Vector origin; // location
#if __GNUC__
	uint32_t iPack; // because Visual Studio and gcc do layout slightly differently
#endif
} waypoint_t;

const int MAX_PATH_INDEX = 4;

// define the structure for waypoint paths (paths are connections between
// two waypoint nodes that indicates the bot can get from point A to point B.
// note that paths DON'T have to be two-way.  sometimes they are just one-way
// connections between two points.  There is an array called "paths" that
// contains head pointers to these structures for each waypoint index.
typedef struct path_s {
	int16_t index[MAX_PATH_INDEX]; // indexes of waypoints (index -1 means not used)
	struct path_s* next; // link to next structure
} path_t;

// only supported in newer versions of C++ (which Visual Studio doesn't say it supports, but is only supported by
// Visual Studio 2010 onwards: https://msdn.microsoft.com/en-us/library/dd293588.aspx)
#if (_MSC_VER >= 1600) || (__cplusplus >= 201103L)
// compiler-specific packing sucks - some day it should all be 0 packing but I don't want to recreate the waypoint files
static_assert(sizeof(waypoint_t) == 24, "waypoint_t should be 24 bytes");
static_assert(sizeof(Vector) == 12, "Vector should be 12 bytes");
static_assert(offsetof(waypoint_t, origin) == 8, "origin should be 8 bytes into waypoint_t");

// no need to assert size of path_t as the whole structure isn't written to a file - just need to make sure the size of index[] is as expected
static_assert(offsetof(path_t, next) == 8, "next should be 8 bytes into PATH");
#else
#error No checking of waypoint structure layouts - there may be problems
#endif

#endif // _WPT_H_
