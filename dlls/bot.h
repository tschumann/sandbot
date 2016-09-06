//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot.h
//

#ifndef BOT_H
#define BOT_H

#include <algorithm>
#include <vector>

using std::vector;
using std::min;

// stuff for Win32 vs. Linux builds
// TODO: are far pointers still a thing?

#ifndef __linux__

typedef int (FAR *GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef int (FAR *GETNEWDLLFUNCTIONS)(NEW_DLL_FUNCTIONS *, int *);
typedef void (DLLEXPORT *GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);
typedef int (*SERVER_GETBLENDINGINTERFACE) (int, struct sv_blending_interface_s **, struct engine_studio_api_s *, float (*)[3][4], float (*)[MAXSTUDIOBONES][3][4]);
typedef void (DLLEXPORT *SV_SAVEGAMECOMMENT)(char *, int);

typedef void (FAR *LINK_ENTITY_FUNC)(entvars_t *);

#else

#include <dlfcn.h>
#define GetProcAddress dlsym

typedef int BOOL;

typedef int (*GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef int (*GETNEWDLLFUNCTIONS)(NEW_DLL_FUNCTIONS *, int *);
typedef void (*GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);
typedef int (*SERVER_GETBLENDINGINTERFACE) (int, struct sv_blending_interface_s **, struct engine_studio_api_s *, float (*)[3][4], float (*)[MAXSTUDIOBONES][3][4]);
typedef void (*SV_SAVEGAMECOMMENT)(char *, int);

typedef void (*LINK_ENTITY_FUNC)(entvars_t *);

#endif


#if _DEBUG
#define DEBUG_CODE 1
#else
#define DEBUG_CODE 0
#endif // _DEBUG


// define constants used to identify the MOD we are playing...

#define VALVE_DLL		1
#define BSHIFT_DLL		2
#define GEARBOX_DLL		3
#define DECAY_DLL		4
#define CSTRIKE_DLL		5
#define CZERO_DLL		6
#define CZEROR_DLL		7
#define DOD_DLL			8
#define TFC_DLL			9
#define REWOLF_DLL		10

#define NS_DLL			20
#define HUNGER_DLL		21
#define SHIP_DLL		22


// define some function prototypes...
BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128] );
void ClientPutInServer( edict_t *pEntity );
void ClientCommand( edict_t *pEntity );

void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3);


const char *Cmd_Args( void );
const char *Cmd_Argv( int argc );
int Cmd_Argc( void );

#define HW_WAIT_SPAWN 1


#define LADDER_UNKNOWN  0
#define LADDER_UP       1
#define LADDER_DOWN     2

#define WANDER_LEFT  1
#define WANDER_RIGHT 2

#define BOT_PITCH_SPEED 20
#define BOT_YAW_SPEED 20

#define RESPAWN_IDLE             1
#define RESPAWN_NEED_TO_RESPAWN  2
#define RESPAWN_IS_RESPAWNING    3

// game start messages for TFC...
#define MSG_TFC_IDLE          1
#define MSG_TFC_TEAM_SELECT   2
#define MSG_TFC_CLASS_SELECT  3

// game start messages for CS...
#define MSG_CS_IDLE         1
#define MSG_CS_TEAM_SELECT  2
#define MSG_CS_CT_SELECT    3
#define MSG_CS_T_SELECT     4

#define MSG_DOD_IDLE			1
#define MSG_DOD_TEAM_SELECT		2
#define MSG_DOD_ALLIED_SELECT	3
#define MSG_DOD_AXIS_SELECT		4

// game start messages for OpFor...
#define MSG_OPFOR_IDLE          1
#define MSG_OPFOR_TEAM_SELECT   2
#define MSG_OPFOR_CLASS_SELECT  3

// not strictly messages as you don't join team via a menu
#define MSG_NS_JOIN_ALIEN		1
#define MSG_NS_JOIN_MARINE		2
#define MSG_NS_JOIN_AUTO		3

// game start messages for FrontLineForce...
#define MSG_FLF_IDLE            1
#define MSG_FLF_TEAM_SELECT     2
#define MSG_FLF_CLASS_SELECT    3
#define MSG_FLF_PISTOL_SELECT   4
#define MSG_FLF_WEAPON_SELECT   5
#define MSG_FLF_RIFLE_SELECT    6
#define MSG_FLF_SHOTGUN_SELECT  7
#define MSG_FLF_SUBMACHINE_SELECT   8
#define MSG_FLF_HEAVYWEAPONS_SELECT 9


#define kGorgeCost 10
#define kLerkCost 30
#define kFadeCost 50
#define kOnosCost 75

