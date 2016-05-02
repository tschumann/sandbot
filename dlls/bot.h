//
// gbot - The GoldSource bot
//
// <no site>
//
// bot.h
//

#ifndef BOT_H
#define BOT_H

#include "studio.h"

#define PREVIOUS_WAYPOINTS 10

// stuff for Windows vs Linux builds
typedef int (FAR *GETENTITYAPI)( DLL_FUNCTIONS *, int );
typedef int (FAR *GETENTITYAPI2)( DLL_FUNCTIONS *, int * );
typedef int (FAR *GETNEWDLLFUNCTIONS)( NEW_DLL_FUNCTIONS *, int * );
typedef void (DLLEXPORT *GIVEFNPTRSTODLL)( enginefuncs_t *, globalvars_t * );
typedef int (*SERVER_GETBLENDINGINTERFACE)( int, struct sv_blending_interface_s **, struct engine_studio_api_s *, float (*)[3][4], float (*)[MAXSTUDIOBONES][3][4] );
typedef void (*SV_SAVEGAMECOMMENT)( char *, int );
typedef void (FAR *LINK_ENTITY_FUNC)( entvars_t * );

// identifying constants for supported mods
#define VALVE_DLL		1
#define GEARBOX_DLL		2
#define CSTRIKE_DLL		3
#define DOD_DLL			4
#define TFC_DLL			5
#define GUNMAN_DLL		6
#define HUNGER_DLL		7
#define NS_DLL			8
#define SHIP_DLL		9


// define some function prototypes...
BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] );
void ClientPutInServer( edict_t *pEntity );
void ClientCommand( edict_t *pEntity );

void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);


const char *Cmd_Args( void );
const char *Cmd_Argv( int argc );
int Cmd_Argc( void );


#define LADDER_UNKNOWN  0
#define LADDER_UP         1
#define LADDER_DOWN      2

#define WANDER_LEFT  1
#define WANDER_RIGHT 2

#define BOT_PITCH_SPEED 20
#define BOT_YAW_SPEED 20

// Natural Selection
#define TEAM_NONE	0
#define TEAM_MARINE	1
#define TEAM_ALIEN	2

#define kGorgeCost 10
#define kLerkCost 30
#define kFadeCost 50
#define kOnosCost 75

#define kAlienResourceTowerCost 15
#define kHiveCost 40

// Only one of these allowed per entity, stored in pev->iuser3.
typedef enum
{
	AVH_USER3_NONE = 0,
	AVH_USER3_MARINE_PLAYER,
	AVH_USER3_COMMANDER_PLAYER,
	AVH_USER3_ALIEN_PLAYER1,
	AVH_USER3_ALIEN_PLAYER2,
	AVH_USER3_ALIEN_PLAYER3,
	AVH_USER3_ALIEN_PLAYER4,
	AVH_USER3_ALIEN_PLAYER5,
	AVH_USER3_ALIEN_EMBRYO,
	AVH_USER3_SPAWN_TEAMONE,
	AVH_USER3_SPAWN_TEAMTWO,
	AVH_USER3_PARTICLE_ON,				// only valid for AvHParticleEntity: entindex as int in fuser1, template index stored in fuser2
	AVH_USER3_PARTICLE_OFF,				// only valid for AvHParticleEntity: particle system handle in fuser1
	AVH_USER3_WELD,						// float progress (0 - 100) stored in fuser1
	AVH_USER3_ALPHA,					// fuser1 indicates how much alpha this entity toggles to in commander mode, fuser2 for players
	AVH_USER3_MARINEITEM,				// Something a friendly marine can pick up
	AVH_USER3_WAYPOINT,
	AVH_USER3_HIVE,
	AVH_USER3_NOBUILD,
	AVH_USER3_USEABLE,
	AVH_USER3_AUDIO_ON,
	AVH_USER3_AUDIO_OFF,
	AVH_USER3_FUNC_RESOURCE,
	AVH_USER3_COMMANDER_STATION,
	AVH_USER3_TURRET_FACTORY, 
	AVH_USER3_ARMORY, 
	AVH_USER3_ADVANCED_ARMORY,
	AVH_USER3_ARMSLAB,
	AVH_USER3_PROTOTYPE_LAB, 
	AVH_USER3_OBSERVATORY,
	AVH_USER3_CHEMLAB,
	AVH_USER3_MEDLAB,
	AVH_USER3_NUKEPLANT,
	AVH_USER3_TURRET,
	AVH_USER3_SIEGETURRET,
	AVH_USER3_RESTOWER,
	AVH_USER3_PLACEHOLDER,
	AVH_USER3_INFANTRYPORTAL,
	AVH_USER3_NUKE,
	AVH_USER3_BREAKABLE,
	AVH_USER3_UMBRA,
	AVH_USER3_PHASEGATE,
	AVH_USER3_DEFENSE_CHAMBER,
	AVH_USER3_MOVEMENT_CHAMBER,
	AVH_USER3_OFFENSE_CHAMBER,
	AVH_USER3_SENSORY_CHAMBER,
	AVH_USER3_ALIENRESTOWER,
	AVH_USER3_HEAVY,
	AVH_USER3_JETPACK,
	AVH_USER3_ADVANCED_TURRET_FACTORY,
	AVH_USER3_SPAWN_READYROOM,
	AVH_USER3_CLIENT_COMMAND,
	AVH_USER3_FUNC_ILLUSIONARY,
	AVH_USER3_MENU_BUILD,
	AVH_USER3_MENU_BUILD_ADVANCED,
	AVH_USER3_MENU_ASSIST,
	AVH_USER3_MENU_EQUIP,
	AVH_USER3_MINE,
	AVH_USER3_UNKNOWN,
	AVH_USER3_MAX
} AvHUser3;

