//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_weapons.h
//

#ifndef BOT_WEAPONS_H
#define BOT_WEAPONS_H

typedef struct
{
	int iId;  // the weapon ID value
	char weapon_name[64];  // name of the weapon when selecting it
	float fPrimaryRate;			// primary rate of fire
	float fSecondaryRate;		// secondary rate of fire
	float primary_min_distance;    // 0 = no minimum
	float primary_max_distance;    // 9999 = no maximum
	float secondary_min_distance; // 0 = no minimum
	float secondary_max_distance; // 9999 = no maximum
	int use_percent;    // times out of 100 to use this weapon when available
	bool can_use_underwater;      // can use this weapon underwater
	int primary_fire_percent;    // times out of 100 to use primary fire
	int min_primary_ammo;         // minimum ammout of primary ammo needed to fire
	int min_secondary_ammo;      // minimum ammout of seconday ammo needed to fire
	bool primary_fire_hold;        // hold down primary fire button to use?
	bool secondary_fire_hold;     // hold down secondary fire button to use?
	bool primary_fire_charge;     // charge weapon using primary fire?
	bool secondary_fire_charge;  // charge weapon using secondary fire?
	float primary_charge_delay;    // time to charge weapon
	float secondary_charge_delay; // time to charge weapon
	// int iPrimaryAmmoIndex;
	// int iSecondaryAmmoIndex;
	int flags;
} bot_weapon_select_t;

#define NO_SUCH_WEAPON -1

#define WEAPON_NONE 0
#define WEAPON_MELEE 1
#define WEAPON_PISTOL 2
#define WEAPON_SHOTGUN 4
#define WEAPON_MACHINEGUN 8
#define WEAPON_RIFLE 16
#define WEAPON_SNIPER 32
#define WEAPON_ROCKETLAUNCHER 64
#define WEAPON_ENERGY 128  
#define WEAPON_ORGANIC 256
#define WEAPON_GRENADE 512
#define WEAPON_CHEMICAL 1024
// #define WEAPON_OTHER 2048

// weapon ID values for Half-Life
#define VALVE_WEAPON_CROWBAR		1
#define VALVE_WEAPON_GLOCK			2
#define VALVE_WEAPON_PYTHON			3
#define VALVE_WEAPON_MP5			4
#define VALVE_WEAPON_CHAINGUN		5
#define VALVE_WEAPON_CROSSBOW		6
#define VALVE_WEAPON_SHOTGUN		7
#define VALVE_WEAPON_RPG			8
#define VALVE_WEAPON_GAUSS			9
#define VALVE_WEAPON_EGON			10
#define VALVE_WEAPON_HORNETGUN		11
#define VALVE_WEAPON_HANDGRENADE	12
#define VALVE_WEAPON_TRIPMINE		13
#define VALVE_WEAPON_SATCHEL		14
#define VALVE_WEAPON_SNARK			15

#define VALVE_MAX_NORMAL_BATTERY	100
#define VALVE_HORNET_MAX_CARRY		8

// weapon ID values for Half-Life Opposing Force
#define GEARBOX_WEAPON_CROWBAR		1
#define GEARBOX_WEAPON_GLOCK		2
#define GEARBOX_WEAPON_PYTHON		3
#define GEARBOX_WEAPON_MP5			4
#define GEARBOX_WEAPON_CHAINGUN		5
#define GEARBOX_WEAPON_CROSSBOW		6
#define GEARBOX_WEAPON_SHOTGUN		7
#define GEARBOX_WEAPON_RPG			8
#define GEARBOX_WEAPON_GAUSS		9
#define GEARBOX_WEAPON_EGON			10
#define GEARBOX_WEAPON_HORNETGUN	11
#define GEARBOX_WEAPON_HANDGRENADE	12
#define GEARBOX_WEAPON_TRIPMINE		13
#define GEARBOX_WEAPON_SATCHEL		14
#define GEARBOX_WEAPON_SNARK		15
#define GEARBOX_WEAPON_GRAPPLE		16
#define GEARBOX_WEAPON_EAGLE		17
#define GEARBOX_WEAPON_PIPEWRENCH	18
#define GEARBOX_WEAPON_M249			19
#define GEARBOX_WEAPON_DISPLACER	20
#define GEARBOX_WEAPON_UNKNOWN21	21
#define GEARBOX_WEAPON_SHOCKRIFLE	22
#define GEARBOX_WEAPON_SPORELAUNCHER	23
#define GEARBOX_WEAPON_SNIPERRIFLE	24
#define GEARBOX_WEAPON_KNIFE		25

