//
// gbot - The GoldSource bot
//
// <no site>
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

extern int g_iMod;

extern bot_weapon_t weapon_defs[MAX_WEAPONS];

extern cvar_t bot_notarget;

typedef struct
{
    int iId;  // the weapon ID value
    char szName[64];  // name of the weapon when selecting it
    float fPrimaryRate;			// primary rate of fire
	float fSecondaryRate;		// secondary rate of fire
    float primary_min_distance;    // 0 = no minimum
    float primary_max_distance;    // 9999 = no maximum
    float secondary_min_distance; // 0 = no minimum
    float secondary_max_distance; // 9999 = no maximum
    int    use_percent;    // times out of 100 to use this weapon when available
    bool  can_use_underwater;      // can use this weapon underwater
    int    primary_fire_percent;    // times out of 100 to use primary fire
    int    min_primary_ammo;         // minimum ammout of primary ammo needed to fire
    int    min_secondary_ammo;      // minimum ammout of seconday ammo needed to fire
    bool  primary_fire_hold;        // hold down primary fire button to use?
    bool  secondary_fire_hold;     // hold down secondary fire button to use?
    bool  primary_fire_charge;     // charge weapon using primary fire?
    bool  secondary_fire_charge;  // charge weapon using secondary fire?
    float primary_charge_delay;    // time to charge weapon
    float secondary_charge_delay; // time to charge weapon
	int iPrimaryAmmoIndex;
	int iSecondaryAmmoIndex;
} bot_weapon_select_t;

// weapons are stored in priority order, most desired weapon should be at
// the start of the array and least desired should be at the end