typedef enum
{
	MASK_NONE = 0,
	MASK_VIS_SIGHTED = 1,					// This means this is an entity that can be seen by at least one member of the opposing team.  Assumes commanders can never be seen.
	MASK_VIS_DETECTED = 2,					// This entity has been detected by the other team but isn't currently seen
	MASK_BUILDABLE = 4,						// This entity is buildable
	MASK_UPGRADE_1 = 8,						// Marine weapons 1, armor, marine basebuildable slot #0
	MASK_UPGRADE_2 = 16,					// Marine weapons 2, regen, marine basebuildable slot #1
	MASK_UPGRADE_3 = 32,					// Marine weapons 3, redemption, marine basebuildable slot #2
	MASK_UPGRADE_4 = 64,					// Marine armor 1, speed, marine basebuildable slot #3
	MASK_UPGRADE_5 = 128,					// Marine armor 2, adrenaline, marine basebuildable slot #4
	MASK_UPGRADE_6 = 256,					// Marine armor 3, silence, marine basebuildable slot #5
	MASK_UPGRADE_7 = 512,					// Marine jetpacks, Cloaking, marine basebuildable slot #6
	MASK_UPGRADE_8 = 1024,					// Pheromone, motion-tracking, marine basebuildable slot #7
	MASK_UPGRADE_9 = 2048,					// Scent of fear, exoskeleton
	MASK_UPGRADE_10 = 4096,					// Defensive level 2, power armor
	MASK_UPGRADE_11 = 8192,					// Defensive level 3, electrical defense
	MASK_UPGRADE_12 = 16384,				// Movement level 2, 
	MASK_UPGRADE_13 = 32768,				// Movement level 3, marine heavy armor
	MASK_UPGRADE_14 = 65536,				// Sensory level 2
	MASK_UPGRADE_15 = 131072,				// Sensory level 3
	MASK_ALIEN_MOVEMENT = 262144,			// Onos is charging
	MASK_WALLSTICKING = 524288,				// Flag for wall-sticking
	MASK_BUFFED = 1048576,                  // Alien is in range of active primal scream, or marine is under effects of catalyst
	MASK_UMBRA = 2097152,
	MASK_DIGESTING = 4194304,				// When set on a visible player, player is digesting.  When set on invisible player, player is being digested
	MASK_RECYCLING = 8388608,
	MASK_TOPDOWN = 16777216,
	MASK_PLAYER_STUNNED = 33554432,			// Player has been stunned by stomp
	MASK_ENSNARED = 67108864,
	MASK_ALIEN_EMBRYO = 134217728,
	MASK_SELECTABLE = 268435456,
	MASK_PARASITED = 536870912,
	MASK_SENSORY_NEARBY = 1073741824
} AvHUser4;

typedef enum
{
	PLAYMODE_UNDEFINED = 0,
	PLAYMODE_READYROOM = 1,
	PLAYMODE_PLAYING = 2,
	PLAYMODE_AWAITINGREINFORCEMENT = 3,	// Player is dead and waiting in line to get back in
	PLAYMODE_REINFORCING = 4,			// Player is in the process of coming back into the game
	PLAYMODE_OBSERVER = 5,
	PLAYMODE_REINFORCINGCOMPLETE = 6	// Combat only: 'press fire to respawn'
} AvHPlayMode;

// Team Fortress Classic
#define PC_SCOUT		1 
#define PC_SNIPER		2 
#define PC_SOLDIER		3 
#define PC_DEMOMAN		4 
#define PC_MEDIC		5 
#define PC_HVYWEAP		6 
#define PC_PYRO			7
#define PC_SPY			8
#define PC_ENGINEER		9

#define RESPAWN_IDLE			1
#define RESPAWN_NEED_TO_RESPAWN	2
#define RESPAWN_IS_RESPAWNING	3

#define BOT_SKIN_LEN 32
#define BOT_NAME_LEN 32

#define MAX_BOT_WHINE 100

struct bot_player_t
{
	char szName[32];
	char szModel[16];
	BOOL bIsUsed;
};

typedef struct
{
    int  iId;      // weapon ID
    int  iClip;    // amount of ammo in the clip
    int  iAmmo1;  // amount of ammo in primary reserve
    int  iAmmo2;  // amount of ammo in secondary reserve
} bot_current_weapon_t;