// weapon ID values for Counter-Strike
#define CS_WEAPON_P228				1
#define CS_WEAPON_SHIELDGUN			2
#define CS_WEAPON_SCOUT				3
#define CS_WEAPON_HEGRENADE			4
#define CS_WEAPON_XM1014			5
#define CS_WEAPON_C4				6
#define CS_WEAPON_MAC10				7
#define CS_WEAPON_AUG				8
#define CS_WEAPON_SMOKEGRENADE		9
#define CS_WEAPON_ELITE				10
#define CS_WEAPON_FIVESEVEN			11
#define CS_WEAPON_UMP45				12
#define CS_WEAPON_SG550				13
#define CS_WEAPON_GALIL				14
#define CS_WEAPON_FAMAS				15
#define CS_WEAPON_USP				16
#define CS_WEAPON_GLOCK18			17
#define CS_WEAPON_AWP				18
#define CS_WEAPON_MP5NAVY			19
#define CS_WEAPON_M249				20
#define CS_WEAPON_M3				21
#define CS_WEAPON_M4A1				22
#define CS_WEAPON_TMP				23
#define CS_WEAPON_G3SG1				24
#define CS_WEAPON_FLASHBANG			25
#define CS_WEAPON_DEAGLE			26
#define CS_WEAPON_SG552				27
#define CS_WEAPON_AK47				28
#define CS_WEAPON_KNIFE				29
#define CS_WEAPON_P90				30

// weapon ID values for Day of Defeat
#define DOD_WEAPON_GARAND			5
#define DOD_WEAPON_THOMPSON			7
#define DOD_WEAPON_MP44				8
#define DOD_WEAPON_KAR				10
#define DOD_WEAPON_MP40				12
#define DOD_WEAPON_HANDGRENADE		13
#define DOD_WEAPON_STICKGRENADE		14
#define DOD_WEAPON_M1CARBINE		20
#define DOD_WEAPON_GREASEGUN		22
#define DOD_WEAPON_K43				24
#define DOD_WEAPON_BAZOOKA			29
#define DOD_WEAPON_PSCHRECK			30
#define DOD_WEAPON_30CAL
#define DOD_WEAPON_AMERKNIFE
#define DOD_WEAPON_BAR
#define DOD_WEAPON_BREN
#define DOD_WEAPON_COLT
#define DOD_WEAPON_ENFIELD
#define DOD_WEAPON_FG42
#define DOD_WEAPON_GERKNIFE
#define DOD_WEAPON_LUGER
#define DOD_WEAPON_MG34
#define DOD_WEAPON_MG42
#define DOD_WEAPON_PIAT
#define DOD_WEAPON_SCOPEDKAR
#define DOD_WEAPON_SPADE
#define DOD_WEAPON_SPRING
#define DOD_WEAPON_STEN
#define DOD_WEAPON_WEBLEY

// weapon ID values for Team Fortress Classic
#define TF_WEAPON_UNKNOWN1			1
#define TF_WEAPON_UNKNOWN2			2
#define TF_WEAPON_MEDIKIT			3
#define TF_WEAPON_SPANNER			4
#define TF_WEAPON_AXE				5
#define TF_WEAPON_SNIPERRIFLE		6
#define TF_WEAPON_AUTORIFLE			7
#define TF_WEAPON_SHOTGUN			8
#define TF_WEAPON_SUPERSHOTGUN		9
#define TF_WEAPON_NAILGUN			10
#define TF_WEAPON_SUPERNAILGUN		11
#define TF_WEAPON_GL				12
#define TF_WEAPON_FLAMETHROWER		13
#define TF_WEAPON_RPG				14
#define TF_WEAPON_IC				15
#define TF_WEAPON_UNKNOWN16			16
#define TF_WEAPON_AC				17
#define TF_WEAPON_UNKNOWN18			18
#define TF_WEAPON_UNKNOWN19			19
#define TF_WEAPON_TRANQ				20
#define TF_WEAPON_RAILGUN			21
#define TF_WEAPON_PL				22
#define TF_WEAPON_KNIFE				23