bot_weapon_select_t valve_weapon_select[] = {
    {VALVE_WEAPON_CROWBAR, "weapon_crowbar", 0.3, 0.0,
	0.0, 32.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_HANDGRENADE, "weapon_handgrenade", 0.1, 0.0,
	250.0, 750.0, 0.0, 0.0,
    30, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_SNARK, "weapon_snark", 0.1, 0.0,
	150.0, 500.0, 0.0, 0.0,
    50, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_EGON, "weapon_egon", 0.0, 0.0,
	128.0, 2048.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	TRUE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_GAUSS, "weapon_gauss", 0.2, 1.0,
	0.0, 8192.0, 0.0, 8192.0,
    100, FALSE, 80, 1, 10,
	FALSE, FALSE, FALSE, TRUE, 0.0, 0.8},
    {VALVE_WEAPON_SHOTGUN, "weapon_shotgun", 0.75, 1.5,
	0.0, 150.0, 0.0, 150.0,
    100, FALSE, 70, 1, 2,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_PYTHON, "weapon_357", 0.75, 0.0, 0.0,
	8192.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_HORNETGUN, "weapon_hornetgun", 0.25, 0.0,
	0.0, 1000.0, 0.0, 1000.0,
    100, TRUE, 50, 1, 4,
	FALSE, TRUE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_MP5, "weapon_9mmAR", 0.1, 1.0,
	0.0, 8192.0, 250.0, 600.0,
    100, FALSE, 90, 1, 1,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_CROSSBOW, "weapon_crossbow", 0.75, 0.0,
	128.0, 8192.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_RPG, "weapon_rpg", 1.5, 0.0,
	250.0, 9999.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_GLOCK, "weapon_9mmhandgun", 0.3, 0.2,
	0.0, 8192.0, 0.0, 1024.0,
    100, TRUE, 70, 1, 1,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    /* terminator */
    {0, "", 0, 0.0, 0.0, 0.0, 0.0, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0, 0.0}
};

bot_weapon_select_t gunman_weapon_select[] = {
	{GUNMAN_WEAPON_DMLGRENADE, "weapon_dmlgrenade", 1.0, 0.0,
	250.0, 750.0, 0.0, 0.0,
	30, true, 100, 1, 0,
	false, false, false, false, 0.0, 0.0},
	{GUNMAN_WEAPON_BEAMGUN, "weapon_beamgun", 0.0, 0.0,
	0.0, 500.0, 0.0, 0.0,
	100, false, 100, 1, 0,
	true, false, false, false, 0.0, 0.0},
	{GUNMAN_WEAPON_MINIGUN, "weapon_minigun", 0.1, 0.0,
	0.0, 1024.0, 0.0, 0.0,
	100, true, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{GUNMAN_WEAPON_SHOTGUN, "weapon_shotgun", 0.75, 0.0,
	0.0, 150.0, 0.0, 0.0,
    100, true, 100, 2, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{GUNMAN_WEAPON_CHEMGUN, "weapon_chemgun", 1.0, 0.0,
	100.0, 400.0, 0.0, 0.0,
	100, true, 100, 2, 0,
	false, false, false, false, 0.0, 0.0},
	{GUNMAN_WEAPON_DML, "weapon_dml", 1.5, 0.0, 250.0,
	9999.0, 0.0, 0.0,
	100, true, 100, 1, 0,
	false, false, false, false, 0.0, 0.0},
	{GUNMAN_WEAPON_GAUSSPISTOL, "weapon_gausspistol", 0.1, 0.0,
	0.0, 1024.0, 0.0, 0.0,
    100, true, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{GUNMAN_WEAPON_FISTS, "weapon_fists", 0.3, 0.0,
	0.0, 32.0, 0.0, 0.0,
    100, true, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{GUNMAN_WEAPON_AICORE, "weapon_aicore", 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0, true, 100, 0, 0,
	false, false, false, false, 0.0, 0.0},
	/* terminator */
    {0, "", 0, 0.0, 0.0, 0.0, 0.0, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0, 0.0}
};

bot_weapon_select_t hunger_weapon_select[] = {
    {VALVE_WEAPON_CROWBAR, "weapon_crowbar", 0.3, 0.0,
	0.0, 32.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{HUNGER_WEAPON_TH_SPANNER, "weapon_th_shovel", 0.3, 0.0,
	0.0, 32.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{HUNGER_WEAPON_TH_SPANNER, "weapon_th_spanner", 0.3, 0.0,
	0.0, 32.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_HANDGRENADE, "weapon_handgrenade", 0.1, 0.0,
	250.0, 750.0, 0.0, 0.0,
    30, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_SNARK, "weapon_snark", 0.1, 0.0,
	150.0, 500.0, 0.0, 0.0,
    50, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_EGON, "weapon_egon", 0.0, 0.0,
	128.0, 2048.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	TRUE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_GAUSS, "weapon_gauss", 0.2, 1.0,
	0.0, 8192.0, 0.0, 8192.0,
    100, FALSE, 80, 1, 10,
	FALSE, FALSE, FALSE, TRUE, 0.0, 0.8},
    {VALVE_WEAPON_SHOTGUN, "weapon_shotgun", 0.75, 1.5,
	0.0, 150.0, 0.0, 150.0,
    100, FALSE, 70, 1, 2,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_PYTHON, "weapon_357", 0.75, 0.0,
	0.0, 8192.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{HUNGER_WEAPON_TH_TAURUS, "weapon_th_taurus", 0.75, 0.0,
	0.0, 8192.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_MP5, "weapon_9mmAR", 0.1, 1.0,
	0.0, 8192.0, 250.0, 600.0,
    100, FALSE, 90, 1, 1,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{HUNGER_WEAPON_TH_CHAINGUN, "weapon_th_chaingun", 0.1, 0.0,
	0.0, 8192.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{HUNGER_WEAPON_TH_AP9, "weapon_th_ap9", 0.1, 1.0,
	0.0, 8192.0, 0.0, 8192.0,
    100, FALSE, 70, 1, 3,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{HUNGER_WEAPON_TH_SNIPER, "weapon_th_sniper", 0.1, 0.0,
	0.0, 8192.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_CROSSBOW, "weapon_crossbow", 0.75, 0.0,
	128.0, 8192.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_RPG, "weapon_rpg", 1.5, 0.0, 250.0, 9999.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {VALVE_WEAPON_GLOCK, "weapon_9mmhandgun", 0.3, 0.2,
	0.0, 8192.0, 0.0, 1024.0,
    100, TRUE, 70, 1, 1,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    /* terminator */
    {0, "", 0, 0.0, 0.0, 0.0, 0.0, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0, 0.0}
};

bot_weapon_select_t ns_weapon_select[] = {
    {NS_WEAPON_GRENADE, "weapon_grenade", 1.0, 0.0,
	250.0, 750.0, 0.0, 0.0,
    40, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {NS_WEAPON_HEAVYMACHINEGUN, "weapon_heavymachinegun", 0.0, 0.0,
	0.0, 250.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_SHOTGUN, "weapon_shotgun", 1.3, 0.0,
	30.0, 150.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_MACHINEGUN, "weapon_machinegun", 0.0, 0.0,
	0.0, 250.0, 0.0, 0.0,
    100, FALSE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
    {NS_WEAPON_GRENADEGUN, "weapon_grenadegun", 1.2, 0.0,
	300.0, 500.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
    {NS_WEAPON_PISTOL, "weapon_pistol", 0.5, 0.0,
	0.0, 1200.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_PISTOL, "weapon_welder", 0.0, 0.0,
	0.0, 50.0, 0.0, 0.0,
    100, TRUE, 100, 1, 0,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_KNIFE, "weapon_knife", 0.7, 0.0,
	0.0, 50.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},

	// skulk
	{NS_WEAPON_BITEGUN, "weapon_bitegun", 0.8, 0.0,
	0.0, 50.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},

	// gorge
	{NS_WEAPON_SPIT, "weapon_spit", 0.8, 0.0,
	0.0, 250.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},

	// lerk
	{NS_WEAPON_BITE2GUN, "weapon_bite2gun", 0.7, 0.0,
	0.0, 50.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_SPORE, "weapon_spore", 0.5, 0.0,
	0.0, 225.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},

	// fade
	{NS_WEAPON_SWIPE, "weapon_swipe", 0.9, 0.0,
	0.0, 50.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_BLINK, "weapon_blink", 0.0, 0.0,
	50.0, 9999.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	true, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_ACIDROCKETGUN, "weapon_acidrocketgun", 1.0, 0.0, 0.0, 1200.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},

	// onos
	{NS_WEAPON_CLAWS, "weapon_claws", 0.9, 0.0,
	0.0, 50.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_STOMP, "weapon_stomp", 1.0, 0.0,
	0.0, 250.0, 0.0, 0.0,
    100, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},
	{NS_WEAPON_CHARGE, "weapon_charge", 1.0, 0.0,
	0.0, 100.0, 0.0, 0.0,
    30, TRUE, 100, 0, 0,
	FALSE, FALSE, FALSE, FALSE, 0.0, 0.0},

    /* terminator */
    {0, "", 0, 0.0, 0.0, 0.0, 0.0, 0, TRUE, 0, 1, 1, FALSE, FALSE, FALSE, FALSE, 0.0, 0.0}
};

BOOL Bot_IsTeamplay()
{
    if( g_iMod == CSTRIKE_DLL || g_iMod == DOD_DLL || g_iMod == TFC_DLL || g_iMod == NS_DLL )
	{
        return TRUE;
	}
    else
	{
		// if it's not specifically a team mod, check whether teamplay is enabled
       return (BOOL)CVAR_GET_FLOAT( "mp_teamplay" );
	}
}


edict_t *BotFindEnemy( bot_t *pBot )
{
    Vector vecEnd;
    static bool flag=TRUE;
    edict_t *pent = NULL;
    edict_t *pNewEnemy;
    float nearestdistance;

    edict_t *pEdict = pBot->pEdict;

    if( pBot->pBotEnemy )  // does the bot already have an enemy?
    {
        vecEnd = pBot->pBotEnemy->v.origin + pBot->pBotEnemy->v.view_ofs;

        // if the enemy is dead?
        if( !IsAlive( pBot->pBotEnemy ) )  // is the enemy dead?, assume bot killed it
        {
            // the enemy is dead, jump for joy about 10% of the time
            if( RANDOM_LONG(1, 100) <= 10 && g_iMod != SHIP_DLL )
			{
                pEdict->v.button |= IN_JUMP;
			}

            // don't have an enemy anymore so null out the pointer...
            pBot->pBotEnemy = NULL;
        }
        else if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
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
		else if( g_iMod == NS_DLL && !strcmp( "team_hive", STRING(pBot->pBotEnemy->v.classname) ) && pBot->pBotEnemy->v.solid == SOLID_NOT )
        {
			// stops bots getting stuck attacking a dead hive
			pBot->pBotEnemy = NULL;
		}
#if 0
		else if( g_iMod == NS_DLL && ( !FInViewCone( &vecEnd, pEdict ) || !FVisible( vecEnd, pEdict ) ) )
        {
			// stops bots getting stuck attacking nothing (why does that happen?) and allows them to evolve
			pBot->pBotEnemy = NULL;
		}
#endif
    }

    pent = NULL;
    pNewEnemy = NULL;
    nearestdistance = 1000.0;

    if( !pNewEnemy )
    {
        nearestdistance = 2500.0;

        // search the world for players...
        for( int i = 1; i <= gpGlobals->maxClients; i++ )
        {
            edict_t *pPlayer = INDEXENT(i);

            // skip invalid players and skip self (i.e. this bot)
            if( pPlayer && !pPlayer->free && ( pPlayer != pEdict ) )
            {
                // skip this player if not alive (i.e. dead or dying)
                if( !IsAlive( pPlayer ) )
				{
                    continue;
				}

				// TODO - does this work, if so why (stops players attacking nothing)
				if( !strcmp( "", STRING(pPlayer->v.netname) ) )
				{
					continue;
				}

				// TODO - fix
				// if a marine is being digested, aliens won't attack the marine inside the onos
				// if an onos is digesting, marines will ignore it
				if( pPlayer->v.iuser4 & MASK_DIGESTING )
				{
					continue;
				}

                if( bot_notarget.value && !( pPlayer->v.flags & FL_FAKECLIENT ) )
				{
                    continue;
				}

                // is team play enabled?
                if( Bot_IsTeamplay() )
                {
                    int iPlayerTeam = UTIL_GetTeam( pPlayer );
                    int iBotTeam = UTIL_GetTeam( pEdict );

                    // don't target your teammates or readyroom people
					if( iPlayerTeam == iBotTeam || pEdict->v.playerclass == PLAYMODE_READYROOM )
					{
                        continue;
					}
                }

                vecEnd = pPlayer->v.origin + pPlayer->v.view_ofs;

                // see if bot can see the player...
                if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
                {
                    float fDistance = (pPlayer->v.origin - pEdict->v.origin).Length();
                    if( fDistance < nearestdistance )
                    {
                        nearestdistance = fDistance;
                        pNewEnemy = pPlayer;

                        pBot->pBotUser = NULL;  // don't follow user when enemy found
                    }
                }
            }
        }
    }

	pent = NULL;

	if( !pNewEnemy && g_iMod == NS_DLL )
	{
		if( UTIL_GetTeam( pBot->pEdict ) == TEAM_MARINE )
		{
			while( pent = UTIL_FindEntityInSphere( pent, pEdict->v.origin, 500.0 ) )
			{
				// ignore unbuilt structures
				if( pent->v.solid != SOLID_NOT )
				{
					continue;
				}

				if( !strcmp( "alienresourcetower", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "defensechamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "movementchamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "offensechamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "sensorychamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_hive", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
			}
		}
		else
		{
			while( pent = UTIL_FindEntityInSphere( pent, pEdict->v.origin, 500.0 ) )
			{
				if( !strcmp( "team_advarmoy", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest sentry gun?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_armory", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is gun visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_armslab", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_command", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_infportal", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_observatory", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_prototypelab", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_turretfactory", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "turret", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "siegeturret", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
			}
		}
	}


    if( pNewEnemy )
    {
        // face the enemy
        Vector v_enemy = pNewEnemy->v.origin - pEdict->v.origin;
        Vector bot_angles = UTIL_VecToAngles( v_enemy );

        pEdict->v.ideal_yaw = bot_angles.y;

        BotFixIdealYaw( pEdict );

        // keep track of when we last saw an enemy
        pBot->f_bot_see_enemy_time = gpGlobals->time;
    }

    // has the bot NOT seen an ememy for at least 5 seconds (time to reload)?
    if( ( pBot->f_bot_see_enemy_time > 0.0 ) && ( ( pBot->f_bot_see_enemy_time + 5.0 ) <= gpGlobals->time ) )
    {
        pBot->f_bot_see_enemy_time = -1;  // so we won't keep reloading

        if( g_iMod != GUNMAN_DLL && g_iMod != TFC_DLL )
        {
            pEdict->v.button |= IN_RELOAD;  // press reload button
        }
    }

    return pNewEnemy;
}

edict_t *BotFindFriend( bot_t *pBot )
{
    Vector vecEnd;
    static bool flag=TRUE;
    edict_t *pent = NULL;
    edict_t *pNewFriend;
    float nearestdistance;

    edict_t *pEdict = pBot->pEdict;

    if( pBot->pBotFriend )  // does the bot already have a friend?
    {
		// TODO - check health and armour stuff and see if we can discard this player/entity

		if( g_iMod == TFC_DLL )
		{
		}
		else if( g_iMod == NS_DLL )
		{
			// TODO - make a function to get the armor given a specific configuration (instead of replicating this in a bit)
#if 0
			if( UTIL_GetTeam( pBot->pEdict ) == TEAM_MARINE )
			{
				// TODO - get the correct values!
				// heavy armor and level 3 armor
				if( ( pBot->pBotFriend->v.iuser4 & (MASK_UPGRADE_13|MASK_UPGRADE_6)) && pBot->pBotFriend->v.armorvalue < ? )
				{
					pBot->pBotFriend = NULL;
				}
				// heavy armor and level 2 armor
				else if( ( pBot->pBotFriend->v.iuser4 & (MASK_UPGRADE_13|MASK_UPGRADE_5)) && pBot->pBotFriend->v.armorvalue < ? )
				{
					pBot->pBotFriend = NULL;
				}
				// heavy armor and level 1 armor
				else if( ( pBot->pBotFriend->v.iuser4 & (MASK_UPGRADE_13|MASK_UPGRADE_4)) && pBot->pBotFriend->v.armorvalue < ? )
				{
					pBot->pBotFriend = NULL;
				}
				// heavy armor
				else if( ( pBot->pBotFriend->v.iuser4 & (MASK_UPGRADE_13)) && pBot->pBotFriend->v.armorvalue <= 200 )
				{
					pBot->pBotFriend = NULL;
				}
				// finish this...
			}
#endif
		}

        vecEnd = pBot->pBotFriend->v.origin + pBot->pBotFriend->v.view_ofs;

        // if the friend is dead?
        if( !IsAlive( pBot->pBotFriend ) )  // is the enemy dead?, assume bot killed it
        {
            // don't have a friend anymore so null out the pointer...
            pBot->pBotFriend = NULL;
        }
        else if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
        {
            // if friend is still visible and in field of view, keep it

            // face the friend
            Vector v_friend = pBot->pBotFriend->v.origin - pEdict->v.origin;
            Vector bot_angles = UTIL_VecToAngles( v_friend );

            pEdict->v.ideal_yaw = bot_angles.y;

            BotFixIdealYaw( pEdict );

            return pBot->pBotFriend;
        }
    }

    pent = NULL;
    pNewFriend = NULL;
    nearestdistance = 1000.0;

    if( !pNewFriend )
    {
        nearestdistance = 2500.0;

        // search the world for players...
        for( int i = 1; i <= gpGlobals->maxClients; i++ )
        {
            edict_t *pPlayer = INDEXENT(i);

            // skip invalid players and skip self (i.e. this bot)
            if( pPlayer && !pPlayer->free && ( pPlayer != pEdict ) )
            {
                // skip this player if not alive (i.e. dead or dying)
                if( !IsAlive( pPlayer ) )
				{
                    continue;
				}

				// TODO - does this work (I think so), if so why (stops players attacking nothing)
				if( !strcmp( "", STRING(pPlayer->v.netname) ) )
				{
					continue;
				}

				// skip real players if notarget is on
                if( bot_notarget.value && !( pPlayer->v.flags & FL_FAKECLIENT ) )
				{
                    continue;
				}

                // is team play enabled?
                if( Bot_IsTeamplay() )
                {
                    int iPlayerTeam = UTIL_GetTeam( pPlayer );
                    int iBotTeam = UTIL_GetTeam( pEdict );

                    // don't worry about enemies or readyroom people in the case of Natural Selection
					if( iPlayerTeam != iBotTeam || pEdict->v.playerclass == PLAYMODE_READYROOM )
					{
                        continue;
					}
                }

				// TODO - check health/armor values against maximums/acceptable minimums before engaging

                vecEnd = pPlayer->v.origin + pPlayer->v.view_ofs;

                // see if bot can see the player...
                if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
                {
                    float fDistance = (pPlayer->v.origin - pEdict->v.origin).Length();
                    if( fDistance < nearestdistance )
                    {
                        nearestdistance = fDistance;
                        pNewFriend = pPlayer;

                        pBot->pBotUser = NULL;  // don't follow user when enemy found
                    }
                }
            }
        }
    }

	pent = NULL;

	if( !pNewFriend && g_iMod == NS_DLL )
	{
		if( UTIL_GetTeam( pBot->pEdict ) == TEAM_ALIEN )
		{
			while( pent = UTIL_FindEntityInSphere( pent, pEdict->v.origin, 500.0 ) )
			{
				if( !strcmp( "alienresourcetower", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "defensechamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "movementchamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "offensechamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "sensorychamber", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_hive", STRING(pent->v.classname) ) )
				{
					// TODO - check if exists?
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) && pent->v.solid != SOLID_NOT )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
			}
		}
		else
		{
			while( pent = UTIL_FindEntityInSphere( pent, pEdict->v.origin, 500.0 ) )
			{
				if( !strcmp( "team_advarmoy", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest sentry gun?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_armory", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is gun visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_armslab", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_command", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_infportal", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_observatory", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_prototypelab", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "team_turretfactory", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "turret", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
				else if( !strcmp( "siegeturret", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - pEdict->v.origin).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewFriend = pent;

							pBot->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
                }
			}
		}
	}


    if( pNewFriend )
    {
        // face the enemy
        Vector v_friend = pNewFriend->v.origin - pEdict->v.origin;
        Vector bot_angles = UTIL_VecToAngles( v_friend );

        pEdict->v.ideal_yaw = bot_angles.y;

        BotFixIdealYaw( pEdict );
    }

    return pNewFriend;
}

Vector BotBodyTarget( edict_t *pBotEnemy, bot_t *pBot )
{
    Vector v_target;
    float fScale;
    int iRollOff, iPitchOff, iYawOff;

    float fDistance = (pBotEnemy->v.origin - pBot->pEdict->v.origin).Length();

    if( fDistance > 1000.0 )
	{
        fScale = 1.0;
	}
    else if( fDistance > 100.0 )
	{
        fScale = fDistance / 1000.0;
	}
    else
	{
        fScale = 0.1;
	}

    switch( pBot->bot_skill )
    {
        case 1:
			// POOR, offset for x, y, and z
            v_target = pBotEnemy->v.origin;  // aim for the body
            iRollOff = RANDOM_FLOAT( -20.0, 20.0 ) * fScale;
            iPitchOff = RANDOM_FLOAT( -20.0, 20.0 ) * fScale;
            iYawOff = RANDOM_FLOAT( -32.0, 32.0 ) * fScale;
            break;
        case 2:
            // FAIR, offset somewhat for x, y, and z
            v_target = pBotEnemy->v.origin;  // aim for the body
            iRollOff = RANDOM_FLOAT( -10.0, 10.0 ) * fScale;
            iPitchOff = RANDOM_FLOAT( -10.0, 10.0 ) * fScale;
            iYawOff = RANDOM_FLOAT( -18.0, 18.0 ) * fScale;
            break;
        case 3:
			// GOOD, offset a little for x, y, and z
            v_target = pBotEnemy->v.origin + pBotEnemy->v.view_ofs;  // aim for the head (if you can find it)
            iRollOff = RANDOM_FLOAT( -5.0, 5.0 ) * fScale;
            iPitchOff = RANDOM_FLOAT( -5.0, 5.0 ) * fScale;
            iYawOff = RANDOM_FLOAT( -10.0, 10.0 ) * fScale;
            break;
		default:
			// FAIR, offset somewhat for x, y, and z
            v_target = pBotEnemy->v.origin;  // aim for the body
            iRollOff = RANDOM_FLOAT( -10.0, 10.0 ) * fScale;
            iPitchOff = RANDOM_FLOAT( -10.0, 10.0 ) * fScale;
            iYawOff = RANDOM_FLOAT( -18.0, 18.0 ) * fScale;
            break;
    }

    v_target = v_target + Vector( iRollOff, iPitchOff, iYawOff );

    return v_target;
}


// specifing a weapon_choice allows you to choose the weapon the bot will
// use (assuming enough ammo exists for that weapon)
// BotFireWeapon will return TRUE if weapon was fired, FALSE otherwise

bool BotFireWeapon( Vector v_enemy, bot_t *pBot, int iWeapon )
{
    bot_weapon_select_t *pSelect = NULL;
    int select_index;
    int iId;
    bool use_primary;
    bool use_secondary;
    int primary_percent;

    edict_t *pEdict = pBot->pEdict;

    float fDistance = v_enemy.Length();  // how far away is the enemy?

    if( g_iMod == VALVE_DLL )
    {
        pSelect = &valve_weapon_select[0];
    }
	else if( g_iMod == GEARBOX_DLL )
	{
		pSelect = &valve_weapon_select[0];
	}
    else if( g_iMod == GUNMAN_DLL )
    {
        pSelect = &gunman_weapon_select[0];
    }
    else if( g_iMod == HUNGER_DLL )
    {
        pSelect = &hunger_weapon_select[0];
    }
	else if( g_iMod == NS_DLL )
    {
        pSelect = &ns_weapon_select[0];
    }
	else if( g_iMod == SHIP_DLL )
	{
		pSelect = &valve_weapon_select[0];
	}
	else
	{
		pSelect = &valve_weapon_select[0];
	}

    if( pSelect )
    {
        // are we charging the primary fire?
        if( pBot->f_primary_charging > 0.0 )
        {
            iId = pBot->charging_weapon_id;

            // is it time to fire the charged weapon?
            if (pBot->f_primary_charging <= gpGlobals->time)
            {
                // we DON'T set pEdict->v.button here to release the
                // fire button which will fire the charged weapon

                pBot->f_primary_charging = -1;  // -1 means not charging

                // find the correct fire delay for this weapon
                select_index = 0;

                while ((pSelect[select_index].iId) && (pSelect[select_index].iId != iId))
				{
                    select_index++;
				}

                // set next time to shoot
                int skill = pBot->bot_skill;
                float base_delay; //, min_delay, max_delay;

                base_delay = pSelect[select_index].fPrimaryRate;

                pBot->f_shoot_time = gpGlobals->time + base_delay; // + RANDOM_FLOAT(min_delay, max_delay);

                return TRUE;
            }
            else
            {
                pEdict->v.button |= IN_ATTACK;    // charge the weapon
                pBot->f_shoot_time = gpGlobals->time;  // keep charging

                return TRUE;
            }
        }

        // are we charging the secondary fire?
        if( pBot->f_secondary_charging > 0.0 )
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
				{
                    select_index++;
				}

                // set next time to shoot
                int skill = pBot->bot_skill;
                float base_delay; //, min_delay, max_delay;

				base_delay = pSelect[select_index].fSecondaryRate;

                pBot->f_shoot_time = gpGlobals->time + base_delay; // + RANDOM_FLOAT(min_delay, max_delay);

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
        while( pSelect[select_index].iId )
        {
            // was a weapon choice specified? (and if so do they NOT match?)
            if( iWeapon != 0 && iWeapon != pSelect[select_index].iId )
            {
                select_index++;  // skip to next weapon
                continue;
            }

            // is the bot NOT carrying this weapon?
            if( !(pEdict->v.weapons & (1<<pSelect[select_index].iId) ) )
            {
                select_index++;  // skip to next weapon
                continue;
            }    

            // is the bot underwater and does this weapon NOT work under water?
            if( ( pEdict->v.waterlevel == 3 ) && !pSelect[select_index].can_use_underwater )
            {
                select_index++;  // skip to next weapon
                continue;
            }

            // is use percent greater than weapon use percent?
            if( RANDOM_LONG(1, 100) > pSelect[select_index].use_percent )
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

            if((primary_percent <= pSelect[select_index].primary_fire_percent) && ((weapon_defs[iId].iAmmo1 == -1) ||
                 (pBot->m_rgAmmo[weapon_defs[iId].iAmmo1] >= pSelect[select_index].min_primary_ammo)) &&
                 (fDistance >= pSelect[select_index].primary_min_distance) &&
                 (fDistance <= pSelect[select_index].primary_max_distance) )
            {
                use_primary = TRUE;
            }

            // otherwise see if there is enough secondary ammo AND
            // the bot is far enough away to use secondary fire AND
            // the bot is close enough to the enemy to use secondary fire

            else if (((weapon_defs[iId].iAmmo2 == -1) || (pBot->m_rgAmmo[weapon_defs[iId].iAmmo2] >=
                          pSelect[select_index].min_secondary_ammo)) &&
                        (fDistance >= pSelect[select_index].secondary_min_distance) &&
                        (fDistance <= pSelect[select_index].secondary_max_distance))
            {
                use_secondary = TRUE;
            }

            // see if there wasn't enough ammo to fire the weapon...
            if( ( use_primary == FALSE ) && ( use_secondary == FALSE ) )
            {
                select_index++;  // skip to next weapon
                continue;
            }

            // select this weapon if it isn't already selected
            if( pBot->current_weapon.iId != iId )
			{
                UTIL_SelectItem( pEdict, pSelect[select_index].szName );
			}

            if( use_primary )
            {
                pEdict->v.button |= IN_ATTACK;  // use primary attack

                if( pSelect[select_index].primary_fire_charge )
                {
                    pBot->charging_weapon_id = iId;

                    // release primary fire after the appropriate delay...
                    pBot->f_primary_charging = gpGlobals->time + pSelect[select_index].primary_charge_delay;
                    pBot->f_shoot_time = gpGlobals->time;  // keep charging
                }
                else
                {
                    // set next time to shoot
                    if( pSelect[select_index].primary_fire_hold )
					{
                        pBot->f_shoot_time = gpGlobals->time;  // don't let button up
					}
                    else
                    {
                        int skill = pBot->bot_skill;
                        float base_delay; // , min_delay, max_delay;

						base_delay = pSelect[select_index].fPrimaryRate;

                        pBot->f_shoot_time = gpGlobals->time + base_delay; // + RANDOM_FLOAT(min_delay, max_delay);
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
                        int skill = pBot->bot_skill;
                        float base_delay; //, min_delay, max_delay;

						base_delay = pSelect[select_index].fSecondaryRate;
                        // min_delay = pDelay[select_index].secondary_min_delay[skill];
                        // max_delay = pDelay[select_index].secondary_max_delay[skill];

                        pBot->f_shoot_time = gpGlobals->time + base_delay; // + RANDOM_FLOAT(min_delay, max_delay);
                    }
                }
            }

            return TRUE;  // weapon was fired
        }
    }

    // didn't have any available weapons or ammo, return FALSE
    return FALSE;
}


void Bot_Shoot( bot_t *pBot, bool bIsTargetFriend )
{
	edict_t *pEdict = pBot->pEdict;

    // aim for the head and/or body
    Vector v_target;
	
	if( bIsTargetFriend )
	{
		v_target = BotBodyTarget( pBot->pBotFriend, pBot ) - GetGunPosition( pEdict );
	}
	else
	{
		v_target = BotBodyTarget( pBot->pBotEnemy, pBot ) - GetGunPosition( pEdict );
	}

    pEdict->v.v_angle = UTIL_VecToAngles( v_target );

    if( pEdict->v.v_angle.y > 180.0 )
	{
        pEdict->v.v_angle.y -=360.0;
	}

    if( pEdict->v.v_angle.x > 180.0 )
	{
        pEdict->v.v_angle.x -=360.0;
	}

    // set the body angles to point the gun correctly
    pEdict->v.angles.x = pEdict->v.v_angle.x / 3.0;
    pEdict->v.angles.y = pEdict->v.v_angle.y;
    pEdict->v.angles.z = 0.0;

    // adjust the view angle pitch to aim correctly (MUST be after body v.angles stuff)
    pEdict->v.v_angle.x = -pEdict->v.v_angle.x;

    float x = pEdict->v.v_angle.y;

    if( x > 180.0 )
	{
		x -= 360.0;
	}

#if 0
    if( abs( pEdict->v.ideal_yaw - x ) > 2.0 )
	{
        // fp = NULL;	// what is this?
	}
#endif

    pEdict->v.ideal_yaw = pEdict->v.v_angle.y;

    BotFixIdealYaw( pEdict );


    v_target.z = 0.0;  // ignore z component (up & down)

    float fDistance = v_target.Length();  // how far away is the enemy scum?

    if( fDistance > 200.0 )        // run if distance to enemy is far
	{
        pBot->f_move_speed = pBot->f_max_speed;
	}
    else if( fDistance > 20.0 )  // walk if distance is closer
	{
        pBot->f_move_speed = pBot->f_max_speed / 2.0;
	}
    else                            // don't move if close enough
	{
        pBot->f_move_speed = 0.0;
	}


    // is it time to shoot yet?
    if( pBot->f_shoot_time <= gpGlobals->time )
    {
		if( !bIsTargetFriend )
		{
			// select the best weapon to use at this distance and fire...
			BotFireWeapon( v_target, pBot, 0 );
		}
		else
		{
			if( g_iMod == TFC_DLL )
			{
				if( UTIL_GetClass( pBot->pEdict ) == PC_MEDIC )
				{
					// BotFireWeapon( v_target, pBot, NS_WEAPON_WELDER );
				}
			}
			else if( g_iMod == NS_DLL )
			{
				if( UTIL_GetTeam( pBot->pEdict ) == TEAM_MARINE )
				{
					BotFireWeapon( v_target, pBot, NS_WEAPON_WELDER );
				}
				else
				{
					BotFireWeapon( v_target, pBot, NS_WEAPON_HEALINGSPRAY );
				}
			}
		}
    }
}

// TODO - make this work for opposing force (should already), gunman chronicles, they hunger, the ship etc
bool BotShootTripmine( bot_t *pBot )
{
    edict_t *pEdict = pBot->pEdict;

    if( !pBot->b_shoot_tripmine )
	{
        return false;
	}

    // aim at the tripmine and fire the glock...
    Vector v_enemy = pBot->v_tripmine - GetGunPosition( pEdict );

    pEdict->v.v_angle = UTIL_VecToAngles( v_enemy );

    if( pEdict->v.v_angle.y > 180.0 )
	{
        pEdict->v.v_angle.y -= 360.0;
	}

    if( pEdict->v.v_angle.x > 180.0 )
	{
        pEdict->v.v_angle.x -= 360.0;
	}

    // set the body angles to point the gun correctly
    pEdict->v.angles.x = pEdict->v.v_angle.x / 3.0;
    pEdict->v.angles.y = pEdict->v.v_angle.y;
    pEdict->v.angles.z = 0.0;

    // adjust the view angle pitch to aim correctly (MUST be after body v.angles stuff)
    pEdict->v.v_angle.x = -pEdict->v.v_angle.x;

    pEdict->v.ideal_yaw = pEdict->v.v_angle.y;

    BotFixIdealYaw( pEdict );

    return BotFireWeapon( v_enemy, pBot, VALVE_WEAPON_GLOCK );
}