#define kAlienResourceTowerCost 15
#define kHiveCost 40

const int kGameStatusReset = 0;
const int kGameStatusResetNewMap = 1;
const int kGameStatusEnded = 2;
const int kGameStatusGameTime = 3;
const int kGameStatusUnspentLevels = 4;

#define MAX_PLAYERS	32


#define BOT_SKIN_LEN 32
#define BOT_NAME_LEN 32

#define MAX_BOT_WHINE 100

#define MAX_BACKPACKS 100

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

// pev->playerclass?
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

typedef struct {
   edict_t *edict;
   int  armor;   // 0=none
   int  health;  // 0=none
   int  ammo;    // 0=none
   int  team;    // 0=all, 1=Blue, 2=Red, 3=Yellow, 4=Green
} BACKPACK_S;

typedef struct
{
	int  iId;     // weapon ID
	int  iClip;   // amount of ammo in the clip
	int  iAmmo1;  // amount of ammo in primary reserve
	int  iAmmo2;  // amount of ammo in secondary reserve
} bot_current_weapon_t;

struct bot_player_t
{
	char szName[32];
	char szModel[16];
	BOOL bIsUsed;
};

class bot_t;

typedef bool (bot_t::*CanUseWeapon)();

struct weapon_t
{
	int iWeaponId;
	const char *szWeaponName;
	CanUseWeapon pfnCanUseWeapon;
};

class bot_t
{
public:
	bot_t();
	virtual void OnSpawn();
	virtual void Join();

	virtual int GetTeam();

	virtual bool HasEnemy();
	virtual bool IsValidEnemy( edict_t *pEdict );
	virtual float DistanceToEnemy();
	virtual int GetEnemiesInLineOfSight( float fMinDistance, float fMaxDistance );
	virtual bool ShouldReload();

	virtual void PickUpItem();

	virtual void SetMaxSpeed( float fMaxSpeed );
	virtual float GetMaxSpeed();

	virtual bool IsDead();
	virtual bool IsUnderWater();

	virtual void UpdateSounds();

	virtual bool BaseCanUseWeapon();
	virtual std::vector<weapon_t> GetUsableWeapons();

	bool is_used;
	int respawn_state;
	int index;
	edict_t *pEdict;
	bool need_to_initialize;
	char name[BOT_NAME_LEN+1];
	char skin[BOT_SKIN_LEN+1];
	int bot_skill;
	int not_started;
	int start_action;
	float kick_time;
	float create_time;

	// things from pev in CBasePlayer...
	int bot_team;
	int bot_class;
	int bot_money;        // for Counter-Strike
	int primary_weapon;   // for Front Line Force
	int secondary_weapon; // for Front Line Force
	int defender;         // for Front Line Force
	int warmup;           // for Front Line Force
	float idle_angle;
	float idle_angle_time;  // for Front Line Force
	int round_end;        // round has ended (in round based games)
	float blinded_time;

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
	int prev_waypoint_index[5];
	float f_random_waypoint_time;
	int waypoint_goal;
	float f_waypoint_goal_time;
	bool waypoint_near_flag;
	Vector waypoint_flag_origin;
	float prev_waypoint_distance;
	int weapon_points[6];  // five weapon locations + 1 null

	edict_t *pBotEnemy;
	float f_bot_see_enemy_time;
	float f_bot_find_enemy_time;
	edict_t *pBotUser;
	float f_bot_use_time;
	float f_bot_spawn_time;
	edict_t *killer_edict;
	bool  b_bot_say_killed;
	float f_bot_say_killed;
	float f_sniper_aim_time;

	float f_engineer_build_time;

	int   sentrygun_waypoint;
	bool  b_build_sentrygun;
	int   sentrygun_level;
	int   sentrygun_attack_count;
	float f_other_sentry_time;
	bool  b_upgrade_sentry;

	int   dispenser_waypoint;
	bool  b_build_dispenser;
	int   dispenser_built;
	int   dispenser_attack_count;

	float f_shoot_time;
	float f_primary_charging;
	float f_secondary_charging;
	int   charging_weapon_id;
	float f_move_speed;
	float f_pause_time;
	float f_sound_update_time;
	bool  bot_has_flag;

	bool  b_see_tripmine;
	bool  b_shoot_tripmine;
	Vector v_tripmine;

	bool  b_use_health_station;
	float f_use_health_time;
	bool  b_use_HEV_station;
	float f_use_HEV_time;

	bool  b_use_button;
	float f_use_button_time;
	bool  b_lift_moving;

