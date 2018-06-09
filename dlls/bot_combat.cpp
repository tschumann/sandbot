//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_combat.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"

extern int mod_id;
extern bot_weapon_t weapon_defs[MAX_WEAPONS];
extern bool b_observer_mode;
extern int team_allies[4];

extern cvar_t bot_use_melee;
extern cvar_t bot_use_pistol;
extern cvar_t bot_use_shotgun;
extern cvar_t bot_use_machinegun;
extern cvar_t bot_use_rifle;
extern cvar_t bot_use_sniper;
extern cvar_t bot_use_rocketlauncher;
extern cvar_t bot_use_energy;
extern cvar_t bot_use_organic;
extern cvar_t bot_use_grenade;
extern cvar_t bot_use_chemical;

// weapons are stored in priority order, most desired weapon should be at
// the start of the array and least desired should be at the end
// presumably the no ammo ones are at the top so they can always be selected

bot_weapon_select_t valve_weapon_select[] = {
    {VALVE_WEAPON_CROWBAR, "weapon_crowbar", 0.3f, 0.0f,
	0.0f, 32.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
    {VALVE_WEAPON_HANDGRENADE, "weapon_handgrenade", 0.1f, 0.0f,
	250.0f, 750.0f, 0.0f, 0.0f,
    30, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_GRENADE},
    {VALVE_WEAPON_SNARK, "weapon_snark", 0.1f, 0.0f,
	150.0f, 500.0f, 0.0f, 0.0f,
    50, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_GRENADE},
    {VALVE_WEAPON_EGON, "weapon_egon", 0.0f, 0.0f,
	128.0f, 2048.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	TRUE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ENERGY},
    {VALVE_WEAPON_GAUSS, "weapon_gauss", 0.2f, 1.0f,
	0.0f, 8192.0f, 0.0f, 8192.0f,
    100, FALSE, 80, 1, 10,
	FALSE, FALSE, FALSE, TRUE, 0.0f, 0.8f, WEAPON_ENERGY},
    {VALVE_WEAPON_SHOTGUN, "weapon_shotgun", 0.75f, 1.5f,
	0.0f, 150.0f, 0.0f, 150.0f,
    100, FALSE, 70, 1, 2,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_SHOTGUN},
    {VALVE_WEAPON_PYTHON, "weapon_357", 0.75f, 0.0f, 0.0f,
	8192.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
    {VALVE_WEAPON_HORNETGUN, "weapon_hornetgun", 0.25f, 0.0f,
	0.0f, 1000.0f, 0.0f, 1000.0f,
    100, TRUE, 50, 1, 4,
	FALSE, TRUE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ORGANIC},
    {VALVE_WEAPON_MP5, "weapon_9mmAR", 0.1f, 1.0f,
	0.0f, 8192.0f, 250.0f, 600.0f,
    100, FALSE, 90, 1, 1,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
    {VALVE_WEAPON_CROSSBOW, "weapon_crossbow", 0.75f, 0.0f,
	128.0f, 8192.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_SNIPER},
    {VALVE_WEAPON_RPG, "weapon_rpg", 1.5f, 0.0f,
	250.0f, 9999.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ROCKETLAUNCHER},
    {VALVE_WEAPON_GLOCK, "weapon_9mmhandgun", 0.3f, 0.2f,
	0.0f, 8192.0f, 0.0f, 1024.0f,
    100, TRUE, 70, 1, 1,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
    /* terminator */
    {0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_NONE}
};

bot_weapon_select_t gearbox_weapon_select[] = {
	{GEARBOX_WEAPON_PIPEWRENCH, "weapon_pipewrench", 0.5f, 0.0f,
	 0.0f, 50.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{GEARBOX_WEAPON_KNIFE, "weapon_knife", 0.4f, 0.0f,
	 0.0f, 50.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{GEARBOX_WEAPON_CROWBAR, "weapon_crowbar", 0.3f, 0.0f,
	 0.0f, 50.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{GEARBOX_WEAPON_DISPLACER, "weapon_displacer", 5.0, 0.0f,
	 100.0f, 1000.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ENERGY},
	{GEARBOX_WEAPON_SPORELAUNCHER, "weapon_sporelauncher", 0.5f, 0.0f,
	 500.0f, 1000.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ORGANIC},
	{GEARBOX_WEAPON_SHOCKRIFLE, "weapon_shockrifle", 0.1f, 0.0f,
	 50.0f, 800.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ORGANIC},
	{GEARBOX_WEAPON_SNIPERRIFLE, "weapon_sniperrifle", 1.5f, 0.0f,
	 50.0f, 1500.0f, 0.0f, 0.0f,
	 100, FALSE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_SNIPER},
	{GEARBOX_WEAPON_HANDGRENADE, "weapon_handgrenade", 0.1f, 0.0f,
	 250.0f, 750.0f, 0.0f, 0.0f,
	 30, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_GRENADE},
	{GEARBOX_WEAPON_SNARK, "weapon_snark", 0.1f, 0.0f,
	 150.0f, 500.0f, 0.0f, 0.0f,
	 50, FALSE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_GRENADE},
	{GEARBOX_WEAPON_EGON, "weapon_egon", 0.0f, 0.0f,
	 0.0f, 9999.0f, 0.0f, 0.0f,
	 100, FALSE, 100, 1, 0, TRUE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ENERGY},
	{GEARBOX_WEAPON_GAUSS, "weapon_gauss", 0.2f, 1.0f,
	 0.0f, 9999.0f, 0.0f, 9999.0f,
	 100, FALSE, 80, 1, 10, FALSE, FALSE, FALSE, TRUE, 0.0f, 0.8f, WEAPON_ENERGY},
	{GEARBOX_WEAPON_M249, "weapon_m249", 0.1f, 0.0f,
	 0.0f, 400.0f, 0.0f, 0.0f,
	 100, FALSE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{GEARBOX_WEAPON_SHOTGUN, "weapon_shotgun", 0.75f, 1.5f,
	 30.0f, 150.0f, 30.0f, 150.0f,
	 100, FALSE, 70, 1, 2, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_SHOTGUN},
	{GEARBOX_WEAPON_EAGLE, "weapon_eagle", 0.25f, 0.0f,
	 0.0f, 1200.0f, 0.0f, 0.0f,
	 100, FALSE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
	{GEARBOX_WEAPON_PYTHON, "weapon_357", 0.75f, 0.0f,
	 30.0f, 700.0f, 0.0f, 0.0f,
	 100, FALSE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
	{GEARBOX_WEAPON_HORNETGUN, "weapon_hornetgun", 0.25f, 0.0f,
	 30.0f, 1000.0f, 30.0f, 1000.0f,
	 100, TRUE, 50, 1, 4, FALSE, TRUE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ORGANIC},
	{GEARBOX_WEAPON_MP5, "weapon_9mmAR", 0.1f, 1.0f,
	 0.0f, 250.0f, 300.0f, 600.0f,
	 100, FALSE, 90, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{GEARBOX_WEAPON_CROSSBOW, "weapon_crossbow", 0.75f, 0.0f,
	 100.0f, 1000.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_SNIPER},
	{GEARBOX_WEAPON_RPG, "weapon_rpg", 1.5f, 0.0f,
	 300.0f, 9999.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_ROCKETLAUNCHER},
	{GEARBOX_WEAPON_GLOCK, "weapon_9mmhandgun", 0.3f, 0.2f,
	 0.0f, 1200.0f, 0.0f, 1200.0f,
	 100, TRUE, 70, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
	/* terminator */
	{0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_NONE}
};

bot_weapon_select_t cs_weapon_select[] = {
	{CS_WEAPON_KNIFE, "weapon_knife", 0.3f, 0.0f,
	 0.0f, 50.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{CS_WEAPON_USP, "weapon_usp", 0.3f, 0.2f,
	 0.0f, 1200.0f, 0.0f, 1200.0f,
	 100, TRUE, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
	{CS_WEAPON_GLOCK18, "weapon_glock18", 0.3f, 0.2f,
	 0.0f, 1200.0f, 0.0f, 1200.0f,
	 100, TRUE, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
	/* terminator */
	{0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_NONE}
};

bot_weapon_select_t dod_weapon_select[] = {
	{DOD_WEAPON_GARAND, "weapon_garand", 0.3f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_RIFLE},
	{DOD_WEAPON_M1CARBINE, "weapon_m1carbine", 0.3f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_RIFLE},
	{DOD_WEAPON_THOMPSON, "weapon_thompson", 0.0f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{DOD_WEAPON_GREASEGUN, "weapon_greasegun", 0.0f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{DOD_WEAPON_BAZOOKA, "weapon_bazooka", 1.5f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 1.0f, 0.0f, WEAPON_ROCKETLAUNCHER},

	/*{DOD_WEAPON_COLT, "weapon_colt", 0.3f, 0.2f,
	 0.0f, 1200.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},*/
	{DOD_WEAPON_HANDGRENADE, "weapon_handgrenade", 1.0f, 0.0f,
	 0.0f, 100.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_GRENADE},
	/*{DOD_WEAPON_AMERKNIFE, "weapon_amerknife", 0.3f, 0.0f,
	 0.0f, 50.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},*/

	{DOD_WEAPON_KAR, "weapon_kar", 0.3f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_RIFLE},
	{DOD_WEAPON_K43, "weapon_k43", 0.3f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_RIFLE},
	{DOD_WEAPON_MP40, "weapon_mp40", 0.0f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{DOD_WEAPON_MP44, "weapon_mp44", 0.0f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{DOD_WEAPON_PSCHRECK, "weapon_pschreck", 1.5f, 0.0f,
	 0.0f, 2000.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 1.0f, 0.0f, WEAPON_ROCKETLAUNCHER},

	/*{DOD_WEAPON_LUGER, "weapon_luger", 0.3f, 0.2f,
	 0.0f, 1200.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},*/
	{DOD_WEAPON_STICKGRENADE, "weapon_stickgrenade", 1.0f, 0.0f,
	 0.0f, 100.0f, 0.0f, 0.0f,
	 100, false, 100, 1, 0,
	 FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_GRENADE},
	/*{DOD_WEAPON_GERKNIFE, "weapon_gerknife", 0.3f, 0.0f,
	 0.0f, 50.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},*/
	/* terminator */
	{0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_NONE}
};

bot_weapon_select_t tfc_weapon_select[] = {
	{TF_WEAPON_AXE, "tf_weapon_axe", 0.3f, 0.0f,
	 0.0f, 50.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_KNIFE, "tf_weapon_knife", 0.3f, 0.0f,
	 0.0f, 40.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_SPANNER, "tf_weapon_knife", 0.3f, 0.0f,
	 0.0f, 40.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_MEDIKIT, "tf_weapon_medikit", 0.3f, 0.0f,
	 0.0f, 40.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 0, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_SNIPERRIFLE, "tf_weapon_sniperrifle", 1.0f, 0.0f,
	 300.0f, 2500.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, TRUE, FALSE, 1.0f, 0.0f},
	{TF_WEAPON_FLAMETHROWER, "tf_weapon_flamethrower", 0.0f, 0.0f,
	 100.0f, 500.0f, 0.0f, 0.0f,
	 100, FALSE, 100, 1, 0, TRUE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_AC, "tf_weapon_ac", 0.0f, 0.0f,
	 50.0f, 1000.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, TRUE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_GL, "tf_weapon_gl", 0.6f, 0.0f,
	 300.0f, 900.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_RPG, "tf_weapon_rpg", 0.5f, 0.0f,
	 300.0f, 900.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_IC, "tf_weapon_ic", 2.0, 0.0f,
	 300.0f, 800.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_TRANQ, "tf_weapon_tranq", 1.5f, 0.0f,
	 40.0f, 1000.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_RAILGUN, "tf_weapon_railgun", 0.4f, 0.0f,
	 40.0f, 800.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_SUPERNAILGUN, "tf_weapon_superng", 0.0f, 0.0f,
	 40.0f, 800.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, TRUE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_SUPERSHOTGUN, "tf_weapon_supershotgun", 0.6f, 0.0f,
	 40.0f, 500.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 2, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_AUTORIFLE, "tf_weapon_autorifle", 0.1f, 0.0f,
	 0.0f, 800.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_SHOTGUN, "tf_weapon_shotgun", 0.5f, 0.0f,
	 40.0f, 400.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{TF_WEAPON_NAILGUN, "tf_weapon_ng", 0.1f, 0.0f,
	 40.0f, 600.0f, 0.0f, 0.0f,
	 100, TRUE, 100, 1, 0, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	/* terminator */
	{0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f}
};

bot_weapon_select_t gunman_weapon_select[] = {
	{GUNMAN_WEAPON_DMLGRENADE, "weapon_dmlgrenade", 1.0f, 0.0f,
	250.0f, 750.0f, 0.0f, 0.0f,
	30, true, 100, 1, 0,
	false, false, false, false, 0.0f, 0.0f, WEAPON_GRENADE},
	{GUNMAN_WEAPON_BEAMGUN, "weapon_beamgun", 0.0f, 0.0f,
	0.0f, 500.0f, 0.0f, 0.0f,
	100, false, 100, 1, 0,
	true, false, false, false, 0.0f, 0.0f, WEAPON_ENERGY},
	{GUNMAN_WEAPON_MINIGUN, "weapon_minigun", 0.1f, 0.0f,
	0.0f, 1024.0f, 0.0f, 0.0f,
	100, true, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{GUNMAN_WEAPON_SHOTGUN, "weapon_shotgun", 0.75f, 0.0f,
	0.0f, 150.0f, 0.0f, 0.0f,
    100, true, 100, 2, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_SHOTGUN},
	{GUNMAN_WEAPON_CHEMGUN, "weapon_chemgun", 1.0f, 0.0f,
	100.0f, 400.0f, 0.0f, 0.0f,
	100, true, 100, 2, 0,
	false, false, false, false, 0.0f, 0.0f, WEAPON_CHEMICAL},
	{GUNMAN_WEAPON_DML, "weapon_dml", 1.5f, 0.0f,
	250.0f, 9999.0f, 0.0f, 0.0f,
	100, true, 100, 1, 0,
	false, false, false, false, 0.0f, 0.0f, WEAPON_ROCKETLAUNCHER},
	{GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", 0.0f, 0.0f,
	0.0f, 1024.0f, 0.0f, 0.0f,
    100, true, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
	{GUNMAN_WEAPON_FISTS, "weapon_fists", 0.3f, 0.0f,
	0.0f, 32.0, 0.0f, 0.0f,
    100, true, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{GUNMAN_WEAPON_AICORE, "weapon_aicore", 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0, true, 100, 0, 0,
	false, false, false, false, 0.0f, 0.0f, WEAPON_NONE},
	/* terminator */
    {0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_NONE}
};

// see AvHBasePlayerWeapon::mRange
bot_weapon_select_t ns_weapon_select[] = {
    {NS_WEAPON_GRENADE, "weapon_grenade", 1.0f, 0.0f,
	250.0f, 750.0f, 0.0f, 0.0f,
    40, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_GRENADE},
    {NS_WEAPON_HEAVYMACHINEGUN, "weapon_heavymachinegun", 0.0f, 0.0f,
	0.0f, 250.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
	{NS_WEAPON_SHOTGUN, "weapon_shotgun", 1.3f, 0.0f,
	30.0f, 150.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_SHOTGUN},
	{NS_WEAPON_MACHINEGUN, "weapon_machinegun", 0.0f, 0.0f,
	0.0f, 250.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MACHINEGUN},
    {NS_WEAPON_GRENADEGUN, "weapon_grenadegun", 1.2f, 0.0f,
	300.0f, 500.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {NS_WEAPON_PISTOL, "weapon_pistol", 0.5f, 0.0f,
	0.0f, 1200.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_PISTOL},
	{NS_WEAPON_PISTOL, "weapon_welder", 0.0f, 0.0f,
	0.0f, 50.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{NS_WEAPON_KNIFE, "weapon_knife", 0.7f, 0.0f,
	0.0f, 5.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},

	// skulk
	{NS_WEAPON_BITEGUN, "weapon_bitegun", 0.8f, 0.0f,
	0.0f, 50.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},

	// gorge
	{NS_WEAPON_SPIT, "weapon_spit", 0.8f, 0.0f,
	0.0f, 250.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_CHEMICAL},

	// lerk
	{NS_WEAPON_BITE2GUN, "weapon_bite2gun", 0.7f, 0.0f,
	0.0f, 50.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{NS_WEAPON_SPORE, "weapon_spore", 0.5f, 0.0f,
	0.0f, 225.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},

	// fade
	{NS_WEAPON_SWIPE, "weapon_swipe", 0.9f, 0.0f,
	0.0f, 50.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{NS_WEAPON_BLINK, "weapon_blink", 0.0f, 0.0f,
	50.0f, 9999.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{NS_WEAPON_ACIDROCKETGUN, "weapon_acidrocketgun", 1.0f, 0.0f,
	50.0f, 1200.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_CHEMICAL},

	// onos
	{NS_WEAPON_CLAWS, "weapon_claws", 0.9f, 0.0f,
	0.0f, 50.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f, WEAPON_MELEE},
	{NS_WEAPON_STOMP, "weapon_stomp", 1.0f, 0.0f,
	0.0f, 250.0f, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{NS_WEAPON_CHARGE, "weapon_charge", 1.0f, 0.0f,
	0.0f, 100.0f, 0.0f, 0.0f,
    30, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},

    /* terminator */
    {0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f}
};

bot_weapon_select_t hunger_weapon_select[] = {
    {VALVE_WEAPON_CROWBAR, "weapon_crowbar", 0.3f, 0.0f,
	0.0f, 32.0, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{HUNGER_WEAPON_TH_SPANNER, "weapon_th_shovel", 0.3f, 0.0f,
	0.0f, 32.0, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{HUNGER_WEAPON_TH_SPANNER, "weapon_th_spanner", 0.3f, 0.0f,
	0.0f, 32.0, 0.0f, 0.0f,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_HANDGRENADE, "weapon_handgrenade", 0.1f, 0.0f,
	250.0f, 750.0f, 0.0f, 0.0f,
    30, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_SNARK, "weapon_snark", 0.1f, 0.0f,
	150.0f, 500.0f, 0.0f, 0.0f,
    50, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_EGON, "weapon_egon", 0.0f, 0.0f,
	128.0f, 2048.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	TRUE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_GAUSS, "weapon_gauss", 0.2f, 1.0f,
	0.0f, 8192.0f, 0.0f, 8192.0f,
    100, FALSE, 80, 1, 10,
	FALSE, FALSE, FALSE, TRUE, 0.0f, 0.8f},
    {VALVE_WEAPON_SHOTGUN, "weapon_shotgun", 0.75f, 1.5f,
	0.0f, 150.0f, 0.0f, 150.0f,
    100, FALSE, 70, 1, 2,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_PYTHON, "weapon_357", 0.75f, 0.0f,
	0.0f, 8192.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{HUNGER_WEAPON_TH_TAURUS, "weapon_th_taurus", 0.75f, 0.0f,
	0.0f, 8192.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_MP5, "weapon_9mmAR", 0.1f, 1.0f,
	0.0f, 8192.0f, 250.0f, 600.0f,
    100, FALSE, 90, 1, 1,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{HUNGER_WEAPON_TH_CHAINGUN, "weapon_th_chaingun", 0.1f, 0.0f,
	0.0f, 8192.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{HUNGER_WEAPON_TH_AP9, "weapon_th_ap9", 0.1f, 1.0f,
	0.0f, 8192.0f, 0.0f, 8192.0f,
    100, FALSE, 70, 1, 3,
	true, FALSE, FALSE, FALSE, 0.0f, 0.0f},
	{HUNGER_WEAPON_TH_SNIPER, "weapon_th_sniper", 0.1f, 0.0f,
	0.0f, 8192.0f, 0.0f, 0.0f,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_CROSSBOW, "weapon_crossbow", 0.75f, 0.0f,
	128.0f, 8192.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_RPG, "weapon_rpg", 1.5f, 0.0f, 250.0f, 9999.0f, 0.0f, 0.0f,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    {VALVE_WEAPON_GLOCK, "weapon_9mmhandgun", 0.3f, 0.2f,
	0.0f, 8192.0f, 0.0f, 1024.0f,
    100, TRUE, 70, 1, 1,
	FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f},
    /* terminator */
    {0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f}
};

bot_weapon_select_t ship_weapon_select[] = {
	/* terminator */
	{0, "", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0f, 0.0f}
};

edict_t *BotFindEnemy( bot_t *pBot )
{
	Vector vecEnd;
	static bool flag=TRUE;
	edict_t *pent = NULL;
	edict_t *pNewEnemy;
	float nearestdistance;
	int i;

	edict_t *pEdict = pBot->pEdict;

	if (pBot->HasEnemy())  // does the bot already have an enemy?
	{
		vecEnd = pBot->pBotEnemy->v.origin + pBot->pBotEnemy->v.view_ofs;

		// if the enemy is dead? if it is, assume the bot killed it
		// SOLID_NOT check is for Natural Selection
		if (!IsAlive(pBot->pBotEnemy) || pBot->pBotEnemy->v.solid == SOLID_NOT)
		{
			if( pBot->ShouldJumpAfterDeath() )
			{
				pEdict->v.button |= IN_JUMP;
			}

			// don't have an enemy anymore so null out the pointer
			pBot->pBotEnemy = nullptr;
		}
		else if (FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ))
		{
			if ((mod_id == TFC_DLL) && (pEdict->v.playerclass == TFCBot::CLASS_MEDIC))
			{
				if (pBot->pBotEnemy->v.health >= pBot->pBotEnemy->v.max_health)
				{
					// player is healed, null out pointer
					pBot->pBotEnemy = nullptr;
				}
			}
			else
			{
				// if enemy is still visible and in field of view, keep it

				// face the enemy
				Vector v_enemy = pBot->pBotEnemy->v.origin - pEdict->v.origin;
				Vector bot_angles = UTIL_VecToAngles( v_enemy );

				pEdict->v.ideal_yaw = bot_angles.y;

				BotFixIdealYaw(pEdict);

				// keep track of when we last saw an enemy
				pBot->f_bot_see_enemy_time = gpGlobals->time;

				return pBot->pBotEnemy;
			}
		}
	}

   pent = NULL;
   pNewEnemy = NULL;
   nearestdistance = 1000;

   if( mod_id == TFC_DLL )
   {
	   pNewEnemy = ((TFCBot *)pBot)->FindEnemy();
   }

   // look for an enemy from players
   if (pNewEnemy == NULL)
   {
      nearestdistance = 2500;

      // search the world for players...
      for (i = 1; i <= gpGlobals->maxClients; i++)
      {
         edict_t *pPlayer = INDEXENT(i);

         // skip invalid players
         if (pGame->IsValidEdict( pPlayer ) )
         {
			if( !pBot->IsValidEnemy(pPlayer) )
			{
				continue;
			}

            if ((b_observer_mode) && !(pPlayer->v.flags & FL_FAKECLIENT))
               continue;

            // is team play enabled?
			if (pGame->IsTeamPlay())
            {
               int player_team = UTIL_GetTeam(pPlayer);
               int bot_team = UTIL_GetTeam(pEdict);

               // don't target your teammates...
               if (bot_team == player_team)
                  continue;
            }

            vecEnd = pPlayer->v.origin + pPlayer->v.view_ofs;

            // see if bot can see the player...
            if (FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ))
            {
               float distance = (pPlayer->v.origin - pEdict->v.origin).Length();
               if (distance < nearestdistance)
               {
                  nearestdistance = distance;
                  pNewEnemy = pPlayer;

                  pBot->pBotUser = NULL;  // don't follow user when enemy found
               }
            }
         }
      }
   }

	if (pNewEnemy)
	{
		// face the enemy
		Vector v_enemy = pNewEnemy->v.origin - pEdict->v.origin;
		Vector bot_angles = UTIL_VecToAngles( v_enemy );

		pEdict->v.ideal_yaw = bot_angles.y;

		BotFixIdealYaw(pEdict);

		// keep track of when we last saw an enemy
		pBot->f_bot_see_enemy_time = gpGlobals->time;
	}
	else if( mod_id == NS_DLL )
	{
		pNewEnemy = ((NSBot *)pBot)->FindEnemy();
	}

	// has the bot NOT seen an ememy for at least 5 seconds (time to reload)?
	if ((pBot->f_bot_see_enemy_time > 0) && ((pBot->f_bot_see_enemy_time + 5.0) <= gpGlobals->time))
	{
		pBot->f_bot_see_enemy_time = -1;  // so we won't keep reloading

		// TODO: does reloading cause ammo to be lost in Day of Defeat?
		if( pBot->ShouldReload() )
		{
			pBot->Reload();
		}
	}

	return pNewEnemy;
}


// specifing a weapon_choice allows you to choose the weapon the bot will
// use (assuming enough ammo exists for that weapon)
// BotFireWeapon will return TRUE if weapon was fired, FALSE otherwise

bool BotFireWeapon( Vector v_enemy, bot_t *pBot, int weapon_choice)
{
	bot_weapon_select_t *pSelect = NULL;
	int select_index;
	int iId;
	bool use_primary;
	bool use_secondary;
	int use_percent;
	int primary_percent;

	edict_t *pEdict = pBot->pEdict;

	// how far away is the enemy?
	float distance = v_enemy.Length();

	if (mod_id == VALVE_DLL)
	{
		pSelect = &valve_weapon_select[0];
	}
	else if (mod_id == GEARBOX_DLL)
	{
		pSelect = &gearbox_weapon_select[0];
	}
	else if (mod_id == CSTRIKE_DLL)
	{
		pSelect = &cs_weapon_select[0];
	}
	else if (mod_id == DOD_DLL)
	{
		pSelect = &dod_weapon_select[0];
	}
	else if (mod_id == TFC_DLL)
	{
		pSelect = &tfc_weapon_select[0];
	}
	else if (pGame->IsGunmanChronicles())
	{
		pSelect = &gunman_weapon_select[0];
	}
	else if (mod_id == NS_DLL)
	{
		pSelect = &ns_weapon_select[0];
	}
	else if (mod_id == HUNGER_DLL)
	{
		pSelect = &hunger_weapon_select[0];
	}
	else if (mod_id == SHIP_DLL)
	{
		pSelect = &ship_weapon_select[0];
	}

   if (pSelect)
   {
      // are we charging the primary fire?
      if (pBot->f_primary_charging > 0)
      {
         iId = pBot->charging_weapon_id;

         if (mod_id == TFC_DLL)
         {
            if (iId == TF_WEAPON_SNIPERRIFLE)
            {
               pBot->SetSpeed( 0.0 );  // don't move while using sniper rifle
            }
         }

         // is it time to fire the charged weapon?
         if (pBot->f_primary_charging <= gpGlobals->time)
         {
            // we DON'T set pEdict->v.button here to release the
            // fire button which will fire the charged weapon

            pBot->f_primary_charging = -1;  // -1 means not charging

            // find the correct fire delay for this weapon
            select_index = 0;

            while ((pSelect[select_index].iId) && (pSelect[select_index].iId != iId))
               select_index++;

            pBot->f_shoot_time = gpGlobals->time + pSelect[select_index].fPrimaryRate;

            return TRUE;
         }
         else
         {
            pEdict->v.button |= IN_ATTACK;   // charge the weapon
            pBot->f_shoot_time = gpGlobals->time;  // keep charging

            return TRUE;
         }
      }

      // are we charging the secondary fire?
      if (pBot->f_secondary_charging > 0)
      {
         iId = pBot->charging_weapon_id;

         // is it time to fire the charged weapon?
         if (pBot->f_secondary_charging <= gpGlobals->time)
         {
            // we DON'T set pEdict->v.button here to release the
            // fire button which will fire the charged weapon

            pBot->f_secondary_charging = -1;  // -1 means not charging

            // find the correct fire delay for this weapon
            select_index = 0;

            while ((pSelect[select_index].iId) && (pSelect[select_index].iId != iId))
               select_index++;

            pBot->f_shoot_time = gpGlobals->time + pSelect[select_index].fSecondaryRate;

            return TRUE;
         }
         else
         {
            pEdict->v.button |= IN_ATTACK2;  // charge the weapon
            pBot->f_shoot_time = gpGlobals->time;  // keep charging

            return TRUE;
         }
      }

      select_index = 0;

      // loop through all the weapons until terminator is found...
      while (pSelect[select_index].iId)
      {
		// was a weapon choice specified? (and if so do they NOT match?)
		if( ( weapon_choice != 0 ) && ( weapon_choice != pSelect[select_index].iId ) )
		{
			select_index++;  // skip to next weapon
			continue;
		}

		// if it's a melee weapon and melee weapons have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_MELEE) && bot_use_melee.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a pistol and pistols have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_PISTOL) && bot_use_pistol.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a shotgun and shotguns have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_SHOTGUN) && bot_use_shotgun.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a machine gun and machine guns have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_MACHINEGUN) && bot_use_machinegun.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a rifle and rifles have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_RIFLE) && bot_use_rifle.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a sniping weapon and sniping weapons have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_SNIPER) && bot_use_sniper.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a rocket launcher and rocket launchers have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_ROCKETLAUNCHER) && bot_use_rocketlauncher.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's an energy weapon and energy weapons have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_ENERGY) && bot_use_energy.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's an organic weapon and organic weapons have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_ORGANIC) && bot_use_organic.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a grenade and grenades have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_GRENADE) && bot_use_grenade.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

		// if it's a chemical weapon and chemical weapons have been disabled for bots
		if( (pSelect[select_index].flags & WEAPON_CHEMICAL) && bot_use_chemical.value < 1 )
		{
			// skip to next weapon
			select_index++;
			continue;
		}

        // is the bot NOT carrying this weapon?
        if( !( pEdict->v.weapons & (1 << pSelect[select_index].iId) ) )
        {
			select_index++;  // skip to next weapon
			continue;
        }   

#if 0
         // is the bot NOT skilled enough to use this weapon?
         if ((pBot->GetSkill() +1) > pSelect[select_index].skill_level)
         {
            select_index++;  // skip to next weapon
            continue;
         }
#endif

         // is the bot underwater and does this weapon NOT work under water?
         if ((pBot->IsUnderWater()) && !(pSelect[select_index].can_use_underwater))
         {
            select_index++;  // skip to next weapon
            continue;
         }

         use_percent = RANDOM_LONG(1, 100);

         // is use percent greater than weapon use percent?
         if (use_percent > pSelect[select_index].use_percent)
         {
            select_index++;  // skip to next weapon
            continue;
         }

         iId = pSelect[select_index].iId;
         use_primary = FALSE;
         use_secondary = FALSE;
         primary_percent = RANDOM_LONG(1, 100);

         // is primary percent less than weapon primary percent AND
         // no ammo required for this weapon OR
         // enough ammo available to fire AND
         // the bot is far enough away to use primary fire AND
         // the bot is close enough to the enemy to use primary fire

         if ((primary_percent <= pSelect[select_index].primary_fire_percent) && ((weapon_defs[iId].iAmmo1 == -1) ||
              (pBot->m_rgAmmo[weapon_defs[iId].iAmmo1] >= pSelect[select_index].min_primary_ammo)) &&
             (distance >= pSelect[select_index].primary_min_distance) && (distance <= pSelect[select_index].primary_max_distance))
         {
            use_primary = TRUE;
         }

         // otherwise see if there is enough secondary ammo AND
         // the bot is far enough away to use secondary fire AND
         // the bot is close enough to the enemy to use secondary fire

         else if (((weapon_defs[iId].iAmmo2 == -1) || (pBot->m_rgAmmo[weapon_defs[iId].iAmmo2] >= pSelect[select_index].min_secondary_ammo)) &&
                  (distance >= pSelect[select_index].secondary_min_distance) && (distance <= pSelect[select_index].secondary_max_distance))
         {
            use_secondary = TRUE;
         }

         // see if there wasn't enough ammo to fire the weapon...
         if ((use_primary == FALSE) && (use_secondary == FALSE))
         {
            select_index++;  // skip to next weapon
            continue;
         }

         // select this weapon if it isn't already selected
         if (pBot->current_weapon.iId != iId)
		{
            UTIL_SelectItem(pEdict, pSelect[select_index].weapon_name);
		}

         if (mod_id == TFC_DLL)
         {
            if (iId == TF_WEAPON_SNIPERRIFLE)
            {
               pBot->SetSpeed( 0.0 );  // don't move while using sniper rifle

               if (pEdict->v.velocity.Length() > 50)
               {
                  return FALSE;  // don't press attack key until velocity is < 50
               }
            }

            if (pEdict->v.playerclass == TFCBot::CLASS_MEDIC)
            {
               int player_team = UTIL_GetTeam(pBot->pBotEnemy);
               int bot_team = UTIL_GetTeam(pEdict);

               // only heal your teammates or allies...
               if (((bot_team == player_team) || (team_allies[bot_team] & (1<<player_team))) && (iId != TF_WEAPON_MEDIKIT))
               {
                  return FALSE;  // don't "fire" unless weapon is medikit
               }
            }
         }

         if (use_primary)
         {
            pEdict->v.button |= IN_ATTACK;  // use primary attack

            if (pSelect[select_index].primary_fire_charge)
            {
               pBot->charging_weapon_id = iId;

               // release primary fire after the appropriate delay...
               pBot->f_primary_charging = gpGlobals->time + pSelect[select_index].primary_charge_delay;

               pBot->f_shoot_time = gpGlobals->time;  // keep charging
            }
            else
            {
               // set next time to shoot
               if (pSelect[select_index].primary_fire_hold)
			   {
                  pBot->f_shoot_time = gpGlobals->time;  // don't let button up
			   }
               else
               {
                  pBot->f_shoot_time = gpGlobals->time + pSelect[select_index].fPrimaryRate;
               }
            }
         }
         else  // MUST be use_secondary...
         {
            pEdict->v.button |= IN_ATTACK2;  // use secondary attack

            if (pSelect[select_index].secondary_fire_charge)
            {
               pBot->charging_weapon_id = iId;

               // release secondary fire after the appropriate delay...
               pBot->f_secondary_charging = gpGlobals->time + pSelect[select_index].secondary_charge_delay;

               pBot->f_shoot_time = gpGlobals->time;  // keep charging
            }
            else
            {
               // set next time to shoot
               if (pSelect[select_index].secondary_fire_hold)
			   {
                  pBot->f_shoot_time = gpGlobals->time;  // don't let button up
			   }
               else
               {
                  pBot->f_shoot_time = gpGlobals->time + pSelect[select_index].fSecondaryRate;
               }
            }
         }

         return TRUE;  // weapon was fired
      }
   }

   // didn't have any available weapons or ammo, return FALSE
   return FALSE;
}


void BotShootAtEnemy( bot_t *pBot )
{
	edict_t *pEdict = pBot->pEdict;

	// aim for the head and/or body
	Vector v_enemy = pBot->GetPointToShootAt() - GetGunPosition(pEdict);

	pEdict->v.v_angle = UTIL_VecToAngles( v_enemy );

	if (pEdict->v.v_angle.y > 180)
		pEdict->v.v_angle.y -=360;

	// Paulo-La-Frite - START bot aiming bug fix
	if (pEdict->v.v_angle.x > 180)
		pEdict->v.v_angle.x -=360;

	// set the body angles to point the gun correctly
	pEdict->v.angles.x = pEdict->v.v_angle.x / 3;
	pEdict->v.angles.y = pEdict->v.v_angle.y;
	pEdict->v.angles.z = 0;

	// adjust the view angle pitch to aim correctly (MUST be after body v.angles stuf)
	pEdict->v.v_angle.x = -pEdict->v.v_angle.x;
	// Paulo-La-Frite - END

	pEdict->v.ideal_yaw = pEdict->v.v_angle.y;

	BotFixIdealYaw(pEdict);

	v_enemy.z = 0;  // ignore z component (up & down)

	pBot->SetSpeed( pBot->GetSpeedToEnemy() );

	// is it time to shoot yet?
	if (pBot->f_shoot_time <= gpGlobals->time)
	{
		// usable weapons is weapons that should be used i.e. it won't pick a weapon
		// if the enemy is too far away - really usable weapons are weapons that are
		// definitely usable and only checks conditions like ammo and whether the
		// bot is under water etc
		std::vector<weapon_t> usableWeapons = pBot->GetUsableWeapons( false );
		std::vector<weapon_t> reallyUsableWeapons = pBot->GetUsableWeapons( true );

		int choice = 0;

		if( pGame->IsGunmanChronicles() )
		{
			// select the best weapon to use at this distance and fire...
			if( usableWeapons.size() > 0 )
			{
				choice = usableWeapons.back().iWeaponId;
				BotFireWeapon(v_enemy, pBot, choice);
			}
			else if( reallyUsableWeapons.size() > 0 )
			{
				choice = reallyUsableWeapons.back().iWeaponId;

				BotFireWeapon(v_enemy, pBot, choice);
			}
			else
			{
				ALERT( at_console, "Bot couldn't find a weapon to use.\n" );
			}
		}
		else
		{
			BotFireWeapon(v_enemy, pBot, choice);
		}
	}
}


bool BotShootTripmine( bot_t *pBot )
{
	edict_t *pEdict = pBot->pEdict;

	if (pBot->b_shoot_tripmine != TRUE)
	{
		return FALSE;
	}

	// aim at the tripmine and fire the glock...
	Vector v_enemy = pBot->v_tripmine - GetGunPosition( pEdict );

	pEdict->v.v_angle = UTIL_VecToAngles( v_enemy );

	if (pEdict->v.v_angle.y > 180)
	{
		pEdict->v.v_angle.y -=360;
	}

	// Paulo-La-Frite - START bot aiming bug fix
	if (pEdict->v.v_angle.x > 180)
	{
		pEdict->v.v_angle.x -=360;
	}

	// set the body angles to point the gun correctly
	pEdict->v.angles.x = pEdict->v.v_angle.x / 3;
	pEdict->v.angles.y = pEdict->v.v_angle.y;
	pEdict->v.angles.z = 0;

	// adjust the view angle pitch to aim correctly (MUST be after body v.angles stuf)
	pEdict->v.v_angle.x = -pEdict->v.v_angle.x;
	// Paulo-La-Frite - END

	pEdict->v.ideal_yaw = pEdict->v.v_angle.y;

	BotFixIdealYaw(pEdict);

	return BotFireWeapon( v_enemy, pBot, pBot->GetPistol() );
}