// weapon ID values for Gunman Chronicles
#define GUNMAN_WEAPON_FISTS			1
#define GUNMAN_WEAPON_AICORE		7
#define GUNMAN_WEAPON_DMLGRENADE	13
#define GUNMAN_WEAPON_BEAMGUN		14
#define GUNMAN_WEAPON_DML			16
#define GUNMAN_WEAPON_CHEMGUN		19
#define GUNMAN_WEAPON_SHOTGUN		17
#define GUNMAN_WEAPON_MINIGUN		28
#define GUNMAN_WEAPON_GAUSSPISTOL	30

// weapon ID values for Natural Selection
#define NS_WEAPON_CLAWS				1
#define NS_WEAPON_SPIT				2
#define NS_WEAPON_SPORE				3
#define NS_WEAPON_SPIKEGUN			4
#define NS_WEAPON_BITEGUN			5
#define NS_WEAPON_BITE2GUN			6
#define NS_WEAPON_SWIPE				7
#define NS_WEAPON_WEBSPINNER		8
#define NS_WEAPON_METABOLIZE		9
#define NS_WEAPON_PARASITE			10
#define NS_WEAPON_BLINK				11
#define NS_WEAPON_DIVINEWIND		12
#define NS_WEAPON_KNIFE				13
#define NS_WEAPON_PISTOL			14
#define NS_WEAPON_MACHINEGUN		15
#define NS_WEAPON_SHOTGUN			16
#define NS_WEAPON_HEAVYMACHINEGUN	17
#define NS_WEAPON_WELDER			18
#define NS_WEAPON_MINE				19
#define NS_WEAPON_GRENADEGUN		20
#define NS_WEAPON_LEAP				21
#define NS_WEAPON_CHARGE			22
#define NS_WEAPON_UMBRA				23
#define NS_WEAPON_PRIMALSCREAM		24
#define NS_WEAPON_BILEBOMBGUN		25
#define NS_WEAPON_ACIDROCKETGUN		26
#define NS_WEAPON_HEALINGSPRAY		27
#define NS_WEAPON_GRENADE			28
#define NS_WEAPON_STOMP				29
#define NS_WEAPON_DEVOUR			30

// weapon ID values for They Hunger (others shared with Half-Life)
// TODO: redefine the Half-Life ones
#define HUNGER_WEAPON_TH_SPANNER	16
#define HUNGER_WEAPON_TH_AP9		17
#define HUNGER_WEAPON_TH_SHOVEL		18
#define HUNGER_WEAPON_TH_SNIPER		19
#define HUNGER_WEAPON_TH_TAURUS		21
#define HUNGER_WEAPON_TH_CHAINGUN	22
#define HUNGER_WEAPON_TH_MEDKIT		23

// weapon ID values for The Ship
#define SHIP_WEAPON_CROWBAR			1
#define SHIP_WEAPON_9MMHANDGUN		2
#define SHIP_WEAPON_357				3
#define SHIP_WEAPON_9MMAR			4
#define SHIP_WEAPON_CROSSBOW		6
#define SHIP_WEAPON_SHOTGUN			7
#define SHIP_WEAPON_RPG				8
#define SHIP_WEAPON_HANDGRENADE		12
#define SHIP_WEAPON_SATCHEL			14
#define SHIP_WEAPON_PUSHHANDS		16
#define SHIP_WEAPON_UMBRELLA		17
#define SHIP_WEAPON_SYRINGE			18
// #define SHIP_WEAPON_GAUSS			9
// #define SHIP_WEAPON_EGON			10
// #define SHIP_WEAPON_HORNETGUN		11
// #define SHIP_WEAPON_TRIPMINE		13
// #define SHIP_WEAPON_SNARK			15

typedef struct
{
	char szClassname[64];
	int  iAmmo1;     // ammo index for primary ammo
	int  iAmmo1Max;  // max primary ammo
	int  iAmmo2;     // ammo index for secondary ammo
	int  iAmmo2Max;  // max secondary ammo
	int  iSlot;      // HUD slot (0 based)
	int  iPosition;  // slot position
	int  iId;        // weapon ID
	int  iFlags;     // flags???
	// Gunman Chronicles extras
	char szParentClass[64];	// parent weapon for customisations
	int iUnknown[3];		// TODO: what is this?
} bot_weapon_t;

extern bot_weapon_t weapon_defs[MAX_WEAPONS];

#endif // BOT_WEAPONS_H