// cust_
#define PISTOL_PULSE		11
#define PISTOL_CHARGE		12
#define PISTOL_RAPID		13
#define PISTOL_SNIPER		14

#define SHOTGUN_RIOTGUN		13
#define SHOTGUN_SHOTGUN		12
#define SHOTGUN_RIFLE		11


typedef struct
{
    bool is_used;
    int respawn_state;
    edict_t *pEdict;
    bool need_to_initialize;
    char name[BOT_NAME_LEN+1];
    int bot_skill;
    int not_started;
    int start_action;
    float kick_time;
    float create_time;

    // things from pev in CBasePlayer...
    int bot_team;
    int bot_class;

    float idle_angle;
    float idle_angle_time;  // for Front Line Force
    int round_end;          // round has ended (in round based games)
    float blinded_time;

    float f_max_speed;
    float prev_speed;
    float prev_time;
    Vector v_prev_origin;

    float f_find_item;
    edict_t *pBotPickupItem;

    int ladder_dir;
    float f_start_use_ladder_time;
    float f_end_use_ladder_time;
    bool  waypoint_top_of_ladder;

    float f_wall_check_time;
    float f_wall_on_right;
    float f_wall_on_left;
    float f_dont_avoid_wall_time;
    float f_look_for_waypoint_time;
    float f_jump_time;
    float f_dont_check_stuck;

    int wander_dir;
    float f_exit_water_time;

    Vector waypoint_origin;
    float f_waypoint_time;
    int curr_waypoint_index;
    int prev_waypoint_index[PREVIOUS_WAYPOINTS];
    float f_random_waypoint_time;
    int waypoint_goal;
    float f_waypoint_goal_time;
    bool waypoint_near_flag;
    Vector waypoint_flag_origin;
    float prev_waypoint_distance;

    edict_t *pBotEnemy;
	edict_t *pBotFriend;
    float f_bot_see_enemy_time;
    float f_bot_find_enemy_time;
    edict_t *pBotUser;
    float f_bot_use_time;
    float f_bot_spawn_time;
    edict_t *killer_edict;
    float f_sniper_aim_time;
    

    float f_shoot_time;
    float f_primary_charging;
    float f_secondary_charging;
    int    charging_weapon_id;
    float f_move_speed;
    float f_pause_time;
    float f_sound_update_time;
    bool  bot_has_flag;

    bool  b_see_tripmine;
    bool  b_shoot_tripmine;
    Vector v_tripmine;

    bool bUseHealthCharger;
    float fUseHealthChargerTime;

    bool bUseArmorCharger;
    float fUseHEVTime;

    bool  b_use_button;
    float f_use_button_time;
    bool  b_lift_moving;

	// Gunman Chronicles
	bool bFists;
	int iPistolMode;
	int iShotgunMode;
	bool bMinigunSpin;

	// Natural Selection
	bool bUseArmory;
	float fUseArmoryTime;

	bool bBuild;
	float fBuildTime;
	bool bBuildAlienResourceTower;
	bool bBuildHive;

	int iClass;
	bool bEvolving;
	bool bEvolved;

	// The Ship
	bool bUseDoor;
	float fUseDoorTime;

    bot_current_weapon_t current_weapon;  // one current weapon for each bot
    int m_rgAmmo[MAX_AMMO_SLOTS];  // total ammo amounts (1 array for each bot)

} bot_t;


#define MAX_TEAMS 32
#define MAX_TEAMNAME_LENGTH 16


// new UTIL.CPP functions...
edict_t *UTIL_FindEntityInSphere( edict_t *pentStart, const Vector &vecCenter, float flRadius );
edict_t *UTIL_FindEntityByString( edict_t *pentStart, const char *szKeyword, const char *szValue );
edict_t *UTIL_FindEntityByClassname( edict_t *pentStart, const char *szName );
edict_t *UTIL_FindEntityByTargetname( edict_t *pentStart, const char *szName );
void ClientPrint( edict_t *pEdict, int msg_dest, const char *msg_name);
void UTIL_SayText( const char *pText, edict_t *pEdict );
void UTIL_HostSay( edict_t *pEntity, int teamonly, char *message );
int UTIL_GetTeam(edict_t *pEntity);
int UTIL_GetClass(edict_t *pEntity);
int UTIL_GetBotIndex(edict_t *pEdict);
bot_t *UTIL_GetBotPointer(edict_t *pEdict);
bool IsAlive(edict_t *pEdict);
bool FInViewCone(Vector *pOrigin, edict_t *pEdict);
bool FVisible( const Vector &vecOrigin, edict_t *pEdict );
Vector Center(edict_t *pEdict);
Vector GetGunPosition(edict_t *pEdict);
void UTIL_SelectItem(edict_t *pEdict, char *item_name);
Vector VecBModelOrigin(edict_t *pEdict);
bool UpdateSounds(edict_t *pEdict, edict_t *pPlayer);
void UTIL_ShowMenu( edict_t *pEdict, int slots, int displaytime, bool needmore, char *pText );
void UTIL_BuildFileName(char *filename, char *arg1, char *arg2);


#endif // BOT_H