	bool  b_use_capture;
	float f_use_capture_time;
	edict_t *pCaptureEdict;

   	// Gunman Chronicles
	bool bFists;
	int iShotgunMode;
	bool bMinigunSpin;

	// Natural Selection
	bool bUseArmory;
	float fUseArmoryTime;
	int points_spent;

	bool bBuild;
	float fBuildTime;
	bool bBuildAlienResourceTower;
	bool bBuildHive;

	// The Ship
	bool bUseDoor;
	float fUseDoorTime;

	bot_current_weapon_t current_weapon;  // one current weapon for each bot
	int m_rgAmmo[MAX_AMMO_SLOTS];  // total ammo amounts (1 array for each bot)
protected:
	float fMaxSpeed;
	std::vector<weapon_t> weapons;
};

class HalfLifeBot: public bot_t
{
public:
	HalfLifeBot();
	virtual bool CanUseCrowbar();
	virtual bool CanUseGlock();
	virtual bool CanUseMP5Primary();
};

class DODBot : public bot_t
{
public:
	virtual void OnSpawn();
	virtual void Join();

	bool bCapturing;

	const static int TEAM_ALLIES = 1;
	const static int TEAM_AXIS = 2;
};

class TFCBot : public bot_t
{
public:
	const static int CLASS_CIVILIAN = 0;
	const static int CLASS_SCOUT = 1;
	const static int CLASS_SNIPER = 2;
	const static int CLASS_SOLDIER = 3;
	const static int CLASS_DEMOMAN = 4;
	const static int CLASS_MEDIC = 5;
	const static int CLASS_HWGUY = 6;
	const static int CLASS_PYRO = 7;
	const static int CLASS_SPY = 8;
	const static int CLASS_ENGINEER = 9;
};

class GunmanBot : public bot_t
{
public:
	GunmanBot();
	virtual void OnSpawn();
	virtual bool ShouldReload();

	virtual int GetPistolMode();

	virtual void UseGaussPistolPulse();
	virtual void UseGaussPistolCharge();
	virtual void UseGaussPistolRapid();
	virtual void UseGaussPistolSniper();

	virtual bool CanUseFists();
	virtual bool CanUseGaussPistolPulse();
	virtual bool CanUseGaussPistolCharge();
	virtual bool CanUseGaussPistolRapid();
	virtual bool CanUseShotgun();
	virtual bool CanUseMinigun();
	virtual bool CanUseBeamgun();
	virtual bool CanUseChemgun();
	virtual bool CanUseDML();
	virtual bool CanUseDMLGrenade();
	virtual bool CanUseAICore();

	const static int PISTOL_PULSE = 1;
	const static int PISTOL_CHARGE = 2;
	const static int PISTOL_RAPID = 3;
protected:
	int iPistolMode;
};

class NSBot : public bot_t
{
protected:
	int iDesiredClass;
	bool bIsEvolving;
public:
	virtual void OnSpawn();
	virtual void Join();
	virtual void Reset();
	virtual bool ShouldReload();

	virtual bool IsNearHive();
	virtual bool IsInReadyRoom();
	virtual int GetDesiredClass();
	virtual void SetDesiredClass( int iDesiredClass );
	virtual void ChooseDesiredClass();
	virtual void ClassicUpgrade();
	virtual bool ShouldCombatUpgrade();
	virtual void CombatUpgrade();
	virtual float GetResources();

	virtual bool IsMarine();
	virtual bool HasWeaponDamage1();
	virtual void UpgradeToWeaponDamage1();
	virtual bool HasArmor1();
	virtual void UpgradeToArmor1();
	virtual bool HasShotgun();
	virtual void UpgradeToShotgun();
	virtual bool HasHMG();
	virtual void UpgradeToHMG();

	virtual bool IsAlien();
	virtual bool HasCarapace();
	virtual void UpgradeToCarapace();
	virtual bool HasRegeneration();
	virtual void UpgradeToRegeneration();
	virtual bool HasCelerity();
	virtual void UpgradeToCelerity();
	virtual bool CanEvolve();
	virtual bool IsEvolved();
	virtual void StartEvolving();
	virtual void FinishEvolving();
	virtual void EvolveToGorge();
	virtual void EvolveToLerk();
	virtual void EvolveToFade();
	virtual void EvolveToOnos();

	virtual bool ShouldBecomeGorge();
	virtual bool IsGorge();
	virtual bool ShouldBecomeLerk();
	virtual bool IsLerk();
	virtual bool ShouldBecomeFade();
	virtual bool IsFade();
	virtual bool ShouldBecomeOnos();
	virtual bool IsOnos();

