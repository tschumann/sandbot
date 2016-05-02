//
// gbot - The GoldSource bot
//
// <no site>
//
// bot_weapons.h
//

#ifndef BOT_WEAPONS_H
#define BOT_WEAPONS_H

// weapon ID values for Half-Life
#define VALVE_WEAPON_CROWBAR       1
#define VALVE_WEAPON_GLOCK         2
#define VALVE_WEAPON_PYTHON        3
#define VALVE_WEAPON_MP5           4
#define VALVE_WEAPON_CHAINGUN      5
#define VALVE_WEAPON_CROSSBOW      6
#define VALVE_WEAPON_SHOTGUN       7
#define VALVE_WEAPON_RPG           8
#define VALVE_WEAPON_GAUSS         9
#define VALVE_WEAPON_EGON         10
#define VALVE_WEAPON_HORNETGUN    11
#define VALVE_WEAPON_HANDGRENADE  12
#define VALVE_WEAPON_TRIPMINE     13
#define VALVE_WEAPON_SATCHEL      14
#define VALVE_WEAPON_SNARK        15

#define VALVE_MAX_NORMAL_BATTERY   100

// weapon ID value for Half-Life Opposing Force

// weapon ID values for Gunman Chronicles
#define GUNMAN_WEAPON_FISTS			1
#define GUNMAN_WEAPON_GAUSSPISTOL	30
#define GUNMAN_WEAPON_SHOTGUN		17
#define GUNMAN_WEAPON_MINIGUN		28
#define GUNMAN_WEAPON_BEAMGUN		14
#define GUNMAN_WEAPON_DML			16
#define GUNMAN_WEAPON_CHEMGUN		19
#define GUNMAN_WEAPON_DMLGRENADE	13
#define GUNMAN_WEAPON_AICORE		7

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

// weapon ID values for They Hunger
#define HUNGER_WEAPON_TH_SPANNER	16
#define HUNGER_WEAPON_TH_AP9		17
#define HUNGER_WEAPON_TH_SHOVEL		18
#define HUNGER_WEAPON_TH_SNIPER		19
#define HUNGER_WEAPON_TH_TAURUS		21
#define HUNGER_WEAPON_TH_CHAINGUN	22
#define HUNGER_WEAPON_TH_MEDKIT		23

// weapon ID value for The Ship

typedef struct
{
    char szClassname[64];
    int  iAmmo1;    // ammo index for primary ammo
    int  iAmmo1Max; // max primary ammo
    int  iAmmo2;    // ammo index for secondary ammo
    int  iAmmo2Max; // max secondary ammo
    int  iSlot;     // HUD slot (0 based)
    int  iPosition; // slot position
    int  iId;       // weapon ID
    int  iFlags;    // flags???
	// Gunman Chronicles extras
	char szParentClass[64];	// parent weapon for a customization
	int stuff[3];			// TODO: what is this?
} bot_weapon_t;

#endif // BOT_WEAPONS_H

