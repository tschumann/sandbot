#ifndef WPT_H
#define WPT_H

#ifdef _MSC_VER
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#define WAYPOINT_HEADER "Sandbot"
const int WAYPOINT_VERSION = 1;

// define the waypoint file header structure
typedef struct {
   char filetype[8];
   int  waypoint_file_version;
   int  waypoint_file_flags;
   int  number_of_waypoints;
   char mapname[32]; // name of map for these waypoints
} WAYPOINT_HDR;

// define the structure for waypoints
typedef struct {
   uint64_t flags; // button, lift, flag, health, ammo, etc.
   Vector origin; // location
#if __GNUC__
   uint32_t iPack; // because Visual Studio and gcc do layout slightly differently
#endif
} WAYPOINT;

#endif // WPT_H