	const static int TEAM_NONE = 0;
	const static int TEAM_MARINE = 1;
	const static int TEAM_ALIEN = 2;

	const static int CLASS_SKULK = AVH_USER3_ALIEN_PLAYER1;
	const static int CLASS_GORGE = AVH_USER3_ALIEN_PLAYER2;
	const static int CLASS_LERK = AVH_USER3_ALIEN_PLAYER3;
	const static int CLASS_FADE = AVH_USER3_ALIEN_PLAYER4;
	const static int CLASS_ONOS = AVH_USER3_ALIEN_PLAYER5;

	// from AvHMessage.h
	const static int RESEARCH_ARMOR_ONE = 20;
	const static int RESEARCH_ARMOR_TWO = 21;
	const static int RESEARCH_ARMOR_THREE = 22;
	const static int ALIEN_EVOLUTION_ONE = 101;
	const static int ALIEN_EVOLUTION_TWO = 102;
	const static int ALIEN_EVOLUTION_SEVEN = 107;
	const static int ALIEN_LIFEFORM_ONE = 113;
	const static int ALIEN_LIFEFORM_TWO = 114;
	const static int ALIEN_LIFEFORM_THREE = 115;
	const static int ALIEN_LIFEFORM_FOUR = 116;
	const static int ALIEN_LIFEFORM_FIVE = 117;

	// TODO: possibly make these equal to the exact names of things in the Natural
	// Selection code so they can be traced back if necessary (ideally use the exact
	// names used in Natural Selection but they can be vague at times)
	const static int COMBAT_UPGRADE_WEAPON_DAMAGE_1 = 23;
	const static int COMBAT_UPGRADE_ARMOR_1 = RESEARCH_ARMOR_ONE;
	const static int COMBAT_UPGRADE_SHOTGUN = 64;
	const static int COMBAT_UPGRADE_HMG = 65;
	const static int COMBAT_UPGRADE_CARAPACE = ALIEN_EVOLUTION_ONE;
	const static int COMBAT_UPGRADE_REGENERATION = ALIEN_EVOLUTION_TWO;
	const static int COMBAT_UPGRADE_CELERITY = ALIEN_EVOLUTION_SEVEN;
	const static int EVOLVE_TO_GORGE = ALIEN_LIFEFORM_TWO;
	const static int EVOLVE_TO_LERK = ALIEN_LIFEFORM_THREE;
	const static int EVOLVE_TO_FADE = ALIEN_LIFEFORM_FOUR;
	const static int EVOLVE_TO_ONOS = ALIEN_LIFEFORM_FIVE;
};

class ShipBot : public bot_t
{
protected:
	edict_t *pQuarry;
public:
	virtual bool IsValidEnemy( edict_t *pEdict );
	virtual void PickUpItem();
	virtual void SetQuarry( int iEntIndex );
	virtual edict_t* GetQuarry();
	virtual bool HasQuarry();
};

extern bot_t **pBots;

class Game
{
public:
	virtual bool IsTeamPlay();
};

class GearboxGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return this->IsCTF() || Game::IsTeamPlay();
	}

	bool IsCTF()
	{
		extern edict_t *pent_info_ctfdetect;
		return pent_info_ctfdetect != NULL;
	}
};

class DecayGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}
};

class CStrikeGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}
};

class DODGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}
};

class TFCGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}
};

class NSGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}

	bool IsClassic()
	{
		return !this->IsCombat();
	}

	bool IsCombat()
	{
		const char *szMap = STRING(gpGlobals->mapname);

		return (szMap[0] == 'c') && (szMap[1] == 'o');
	}

	edict_t **GetHives()
	{
		return this->pHives;
	}

	void StartRound()
	{
		for( int i = 0; i < MAX_PLAYERS; i++ )
		{
			pBots[i]->not_started = true;

			// TODO: check that getting the team at this point works
			if( ((NSBot *)pBots[i])->IsAlien() )
			{
				pBots[i]->start_action = MSG_NS_JOIN_ALIEN;
			}
			else if( ((NSBot *)pBots[i])->IsMarine() )
			{
				pBots[i]->start_action = MSG_NS_JOIN_MARINE;
			}
		}
	}
private:
	edict_t *pCommandChair;
	edict_t **pHives;
};

extern Game *pGame;

#define MAX_TEAMS 32
#define MAX_TEAMNAME_LENGTH 16


#define MAX_FLAGS  5

typedef struct {
   bool mdl_match;
   int  team_no;
   edict_t *edict;
} FLAG_S;


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

