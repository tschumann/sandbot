//
// gbot - The GoldSource bot
//
// <no site>
//
// bot.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

#ifndef __linux__
extern HINSTANCE hLibrary;
#else
extern void *hLibrary;
#endif

extern int g_iMod;
extern WAYPOINT waypoints[MAX_WAYPOINTS];
extern int num_waypoints;  // number of waypoints currently in use

extern bool g_bInGame;

#define PLAYER_SEARCH_RADIUS      40.0	// ns bots won't use if it's 40.0

bot_t bots[32];    // max of 32 bots in a game

extern cvar_t bot_dontshoot;
extern cvar_t bot_notarget;
extern cvar_t bot_skill;

extern cvar_t bot_command;

bot_player_t g_valveBots[32] =
{
	{"Kelly", "barney", false},
	{"Ted", "gina", false},
	{"Julie", "gman", false},
	{"Yahn", "gordon", false},
	{"Ken", "helmet", false},
	{"Steve", "hgrunt", false},
	{"Dario", "recon", false},
	{"Greg", "robo", false},
	{"Andrew", "scientist", false},
	{"Wes", "zombie", false},
	{"Matt", "barney", false},
	{"John", "gina", false},
	{"Mona Lisa", "gman", false},
	{"Mike", "gordon", false},
	{"Monica", "helmet", false},
	{"Brett", "hgrunt", false},
	{"Erik", "recon", false},
	{"Chuck", "robo", false},
	{"Phil", "scientist", false},
	{"Marc", "zombie", false},
	{"Karen", "barney", false},
	{"Dave", "gina", false},
	{"Miene", "gman", false},
	{"Randy", "gordon", false},
	{"Gabe", "helmet", false},
	{"Yatsze", "hgrunt", false},
	{"Lisa", "recon", false},
	{"Cade", "robo", false},
	{"Kate", "scientist", false},
	{"Torsten", "zombie", false},
	{"Jay", "barney", false},
	{"Harry", "gina", false}
};

bot_player_t g_gearboxBots[32] = 
{
	{"Randy", "barney", false},
	{"Brian", "beret", false},
	{"John", "cl_suit", false},
	{"Rob", "drill", false},
	{"StephenB", "fassn", false},
	{"Landon", "gina", false},
	{"DavidM", "gman", false},
	{"MikeW", "gordon", false},
	{"Patrick", "grunt", false},
	{"Steve", "helmet", false},
	{"Sean", "hgrunt", false},
	{"StephenP", "massn", false},
	{"Kristy", "otis", false},
	{"Matt", "recon", false},
	{"Sean", "recruit", false},
	{"Ben",	"robo", false},				// Sierra
	{"Chris", "scientist", false},
	{"Harry", "shephard", false},		// voices
	{"MikeS", "tower", false},
	{"Jon", "zombie", false},
	{"JohnC", "barney", false},			// Valve continued
	{"Aaron", "beret", false},
	{"Robin", "cl_suit", false},
	{"Bill", "drill", false},
	{"Douglas", "fassn", false},
	{"Jeffrey",	"gina", false},			// botman
	{"Eric", "gordon", false},			// Allstars
	{"DavidK", "grunt", false},
	{"Tom", "helmet", false},
	{"Marin", "hgrunt", false},
	{"RichardG", "massn", false},
	{"RichardC", "otis", false}
};

bot_player_t g_gunmanBots[32] = 
{
	{"Herb", "bandit", false},		// BoneWolf
	{"Steven", "general", false},	// Wipeoot
	{"Rodney", "scientist", false},	// Hostage
	{"Tomaz", "soldier", false},	// ToM
	{"Reid", "bandit", false},		// BrushBoy
	{"Dale", "general", false},		// Mutley
	{"Renier", "scientist", false},	// Lando
	{"Adrian", "soldier", false},	// PUG
	{"BrianL", "bandit", false},	// Cannelbrae
	{"BrianB", "general", false},	// Sake
	{"ChrisM", "scientist", false},	// Ilian
	{"Mary", "soldier", false},		// Spur
	{"Miriam", "bandit", false},
	{"Christopher", "general", false},	// CyberAcid
	{"Steve", "scientist", false},	// Blevo
	{"Peter", "soldier", false},	// SoulSounds
	{"ChrisL", "bandit", false},
	{"Anthony", "general", false},	// Eyeque
	{"ChrisJ", "scientist", false},
	{"Aaron", "soldier", false},
	{"David", "bandit", false},
	{"Bill", "scientist", false},
	{"Cynthia", "soldier", false},
	{"Mary Kae", "bandit", false},
	{"Eric", "general", false},
	{"Dex", "scientist", false},
	{"Jim", "soldier", false},
	{"Ilian", "bandit", false},
	{"Joe", "general", false},
	{"Jeff", "scientist", false},
	{"Yahn", "bandit", false},
	{"Robin", "general", false}
};

bot_player_t g_hungerBots[32] =
{
	{"Bill", "civie", false},
	{"Dave", "dave", false},
	{"Einar", "einar", false},
	{"BDog", "einarhev", false},
	{"Eric", "franklin", false},
	{"Sebastian", "gangster", false},
	{"Jack", "jack", false},
	{"Magnus", "magnus", false},
	{"Neil", "neil", false},
	{"Aaron", "nohead", false},
	{"Ted", "nypdcop", false},
	{"Per", "orderly", false},
	{"Petter", "patient", false},
	{"Lo", "paul", false},
	{"Richard", "sheriff", false},
	{"Minh", "worker", false},
	{"Fabian", "zork", false},
	{"Dik", "civie", false},
	{"Bennet", "dave", false},
	{"David", "einar", false},
	{"Pete", "franklin", false},
	{"Nick", "gangster", false},
	{"Margaret", "jack", false},
	{"Jerry", "magnus", false},
	{"Andrew", "nohead", false},
	{"Don", "nypdcop", false},
	{"John", "orderly", false},
	{"Morgan", "patient", false},
	{"Jiang", "paul", false},
	{"Dennis", "sheriff", false},
	{"Brian", "worker", false},
	{"Ben", "zork", false}
};

bot_player_t g_nsBots[32] = 
{
	{"Charlie", NULL, false},
	{"Jon", NULL, false},
	{"Joel", NULL, false},
	{"David", NULL, false},
	{"Harry", NULL, false},
	{"Petter", NULL, false},
	{"Jim", NULL, false},
	{"Jason", NULL, false},
	{"Cory", NULL, false},
	{"Josh", NULL, false},
	{"Mike", NULL, false},
	{"Jeff", NULL, false},
	{"Alex", NULL, false},
	{"Phil", NULL, false},
	{"Florian", NULL, false},
	{"Karl", NULL, false},
	{"Joe", NULL, false},
	{"Max", NULL, false},
	{"Kevin", NULL, false},
	{"Ned", NULL, false},
	{"Guy", NULL, false},
	{"Spencer", NULL, false},
	{"Tom", NULL, false},
	{"Joseph", NULL, false},
	{"Ozgur", NULL, false},
	{"Jeremy", NULL, false},
	{"Hugo", NULL, false},
	{"Lani", NULL, false},
	{"Oliver", NULL, false},
	{"Emmanuel", NULL, false},
	{"Matt", NULL, false},
	{"Nick", NULL, false}
};

bot_player_t g_shipBots[32] = 
{
	{"Chris", NULL, false},
	{"Ailsa", NULL, false},
	{"Ed", NULL, false},
	{"Simon", NULL, false},
	{"Jason", NULL, false},
	{"Neil", NULL, false},
	{"Andy", NULL, false},
	{"David", NULL, false},
	{"Jock", NULL, false},
	{"Gayle", NULL, false},
	{"Debbie", NULL, false},
	{"Mike", NULL, false},
	{"Nick", NULL, false},
	{"Mark", NULL, false},
	{"Dorota", NULL, false},
	{"Jonathon", NULL, false},
	{"Julian", NULL, false},
	{"Richard", NULL, false},
	{"Duncan", NULL, false},
	{"Rebecca", NULL, false},
	{"Andrea", NULL, false},
	{"Julie", NULL, false},
	{"Glen", NULL, false},
	{"Ellis", NULL, false},
	{"Edward", NULL, false},
	{"Alek", NULL, false},
	{"Casey", NULL, false},
	{"Suzie", NULL, false},
	{"Caro", NULL, false},
	{"Reanne", NULL, false},
	{"Lucy", NULL, false},
	{"Toby", NULL, false}
};

// TheFatal's method for calculating the msecval
extern int msecnum;
extern float msecdel;
extern float msecval;

// this is the LINK_ENTITY_TO_CLASS function that creates a player (bot)
void player( entvars_t *pev )
{
    static LINK_ENTITY_FUNC otherClassName = NULL;

    if( !otherClassName )
	{
        otherClassName = (LINK_ENTITY_FUNC)GetProcAddress( hLibrary, "player" );
	}
    if( otherClassName )
    {
        (*otherClassName)( pev );
    }
}

void Bot_ForgetWaypoints( bot_t *pBot )
{
	for( int i = 0; i < PREVIOUS_WAYPOINTS; i++ )
	{
		pBot->prev_waypoint_index[i] = -1;
	}

	pBot->curr_waypoint_index = -1;
}


void BotSpawnInit( bot_t *pBot )
{
    pBot->v_prev_origin = Vector( 9999.0, 9999.0, 9999.0 );
    pBot->prev_time = gpGlobals->time;

    pBot->waypoint_origin = Vector( 0, 0, 0 );
    pBot->f_waypoint_time = 0.0;
    pBot->curr_waypoint_index = -1;

	Bot_ForgetWaypoints( pBot );

    pBot->f_random_waypoint_time = gpGlobals->time;
    pBot->waypoint_goal = -1;
    pBot->f_waypoint_goal_time = 0.0;
    pBot->waypoint_near_flag = FALSE;
    pBot->waypoint_flag_origin = Vector(0, 0, 0);
    pBot->prev_waypoint_distance = 0.0;

    pBot->blinded_time = 0.0;

    pBot->prev_speed = 0.0;  // fake "paused" since bot is NOT stuck

    pBot->f_find_item = 0.0;

    pBot->ladder_dir = LADDER_UNKNOWN;
    pBot->f_start_use_ladder_time = 0.0;
    pBot->f_end_use_ladder_time = 0.0;
    pBot->waypoint_top_of_ladder = FALSE;

    pBot->f_wall_check_time = 0.0;
    pBot->f_wall_on_right = 0.0;
    pBot->f_wall_on_left = 0.0;
    pBot->f_dont_avoid_wall_time = 0.0;
    pBot->f_look_for_waypoint_time = 0.0;
    pBot->f_jump_time = 0.0;
    pBot->f_dont_check_stuck = 0.0;

    // pick a wander direction (50% of the time to the left, 50% to the right)
    if( RANDOM_LONG(1, 100) <= 50 )
	{
        pBot->wander_dir = WANDER_LEFT;
	}
    else
	{
        pBot->wander_dir = WANDER_RIGHT;
	}

    pBot->f_exit_water_time = 0.0;

    pBot->pBotEnemy = NULL;
	pBot->pBotFriend = NULL;
    pBot->f_bot_see_enemy_time = gpGlobals->time;
    pBot->f_bot_find_enemy_time = gpGlobals->time;
    pBot->pBotUser = NULL;
    pBot->f_bot_use_time = 0.0;
    pBot->f_sniper_aim_time = 0.0;

    pBot->f_shoot_time = gpGlobals->time;
    pBot->f_primary_charging = -1.0;
    pBot->f_secondary_charging = -1.0;
    pBot->charging_weapon_id = 0;

    pBot->f_pause_time = 0.0;
    pBot->f_sound_update_time = 0.0;
    pBot->bot_has_flag = FALSE;

    pBot->b_see_tripmine = FALSE;
    pBot->b_shoot_tripmine = FALSE;
    pBot->v_tripmine = Vector(0,0,0);

    pBot->bUseHealthCharger = false;
    pBot->fUseHealthChargerTime = 0.0;

    pBot->bUseArmorCharger = false;
    pBot->fUseHEVTime = 0.0;

    pBot->b_use_button = FALSE;
    pBot->f_use_button_time = 0;
    pBot->b_lift_moving = FALSE;

	// Gunman Chronicles
	pBot->bFists = false;
	pBot->iPistolMode = PISTOL_PULSE;
	pBot->iShotgunMode = SHOTGUN_SHOTGUN;
	pBot->bMinigunSpin = false;

	// Natural Selection
	pBot->bUseArmory = false;
	pBot->fUseArmoryTime = 0.0;

	pBot->bBuild = false;
	pBot->fBuildTime = 0.0;
	pBot->bBuildAlienResourceTower = false;
	pBot->bBuildHive = false;

	pBot->bEvolving = false;
	pBot->bEvolved = false;

	// The Ship
	pBot->bUseDoor = false;
	pBot->fUseDoorTime = 0.0;

    memset( &pBot->current_weapon, 0, sizeof(pBot->current_weapon) );
    memset( &pBot->m_rgAmmo, 0, sizeof(pBot->m_rgAmmo) );
}

// TODO - allow choice of skill and team
void BotCreate()
{
    bot_t *pBot;
	bot_player_t *pBots;

	if( g_iMod == VALVE_DLL )
	{
		pBots = g_valveBots;
	}
	else if( g_iMod == GEARBOX_DLL )
	{
		pBots = g_gearboxBots;
	}
	else if( g_iMod == GUNMAN_DLL )
	{
		pBots = g_gunmanBots;
	}
	else if( g_iMod == HUNGER_DLL )
	{
		pBots = g_hungerBots;
	}
	else if( g_iMod == NS_DLL )
	{
		pBots = g_nsBots;
	}
	else if( g_iMod == SHIP_DLL )
	{
		pBots = g_shipBots;
	}

	int iIndex = RANDOM_LONG( 0, 31 );

	// find an unused bot slot
	while( pBots[iIndex].bIsUsed == TRUE )
	{
		iIndex++;

		if( iIndex == 31 )
		{
			iIndex = 0;
		}
	}

	// create the bot
	pBots[iIndex].bIsUsed = true;
    edict_t *pBotEdict = CREATE_FAKE_CLIENT( pBots[iIndex].szName );

    if( FNullEnt( pBotEdict ) )
    {
        ALERT( at_console, "Unable to create bot: pfnCreateFakeClient() returned null.\n" );
    }
    else
    {
        char szMessage[128];	// allocate space for message from ClientConnect
		char szColor[8][2];		// topcolor and bottomcolor

        iIndex = 0;
        while( bots[iIndex].is_used && ( iIndex < 32 ) )
		{
            iIndex++;
		}

        // no slots left
        if( iIndex >= 32 )
        {
            ALERT( at_console, "Unable to create bot: No more slots.\n" );
            return;
        }

        // create the player entity by calling MOD's player function
        // (from LINK_ENTITY_TO_CLASS for player object)
        player( VARS(pBotEdict) );

        char *pInfoBuffer = GET_INFOBUFFER( pBotEdict );
        int iClientIndex = ENTINDEX( pBotEdict );

		// set the bot's model where possible
        if( g_iMod == VALVE_DLL || g_iMod == GEARBOX_DLL || g_iMod == GUNMAN_DLL || g_iMod == HUNGER_DLL )
		{
            SET_CLIENT_KEY_VALUE( iClientIndex, pInfoBuffer, "model", pBots[iIndex].szModel );
		}
		// otherwise set it to gordon and let the mode work it out
        else
		{
            SET_CLIENT_KEY_VALUE( iClientIndex, pInfoBuffer, "model", "gordon" );
		}

		// set the bot's color where possible
		if( ( g_iMod == VALVE_DLL || g_iMod == GEARBOX_DLL ) && !Bot_IsTeamplay() )
		{
			sprintf( szColor[0], "%f", RANDOM_FLOAT( 0, 255.0 ) );
			sprintf( szColor[1], "%f", RANDOM_FLOAT( 0, 255.0 ) );

			SET_CLIENT_KEY_VALUE( iClientIndex, pInfoBuffer, "topcolor", szColor[0] );
			SET_CLIENT_KEY_VALUE( iClientIndex, pInfoBuffer, "bottomcolor", szColor[1] );
		}

        ClientConnect( pBotEdict, pBots[iIndex].szName, "127.0.0.1", szMessage );
        ClientPutInServer( pBotEdict );

		// let the engine know this edict belongs to a bot
        pBotEdict->v.flags |= FL_FAKECLIENT;

        // initialize all the variables for this bot...
        pBot = &bots[iIndex];

        pBot->is_used = TRUE;
        pBot->respawn_state = RESPAWN_IDLE;
        pBot->create_time = gpGlobals->time;
        pBot->name[0] = 0;  // name not set by server yet

        pBot->pEdict = pBotEdict;

        pBot->not_started = 0;  // hasn't joined game yet

		pBot->start_action = 0;  // not needed for non-team MODs

        BotSpawnInit( pBot );

        pBot->need_to_initialize = FALSE;  // don't need to initialize yet

        pBotEdict->v.idealpitch = pBotEdict->v.v_angle.x;
        pBotEdict->v.ideal_yaw = pBotEdict->v.v_angle.y;
        pBotEdict->v.pitch_speed = BOT_PITCH_SPEED;
        pBotEdict->v.yaw_speed = BOT_YAW_SPEED;

        pBot->idle_angle = 0.0;
        pBot->idle_angle_time = 0.0;
        pBot->round_end = 0;

        pBot->bot_team = -1;
        pBot->bot_class = -1;
		pBot->bot_skill = (int)bot_skill.value;

		// TODO - work this out on spawn based on current status? current counts?
		// also, do different distributions for combat
		if( g_iMod == NS_DLL )
		{
			int iClass = RANDOM_LONG( 0, 10 );

			switch( iClass )
			{
			case 0:
			case 1:
				pBot->iClass = AVH_USER3_ALIEN_PLAYER1;
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				pBot->iClass = AVH_USER3_ALIEN_PLAYER2;
				break;
			case 6:
				pBot->iClass = AVH_USER3_ALIEN_PLAYER3;
				break;
			case 7:
			case 8:
			case 9:
				pBot->iClass = AVH_USER3_ALIEN_PLAYER4;
				break;
			case 10:
				pBot->iClass = AVH_USER3_ALIEN_PLAYER5;
				break;
			}
		}
    }
}


int BotInFieldOfView( bot_t *pBot, Vector dest )
{
    // get angles from source to destination
    Vector v_entity = UTIL_VecToAngles( dest );

    // make yaw angle 0 to 360 degrees if negative
    if( v_entity.y < 0.0 )
	{
        v_entity.y += 360.0;
	}

    // get bot's current view angle
    float fViewAngle = pBot->pEdict->v.v_angle.y;

    // make view angle 0 to 360 degrees if negative
    if( fViewAngle < 0.0 )
	{
        fViewAngle += 360.0;
	}

    // return the absolute value of angle to destination entity
    // zero degrees means straight ahead,  45 degrees to the left or
    // 45 degrees to the right is the limit of the normal view angle

    // rsm - START angle bug fix
    int angle = abs( (int)fViewAngle - (int)v_entity.y );

    if( angle > 180 )
	{
        angle = 360 - angle;
	}

    return angle;
    // rsm - END
}


bool BotEntityIsVisible( bot_t *pBot, Vector dest )
{
    TraceResult tr;

    // trace a line from bot's eyes to destination...
    UTIL_TraceLine( pBot->pEdict->v.origin + pBot->pEdict->v.view_ofs, dest, ignore_monsters, pBot->pEdict->v.pContainingEntity, &tr );

    // check if line of sight to object is not blocked (i.e. visible)
    if( tr.flFraction >= 1.0 )
	{
        return true;
	}
    else
	{
        return false;
	}
}


void BotFindItem( bot_t *pBot )
{
    edict_t *pent = NULL;
    edict_t *pPickupEntity = NULL;
    Vector pickup_origin;
    Vector entity_origin;
    float radius = 500.0;
    bool can_pickup;
    float min_distance;
    char item_name[40];
    TraceResult tr;
    Vector vecStart;
    Vector vecEnd;
    int angle_to_entity;
    edict_t *pEdict = pBot->pEdict;

    pBot->pBotPickupItem = NULL;

	/*
    // use a MUCH smaller search radius when waypoints are available
    if( (num_waypoints > 0) && (pBot->curr_waypoint_index != -1) )	// TODO - make more waypoint flags?
	{
        radius = 100.0;
	}
	
    else*/
	{
        radius = 500.0;
	}

    min_distance = radius + 1.0;

    while( pent = UTIL_FindEntityInSphere( pent, pEdict->v.origin, radius ) )
    {
        can_pickup = false;  // assume can't use it until known otherwise

        strcpy( item_name, STRING(pent->v.classname) );

        // see if this is a "func_" type of entity (func_button, etc.)...
        if( !strncmp("func_", item_name, 5) )
        {
            // BModels have 0,0,0 for origin so must use VecBModelOrigin...
            entity_origin = VecBModelOrigin( pent );

            vecStart = pEdict->v.origin + pEdict->v.view_ofs;
            vecEnd = entity_origin;

            angle_to_entity = BotInFieldOfView( pBot, vecEnd - vecStart );

            // check if entity is outside field of view (+/- 45 degrees)
            if( angle_to_entity > 45 )
			{
                continue;  // skip this item if bot can't "see" it
			}

            // check if entity is a ladder (ladders are a special case)
            // DON'T search for ladders if there are waypoints in this level...
            if( !strcmp("func_ladder", item_name) && (num_waypoints == 0) )
            {
                // force ladder origin to same z coordinate as bot since
                // the VecBModelOrigin is the center of the ladder.  For
                // LONG ladders, the center MAY be hundreds of units above
                // the bot.  Fake an origin at the same level as the bot...

                entity_origin.z = pEdict->v.origin.z;
                vecEnd = entity_origin;

                // trace a line from bot's eyes to func_ladder entity...
                UTIL_TraceLine( vecStart, vecEnd, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr );

                // check if traced all the way up to the entity (didn't hit wall)
                if( tr.flFraction >= 1.0 )
                {
                    // find distance to item for later use...
                    float distance = (vecEnd - vecStart).Length( );

                    // use the ladder about 100% of the time, if haven't
                    // used a ladder in at least 5 seconds...
                    if( ( RANDOM_LONG(1, 100) <= 100 ) && ( ( pBot->f_end_use_ladder_time + 5.0 ) < gpGlobals->time ) )
                    {
                        // if close to ladder...
                        if( distance < 100.0 )
                        {
                            // don't avoid walls for a while
                            pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                        }

                        can_pickup = TRUE;
                    }
                }
            }
            else
            {
                // trace a line from bot's eyes to func_ entity...
                UTIL_TraceLine( vecStart, vecEnd, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

                // check if traced all the way up to the entity (didn't hit wall)
                if( !strcmp( item_name, STRING(tr.pHit->v.classname) ) )
                {
                    // find distance to item for later use...
                    float distance = (vecEnd - vecStart).Length();

                    // check if entity is wall mounted health charger...
                    if( g_iMod != NS_DLL && !strcmp( "func_healthcharger", item_name ) )
                    {
                        // check if the bot can use this item and
                        // check if the recharger is ready to use (has power left)...
                        if( (pEdict->v.health < 100) && (pent->v.frame == 0) )
                        {
                            // check if flag not set...
                            if( !pBot->bUseHealthCharger )
                            {
                                // check if close enough and facing it directly...
                                if ((distance < PLAYER_SEARCH_RADIUS) && (angle_to_entity <= 10))
                                {
                                    pBot->bUseHealthCharger = true;
                                    pBot->fUseHealthChargerTime = gpGlobals->time;
                                }

                                // if close to health station...
                                if (distance < 100)
                                {
                                    // don't avoid walls for a while
                                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                                }

                                can_pickup = TRUE;
                            }
                        }
                        else
                        {
                            // don't need or can't use this item...
                            pBot->bUseHealthCharger = FALSE;
                        }
                    }

                    // check if entity is wall mounted HEV charger...
                    else if( !strcmp( "func_recharge", item_name ) )
                    {
                        // check if the bot can use this item and
                        // check if the recharger is ready to use (has power left)...
                        if ((pEdict->v.armorvalue < VALVE_MAX_NORMAL_BATTERY) && (pent->v.frame == 0))
                        {
                            // check if flag not set and facing it...
                            if( !pBot->bUseArmorCharger )
                            {
                                // check if close enough and facing it directly...
                                if ((distance < PLAYER_SEARCH_RADIUS) && (angle_to_entity <= 10))
                                {
                                    pBot->bUseArmorCharger = TRUE;
                                    pBot->fUseHEVTime = gpGlobals->time;
                                }

                                // if close to HEV recharger...
                                if (distance < 100)
                                {
                                    // don't avoid walls for a while
                                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                                }

                                can_pickup = TRUE;
                            }
                        }
                        else
                        {
                            // don't need or can't use this item...
                            pBot->bUseArmorCharger = FALSE;
                        }
                    }

                    // check if entity is a button...
                    else if( !strcmp( "func_button", item_name ) )
                    {
                        // use the button about 100% of the time, if haven't
                        // used a button in at least 5 seconds...
                        if ((RANDOM_LONG(1, 100) <= 100) && ((pBot->f_use_button_time + 5) < gpGlobals->time))
                        {
                            // check if flag not set and facing it...
                            if (!pBot->b_use_button)
                            {
                                // check if close enough and facing it directly...
                                if ((distance < PLAYER_SEARCH_RADIUS) && (angle_to_entity <= 10))
                                {
                                    pBot->b_use_button = TRUE;
                                    pBot->b_lift_moving = FALSE;
                                    pBot->f_use_button_time = gpGlobals->time;
                                }

                                // if close to button...
                                if (distance < 100)
                                {
                                    // don't avoid walls for a while
                                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                                }

                                can_pickup = TRUE;
                            }
                        }
                        else
                        {
                            // don't need or can't use this item...
                            pBot->b_use_button = FALSE;
                        }
                    }

					// check if it's Natural Selection and the entity is a resource node
                    else if( g_iMod == NS_DLL && UTIL_GetTeam( pEdict ) == TEAM_ALIEN && UTIL_GetClass( pEdict ) == AVH_USER3_ALIEN_PLAYER2 && !strcmp( "func_resource", item_name ) )
                    {
                        // check if flag not set and facing it...
                        if( !pBot->bBuildAlienResourceTower )
                        {
                            // check if close enough and facing it directly and has enough resource
                            if( ( distance < PLAYER_SEARCH_RADIUS * 2 ) && ( angle_to_entity <= 10 ) && UTIL_GetResources( pEdict ) >= kAlienResourceTowerCost )
                            {
                                pBot->bBuildAlienResourceTower = TRUE;
                            }

                            // if close
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = TRUE;
                        }
						else
						{
							// don't need or can't use this item...
							pBot->bBuildAlienResourceTower = FALSE;
						}
                    }

					// check if it's The Ship and the entity is a door
                    else if( g_iMod == SHIP_DLL && !strcmp( "func_door", item_name ) )
                    {
                        // use the button about 100% of the time, if haven't
                        // used a button in at least 5 seconds...
                        if( ( RANDOM_LONG(1, 100) <= 100 ) && ( ( pBot->f_use_button_time + 5.0 ) < gpGlobals->time ) )
                        {
                            // check if flag not set and facing it...
                            if( !pBot->bUseDoor )
                            {
                                // check if close enough and facing it directly...
                                if( ( distance < PLAYER_SEARCH_RADIUS ) && ( angle_to_entity <= 10 ) )
                                {
                                    pBot->bUseDoor = TRUE;
                                    pBot->fUseDoorTime = gpGlobals->time;
                                }

                                // if close to door...
                                if( distance < 100.0 )
                                {
                                    // don't avoid walls for a while
                                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                                }

                                can_pickup = TRUE;
                            }
                        }
                        else
                        {
                            // don't need or can't use this item...
                            pBot->bUseDoor = FALSE;
                        }
                    }
                }
            }
        }
        else  // everything else...
        {
            entity_origin = pent->v.origin;

            vecStart = pEdict->v.origin + pEdict->v.view_ofs;
            vecEnd = entity_origin;

            // find angles from bot origin to entity...
            angle_to_entity = BotInFieldOfView( pBot, vecEnd - vecStart );

            // check if entity is outside field of view (+/- 45 degrees)
            if( angle_to_entity > 45 )
			{
                continue;  // skip this item if bot can't "see" it
			}

            // check if line of sight to object is not blocked (i.e. visible)
            if( BotEntityIsVisible( pBot, vecEnd ) )
            {
                // check if entity is a weapon...
                if( !strncmp( "weapon_", item_name, 7 ) )
                {
                    if( pent->v.effects & EF_NODRAW )
                    {
                        // someone owns this weapon or it hasn't respawned yet
                        continue;
                    }

                    can_pickup = TRUE;
                }

                // check if entity is ammo...
                else if( !strncmp( "ammo_", item_name, 5 ) )
                {
                    // check if the item is not visible (i.e. has not respawned)
                    if( pent->v.effects & EF_NODRAW )
					{
                        continue;
					}

                    can_pickup = TRUE;
                }

				// Gunman Chronicles specific entities
				if( g_iMod == GUNMAN_DLL )
				{
					// check if entity is a customisation
					if( !strncmp( "cust_", item_name, 5 ) )
					{
						// check if the item is not visible (i.e. has not respawned)
						if( pent->v.effects & EF_NODRAW )
						{
							continue;
						}

						can_pickup = TRUE;
					}
					// check if entity is armor
					else if( !strcmp( "item_armor", item_name ) )
					{
						// check if the item is not visible (i.e. has not respawned)
						if( pent->v.effects & EF_NODRAW )
						{
							continue;
						}

						// check if the bot has any use for this item
						if( pEdict->v.armorvalue < VALVE_MAX_NORMAL_BATTERY )
						{
							can_pickup = TRUE;
						}
					}
				}
				// Natural Selection specific entities
				if( g_iMod == NS_DLL )
				{
					// check if entity is ammo
					if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "item_ammopack", item_name ) )
					{
						// check if the item is not visible (i.e. has not respawned)
						if( pent->v.effects & EF_NODRAW )
						{
							continue;
						}

						can_pickup = TRUE;
					}
					// check if entity is a catalyst
					else if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "item_catalyst", item_name ) )
					{
						// check if the item is not visible (i.e. has not respawned)
						if( pent->v.effects & EF_NODRAW )
						{
							continue;
						}

						can_pickup = TRUE;
					}
					// check if entity is ammo
					else if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "item_genericammo", item_name ) )
					{
						// check if the item is not visible (i.e. has not respawned)
						if( pent->v.effects & EF_NODRAW )
						{
							continue;
						}

						can_pickup = TRUE;
					}

					// check if entity is a healthkit
					else if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "item_health", item_name ) )
					{
						// check if the item is not visible (i.e. has not respawned)
						if( pent->v.effects & EF_NODRAW )
						{
							continue;
						}

						// check if the bot can make use of this item
						if( pEdict->v.health < pEdict->v.max_health )
						{
							can_pickup = TRUE;
						}
					}
					// check if entity is an alien resource tower
					else if( UTIL_GetTeam( pEdict ) == TEAM_ALIEN && UTIL_GetClass( pEdict ) == AVH_USER3_ALIEN_PLAYER2 && !strcmp( "alienresourcetower", item_name ) )
					{
						float fDistance = (vecEnd - vecStart).Length();

						if( !UTIL_IsBuilt( pent ) )
						{
							if( !pBot->bBuild )
							{
								// check if close enough and facing it directly...
								if( ( fDistance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
								{
									pBot->bBuild = true;
									pBot->fBuildTime = gpGlobals->time;
								}

								// if close to resource tower...
								if( fDistance < 100.0 )
								{
									// don't avoid walls for a while
									pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
								}

								can_pickup = true;
							}
						}
						else
						{
							pBot->bBuild = false;
						}
					}
					// check if entity is a resource tower
					else if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "resourcetower", item_name ) )
					{
						float fDistance = (vecEnd - vecStart).Length();

						if( !UTIL_IsBuilt( pent ) )
						{
							if( !pBot->bBuild )
							{
								// check if close enough and facing it directly...
								if( ( fDistance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
								{
									pBot->bBuild = true;
									pBot->fBuildTime = gpGlobals->time;
								}

								// if close to resource tower...
								if( fDistance < 100.0 )
								{
									// don't avoid walls for a while
									pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
								}

								can_pickup = true;
							}
						}
						else
						{
							pBot->bBuild = false;
						}
					}
					// check if entity is an arms lab
					else if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "team_armslab", item_name ) )
					{
						float fDistance = (vecEnd - vecStart).Length();

						if( !UTIL_IsBuilt( pent ) )
						{
							if( !pBot->bBuild )
							{
								// check if close enough and facing it directly...
								if( ( fDistance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
								{
									pBot->bBuild = true;
									pBot->fBuildTime = gpGlobals->time;
								}

								// if close to armory...
								if( fDistance < 100.0 )
								{
									// don't avoid walls for a while
									pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
								}

								can_pickup = true;
							}
						}
						else
						{
							pBot->bBuild = false;
						}
					}
					// check if entity is an advanced armory
					else if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "team_advarmory", item_name ) )
					{
						float fDistance = (vecEnd - vecStart).Length();

						if( !UTIL_IsBuilt( pent ) )
						{
							if( !pBot->bBuild )
							{
								// check if close enough and facing it directly...
								if( ( fDistance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
								{
									pBot->bBuild = true;
									pBot->fBuildTime = gpGlobals->time;
								}

								// if close to armory...
								if( fDistance < 100.0 )
								{
									// don't avoid walls for a while
									pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
								}

								can_pickup = true;
							}
						}
						else
						{
							pBot->bBuild = false;
						}
					}
					// check if entity is an armory
					else if( UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "team_armory", item_name ) )
					{
						float fDistance = (vecEnd - vecStart).Length();

						if( !UTIL_IsBuilt( pent ) )
						{
							if( !pBot->bBuild )
							{
								// check if close enough and facing it directly...
								if( ( fDistance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
								{
									pBot->bBuild = true;
									pBot->fBuildTime = gpGlobals->time;
								}

								// if close to armory...
								if( fDistance < 100.0 )
								{
									// don't avoid walls for a while
									pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
								}

								can_pickup = true;
							}
						}
						else
						{
							pBot->bBuild = false;
						}
					}
					// check if entity is a hive
					else if( UTIL_GetTeam( pEdict ) == TEAM_ALIEN && UTIL_GetClass( pEdict ) == AVH_USER3_ALIEN_PLAYER2 && !strcmp( "team_hive", item_name ) )
					{
						float fDistance = (vecEnd - vecStart).Length();

						if( !pBot->bBuildHive )
						{
							// check if close enough and facing it directly...
							if( ( fDistance < PLAYER_SEARCH_RADIUS * 12.0 ) && UTIL_GetResources( pEdict ) >= kHiveCost )
							{
								pBot->bBuildHive = true;
							}

							// if close to hive...
							if( fDistance < 100.0 )
							{
								// don't avoid walls for a while
								pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
							}

							can_pickup = true;
						}
					}
				}

                // check if entity is a battery...
                else if( !strcmp( "item_battery", item_name ) )
                {
                    // check if the item is not visible (i.e. has not respawned)
                    if( pent->v.effects & EF_NODRAW )
					{
                        continue;
					}

                    // check if the bot can use this item...
                    if( pEdict->v.armorvalue < VALVE_MAX_NORMAL_BATTERY )
                    {
                        can_pickup = TRUE;
                    }
                }

                // check if entity is a healthkit...
                else if( !strcmp( "item_healthkit", item_name ) )
                {
                    // check if the item is not visible (i.e. has not respawned)
                    if( pent->v.effects & EF_NODRAW )
					{
                        continue;
					}

                    // check if the bot can use this item...
                    if( pEdict->v.health < 100 )
                    {
                        can_pickup = TRUE;
                    }
                }

				// check if entity is an infantry portal...
                else if( g_iMod == NS_DLL && UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "team_infportal", item_name ) )
                {
					float distance = (vecEnd - vecStart).Length();

					if( !UTIL_IsBuilt( pent ) )
					{
						if( !pBot->bBuild )
                        {
                            // check if close enough and facing it directly...
                            if( ( distance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
                            {
                                pBot->bBuild = true;
								pBot->fBuildTime = gpGlobals->time;
                            }

                            // if close to armory...
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = true;
                        }
					}
					else
					{
						pBot->bBuild = false;
					}
                }

				// check if entity is an observatory...
                else if( g_iMod == NS_DLL && UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "team_observatory", item_name ) )
                {
					float distance = (vecEnd - vecStart).Length();

					if( !UTIL_IsBuilt( pent ) )
					{
						if( !pBot->bBuild )
                        {
                            // check if close enough and facing it directly...
                            if( ( distance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
                            {
                                pBot->bBuild = true;
								pBot->fBuildTime = gpGlobals->time;
                            }

                            // if close to armory...
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = true;
                        }
					}
					else
					{
						pBot->bBuild = false;
					}
                }

				// check if entity is a prototype lab...
                else if( g_iMod == NS_DLL && UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "team_prototypelab", item_name ) )
                {
					float distance = (vecEnd - vecStart).Length();

					if( !UTIL_IsBuilt( pent ) )
					{
						if( !pBot->bBuild )
                        {
                            // check if close enough and facing it directly...
                            if( ( distance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
                            {
                                pBot->bBuild = true;
								pBot->fBuildTime = gpGlobals->time;
                            }

                            // if close to armory...
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = true;
                        }
					}
					else
					{
						pBot->bBuild = false;
					}
                }

				// check if entity is a turret factory...
				else if( g_iMod == NS_DLL && UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "team_turretfactory", item_name ) )
                {
					float distance = (vecEnd - vecStart).Length();

					if( !UTIL_IsBuilt( pent ) )
					{
						if( !pBot->bBuild )
                        {
                            // check if close enough and facing it directly...
                            if( ( distance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
                            {
                                pBot->bBuild = true;
								pBot->fBuildTime = gpGlobals->time;
                            }

                            // if close to armory...
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = true;
                        }
					}
					else
					{
						pBot->bBuild = false;
					}
                }

				// check if entity is a siege turret...
				else if( g_iMod == NS_DLL && UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "siegeturret", item_name ) )
                {
					float distance = (vecEnd - vecStart).Length();

					if( !UTIL_IsBuilt( pent ) )
					{
						if( !pBot->bBuild )
                        {
                            // check if close enough and facing it directly...
                            if( ( distance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
                            {
                                pBot->bBuild = true;
								pBot->fBuildTime = gpGlobals->time;
                            }

                            // if close to armory...
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = true;
                        }
					}
					else
					{
						pBot->bBuild = false;
					}
                }

				// check if entity is a turret...
				else if( g_iMod == NS_DLL && UTIL_GetTeam( pEdict ) == TEAM_MARINE && !strcmp( "turret", item_name ) )
                {
					float distance = (vecEnd - vecStart).Length();

					if( !UTIL_IsBuilt( pent ) )
					{
						if( !pBot->bBuild )
                        {
                            // check if close enough and facing it directly...
                            if( ( distance < PLAYER_SEARCH_RADIUS * 2.0 ) /*&& ( angle_to_entity <= 10 )*/ )
                            {
                                pBot->bBuild = true;
								pBot->fBuildTime = gpGlobals->time;
                            }

                            // if close to armory...
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = true;
                        }
					}
					else
					{
						pBot->bBuild = false;
					}
                }

                // check if entity is a packed up weapons box...
                else if( !strcmp( "weaponbox", item_name ) )
                {
                    can_pickup = true;
                }

                // check if entity is the spot from RPG laser
                else if( g_iMod != NS_DLL && !strcmp( "laser_spot", item_name ) )
                {
                }

                // check if entity is an armed tripmine
                else if( !strcmp( "monster_tripmine", item_name ) )
                {
                    float distance = (pent->v.origin - pEdict->v.origin).Length( );

                    if( pBot->b_see_tripmine )
                    {
                        // see if this tripmine is closer to bot...
                        if (distance < (pBot->v_tripmine - pEdict->v.origin).Length())
                        {
                            pBot->v_tripmine = pent->v.origin;
                            pBot->b_shoot_tripmine = FALSE;

                            // see if bot is far enough to shoot the tripmine...
                            if( distance >= 375 )
                            {
                                pBot->b_shoot_tripmine = TRUE;
                            }
                        }
                    }
                    else
                    {
                        pBot->b_see_tripmine = TRUE;
                        pBot->v_tripmine = pent->v.origin;
                        pBot->b_shoot_tripmine = FALSE;

                        // see if bot is far enough to shoot the tripmine...
                        if (distance >= 375)  // 375 is damage radius
                        {
                            pBot->b_shoot_tripmine = TRUE;
                        }
                    }
                }

                // check if entity is an armed satchel charge
                else if( g_iMod != NS_DLL && !strcmp( "monster_satchel", item_name ) )
                {
                }

                // check if entity is a snark (squeak grenade)
                else if( g_iMod != NS_DLL && !strcmp( "monster_snark", item_name ) )
                {
                }

            }  // end if object is visible
        }  // end else not "func_" entity

        if( can_pickup ) // if the bot found something it can pickup...
        {
            float fDistance = ( entity_origin - pEdict->v.origin ).Length();

            // see if it's the closest item so far...
            if( fDistance < min_distance )
            {
                min_distance = fDistance;          // update the minimum distance
                pPickupEntity = pent;          // remember this entity
                pickup_origin = entity_origin;  // remember location of entity
            }
        }
    }  // end while loop

    if( pPickupEntity )
    {
        // let's head off toward that item...
        Vector v_item = pickup_origin - pEdict->v.origin;

        Vector bot_angles = UTIL_VecToAngles( v_item );

        pEdict->v.ideal_yaw = bot_angles.y;

        BotFixIdealYaw( pEdict );

		pBot->pBotPickupItem = pPickupEntity;  // save the item bot is trying to get

		if( g_iMod == SHIP_DLL )
		{
			FakeClientCommand( pEdict, "pickup", NULL, NULL );
		}
    }
}


void BotThink( bot_t *pBot )
{
    int index = 0;
    Vector v_diff;                 // vector from previous to current location
    float pitch_degrees;
    float yaw_degrees;
    float moved_distance;        // length of v_diff vector (distance bot moved)
    TraceResult tr;
    bool found_waypoint;
    bool is_idle;

    edict_t *pEdict = pBot->pEdict;

    pEdict->v.flags |= FL_FAKECLIENT;

    if( pBot->name[0] == 0 )  // name filled in yet?
	{
        strcpy( pBot->name, STRING(pBot->pEdict->v.netname) );
	}

	// join a game if in the readyroom
	if( g_iMod == NS_DLL && pEdict->v.playerclass == PLAYMODE_READYROOM )
	{
		pBot->bEvolving = false;
		pBot->bEvolved = false;
		FakeClientCommand( pEdict, "autoassign", NULL, NULL );
	}

	pBot->f_max_speed = CVAR_GET_FLOAT( "sv_maxspeed" );

    pEdict->v.button = 0;
    pBot->f_move_speed = 0.0;

#if 0	// TODO - reenable?
    // if the bot hasn't selected stuff to start the game yet, go do that...
    if( pBot->not_started )
    {
        BotStartGame( pBot );

        BotFixIdealPitch( pEdict );
        BotFixIdealYaw( pEdict );
        BotFixBodyAngles( pEdict );
        BotFixViewAngles( pEdict );

        g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, 0.0, 0, 0, pEdict->v.button, 0, msecval );

        return;
    }
#endif

    // if the bot is dead, randomly press fire to respawn...
	if( !IsAlive( pBot->pEdict ) )
    {
		// forget the last enemy
		pBot->pBotEnemy = NULL;
		// reset waypoints just in case
		Bot_ForgetWaypoints( pBot );

		// evolution is reset on death for aliens
		if( g_iMod == NS_DLL )
		{
			pBot->bEvolving = false;

			// do it differently for co_ ?
			if( !UTIL_IsCombat() )
			{
				pBot->bEvolved = false;
			}
		}

        if( pBot->need_to_initialize )
        {
            BotSpawnInit( pBot );

            // did another player kill this bot AND bot whine messages loaded AND
            // has the bot been alive for at least 15 seconds AND
            if( pBot->killer_edict && ( (pBot->f_bot_spawn_time + 15.0) <= gpGlobals->time ) )
            {
                if( RANDOM_LONG(1,100) <= 40 )
                {
					// TODO - say something...                    
                }
            }

            pBot->need_to_initialize = FALSE;
        }

        if( RANDOM_LONG(1, 100) > 50 )
		{
            pEdict->v.button = IN_ATTACK;
		}

        BotFixIdealPitch( pEdict );
        BotFixIdealYaw( pEdict );
        BotFixBodyAngles( pEdict );
        BotFixViewAngles( pEdict );

        g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, pBot->f_move_speed, 0, 0, pEdict->v.button, 0, msecval );

        return;
    }

    // set this for the next time the bot dies so it will initialize stuff
    if( pBot->need_to_initialize == FALSE )
    {
        pBot->need_to_initialize = TRUE;
        pBot->f_bot_spawn_time = gpGlobals->time;
    }

    is_idle = FALSE;

	// don't do anything while blinded
    if( pBot->blinded_time > gpGlobals->time )
    {
        is_idle = TRUE;
    }

    if( is_idle )
    {
        if (pBot->idle_angle_time <= gpGlobals->time)
        {
            pBot->idle_angle_time = gpGlobals->time + RANDOM_FLOAT(0.5, 2.0);

            pEdict->v.ideal_yaw = pBot->idle_angle + RANDOM_FLOAT(0.0, 40.0) - 20.0;

            BotFixIdealYaw(pEdict);
        }

        // turn towards ideal_yaw by yaw_speed degrees (slower than normal)
        BotChangeYaw( pBot, pEdict->v.yaw_speed / 2.0 );

        BotFixIdealPitch( pEdict );
        BotFixIdealYaw( pEdict );
        BotFixBodyAngles( pEdict );
        BotFixViewAngles( pEdict );

        g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, pBot->f_move_speed, 0, 0, pEdict->v.button, 0, msecval);

        return;
    }
    else
    {
        pBot->idle_angle = pEdict->v.v_angle.y;
    }

    // check if time to check for player sounds (if don't already have enemy)
    if ((pBot->f_sound_update_time <= gpGlobals->time) && (pBot->pBotEnemy == NULL))
    {
        edict_t *pPlayer;

        pBot->f_sound_update_time = gpGlobals->time + 1.0;

        for( int i = 1; i <= gpGlobals->maxClients; i++ )
        {
            pPlayer = INDEXENT(i);

            // is this player slot is valid and it's not this bot...
            if( pPlayer && !pPlayer->free && (pPlayer != pEdict))
            {
                // if observer mode enabled, don't listen to this player...
				if( bot_notarget.value && !( pPlayer->v.flags & FL_FAKECLIENT ) )
				{
                    continue;
				}

                if (IsAlive(pPlayer) && (FBitSet(pPlayer->v.flags, FL_CLIENT) || FBitSet(pPlayer->v.flags, FL_FAKECLIENT)))
                {
                    // check for sounds being made by other players...
                    if (UpdateSounds(pEdict, pPlayer))
                    {
                        // don't check for sounds for another 30 seconds
                        pBot->f_sound_update_time = gpGlobals->time + 30.0;
                    }
                }
            }
        }
    }

    pBot->f_move_speed = pBot->f_max_speed;  // set to max speed

    if (pBot->prev_time <= gpGlobals->time)
    {
        // see how far bot has moved since the previous position...
        v_diff = pBot->v_prev_origin - pEdict->v.origin;
        moved_distance = v_diff.Length();

        // save current position as previous
        pBot->v_prev_origin = pEdict->v.origin;
        pBot->prev_time = gpGlobals->time + 0.2;
    }
    else
    {
        moved_distance = 2.0;
    }

    // if the bot is under water, adjust pitch by pitch_speed degrees
    if ((pEdict->v.waterlevel == 2) || (pEdict->v.waterlevel == 3))
    {
        // turn towards ideal_pitch by pitch_speed degrees
        pitch_degrees = BotChangePitch( pBot, pEdict->v.pitch_speed );
    }
    else
	{
        pitch_degrees = 0.0;
	}

    // turn towards ideal_yaw by yaw_speed degrees
    yaw_degrees = BotChangeYaw( pBot, pEdict->v.yaw_speed );

    if ((pitch_degrees >= pEdict->v.pitch_speed) || (yaw_degrees >= pEdict->v.yaw_speed))
    {
        pBot->f_move_speed = 0.0;  // don't move while turning a lot
    }
    else if ((pitch_degrees >= 10) || (yaw_degrees >= 10))  // turning more than 10 degrees?
    {
        pBot->f_move_speed = pBot->f_move_speed / 4.0;  // slow down while turning
    }
    else  // else handle movement related actions...
    {
        if( !bot_dontshoot.value )
        {
			pBot->pBotEnemy = BotFindEnemy( pBot );
        }
        else
		{
            pBot->pBotEnemy = NULL;  // clear enemy pointer (no ememy for you!)
		}

        if( pBot->pBotEnemy )  // does an enemy exist?
        {
			if( g_iMod == CSTRIKE_DLL || ( g_iMod == NS_DLL && UTIL_GetTeam( pBot->pEdict ) == TEAM_MARINE ) )
			{
				ALERT( at_notice, "crouching...\n" );
				pBot->pEdict->v.button |= IN_DUCK;
				pBot->f_move_speed = 0.0;
			}

			// TODO - put this is the find enemy code and skip non-solid entities?
			if( g_iMod == NS_DLL && pBot->pBotEnemy->v.solid == SOLID_NOT )
			{
				pBot->pBotEnemy = NULL;
			}
			else
			{
				Bot_Shoot( pBot, ENEMY );  // shoot at the enemy
			}

			// dont't pause if enemy exists
            pBot->f_pause_time = 0.0;  
        }
		else
		{
			if( g_iMod == TFC_DLL )
			{
				// only medics and engineers need to worry about friendlies
				if( UTIL_GetClass( pBot->pEdict ) == PC_MEDIC || UTIL_GetClass( pBot->pEdict ) == PC_ENGINEER )
				{
					pBot->pBotFriend = BotFindFriend( pBot );
				}
			}
			else if( g_iMod == NS_DLL )
			{
				// only marines with welders or gorges with healing spray need to worry about team mates
				if( ( UTIL_GetClass( pBot->pEdict ) == AVH_USER3_MARINE_PLAYER && pBot->pEdict->v.weapons & NS_WEAPON_WELDER ) ||
					( UTIL_GetClass( pBot->pEdict ) == AVH_USER3_ALIEN_PLAYER2 && pBot->pEdict->v.weapons & NS_WEAPON_HEALINGSPRAY ) )
				{
					pBot->pBotFriend = BotFindFriend( pBot );
				}
			}
		}

		if( pBot->pBotFriend )
		{
			Bot_Shoot( pBot, FRIEND );

			// dont't pause if friend exists
			pBot->f_pause_time = 0.0;
		}

        else if( pBot->f_pause_time > gpGlobals->time )  // is bot "paused"?
        {
            // you could make the bot look left then right, or look up
            // and down, to make it appear that the bot is hunting for
            // something (don't do anything right now)
        }

        // is bot being "used" and can still follow "user"?
        else if( pBot->pBotUser && BotFollowUser( pBot ) )
        {
            // do nothing here!
        }

        else
        {
            // no enemy, let's just wander around...

			pBot->f_move_speed = pBot->f_max_speed;

            if ((pEdict->v.waterlevel != 2) &&  // is bot NOT under water?
                 (pEdict->v.waterlevel != 3))
            {
                // reset pitch to 0 (level horizontally)
                pEdict->v.idealpitch = 0;
                pEdict->v.v_angle.x = 0;
            }

            pEdict->v.v_angle.z = 0;  // reset roll to 0 (straight up and down)

            pEdict->v.angles.x = 0;
            pEdict->v.angles.y = pEdict->v.v_angle.y;
            pEdict->v.angles.z = 0;

            // check if bot should look for items now or not...
            if( pBot->f_find_item < gpGlobals->time )
            {
                BotFindItem( pBot );  // see if there are any visible items
            }

            // check if bot sees a tripmine...
            if (pBot->b_see_tripmine)
            {
                // check if bot can shoot the tripmine...
                if ((pBot->b_shoot_tripmine) && BotShootTripmine( pBot ))
                {
                    // shot at tripmine, may or may not have hit it, clear
                    // flags anyway, next BotFindItem will see it again if
                    // it is still there...

                    pBot->b_shoot_tripmine = FALSE;
                    pBot->b_see_tripmine = FALSE;

                    // pause for a while to allow tripmine to explode...
                    pBot->f_pause_time = gpGlobals->time + 0.5;
                }
                else  // run away!!!
                {
                    Vector tripmine_angles;

                    tripmine_angles = UTIL_VecToAngles( pBot->v_tripmine - pEdict->v.origin );

                    // face away from the tripmine
                    pEdict->v.ideal_yaw += 180;  // rotate 180 degrees

                    BotFixIdealYaw( pEdict );

                    pBot->b_see_tripmine = FALSE;

                    pBot->f_move_speed = 0.0;  // don't run while turning
                }
            }

            // check if should use wall mounted health station...
            else if( pBot->bUseHealthCharger )
            {
                if ((pBot->fUseHealthChargerTime + 10.0) > gpGlobals->time)
                {
                    pBot->f_move_speed = 0.0;  // don't move while using health station

                    pEdict->v.button = IN_USE;
                }
                else
                {
                    // bot is stuck trying to "use" a health station...

                    pBot->bUseHealthCharger = FALSE;

                    // don't look for items for a while since the bot
                    // could be stuck trying to get to an item
                    pBot->f_find_item = gpGlobals->time + 0.5;
                }
            }

            // check if should use wall mounted HEV station...
            else if( pBot->bUseArmorCharger )
            {
                if ((pBot->bUseArmorCharger + 10.0) > gpGlobals->time)
                {
                    pBot->f_move_speed = 0.0;  // don't move while using HEV station

                    pEdict->v.button = IN_USE;
                }
                else
                {
                    // bot is stuck trying to "use" a HEV station...

                    pBot->bUseArmorCharger = FALSE;

                    // don't look for items for a while since the bot
                    // could be stuck trying to get to an item
                    pBot->f_find_item = gpGlobals->time + 0.5;
                }
            }

			// check if should build
            else if( pBot->bBuild )
            {
                if( ( pBot->fBuildTime + 22.0 ) > gpGlobals->time )
                {
                    pBot->f_move_speed = 0.0;  // don't move

                    pEdict->v.button = ( IN_USE | IN_DUCK );
                }
                else
                {
                    pBot->bBuild = false;

                    // don't look for items for a while since the bot
                    // could be stuck trying to get to an item
                    pBot->f_find_item = gpGlobals->time + 0.5;
                }
            }
			else if( pBot->bBuildAlienResourceTower )
			{
				pEdict->v.impulse = 90;

				pBot->bBuildAlienResourceTower = false;
			}
			else if( pBot->bBuildHive )
			{
				pEdict->v.impulse = 95;

				pBot->bBuildHive = false;
			}
			// check if should use an armory...
            else if( pBot->bUseArmory )
            {
                if( ( pBot->fUseArmoryTime + 4.0 ) > gpGlobals->time )
                {
                    pBot->f_move_speed = 0.0;  // don't move

                    pEdict->v.button = IN_USE;
                }
                else
                {
                    pBot->bUseArmory = false;

                    // don't look for items for a while since the bot
                    // could be stuck trying to get to an item
                    pBot->f_find_item = gpGlobals->time + 0.5;
                }
            }
            else if( pBot->b_use_button )
            {
                pBot->f_move_speed = 0.0;  // don't move while using elevator

                BotUseLift( pBot, moved_distance );
            }
			// The Ship
			else if( pBot->bUseDoor )
            {
                BotUseDoor( pBot );
            }
            else
            {
                if( pEdict->v.waterlevel == 3 )  // check if the bot is underwater...
                {
                    BotUnderWater( pBot );
                }

                found_waypoint = FALSE;

                // if the bot is not trying to get to something AND
                // it is time to look for a waypoint AND
                // there are waypoints in this level...

                if( !pBot->pBotPickupItem && (pBot->f_look_for_waypoint_time <= gpGlobals->time) && num_waypoints )
                {
                    found_waypoint = BotHeadTowardWaypoint( pBot );
                }

                // check if the bot is on a ladder...
                if (pEdict->v.movetype == MOVETYPE_FLY)
                {
                    // check if bot JUST got on the ladder...
                    if ((pBot->f_end_use_ladder_time + 1.0) < gpGlobals->time)
					{
                        pBot->f_start_use_ladder_time = gpGlobals->time;
					}

                    // go handle the ladder movement
                    BotOnLadder( pBot, moved_distance );

                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 2.0;
                    pBot->f_end_use_ladder_time = gpGlobals->time;
                }
                else
                {
                    // check if the bot JUST got off the ladder...
                    if ((pBot->f_end_use_ladder_time + 1.0) > gpGlobals->time)
                    {
                        pBot->ladder_dir = LADDER_UNKNOWN;
                    }
                }

                // if the bot isn't headed toward a waypoint and there are no waypoints, wander
                if ( !found_waypoint && !num_waypoints )
                {
                    TraceResult tr;

                    // check if we should be avoiding walls
                    if (pBot->f_dont_avoid_wall_time <= gpGlobals->time)
                    {
                        // let's just randomly wander around
                        if( BotStuckInCorner( pBot ) )
                        {
                            pEdict->v.ideal_yaw += 180.0;  // turn 180 degrees
                
                            BotFixIdealYaw( pEdict );
                
                            pBot->f_move_speed = 0.0;  // don't move while turning
                            pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
                
                            moved_distance = 2.0;  // dont use bot stuck code
                        }
                        else
                        {
                            // check if there is a wall on the left...
                            if (!BotCheckWallOnLeft( pBot ))
                            {
                                // if there was a wall on the left over 1/2 a second ago then
                                // 20% of the time randomly turn between 45 and 60 degrees
                
                                if ((pBot->f_wall_on_left != 0) && (pBot->f_wall_on_left <= gpGlobals->time - 0.5) && (RANDOM_LONG(1, 100) <= 20))
                                {
                                    pEdict->v.ideal_yaw += RANDOM_LONG(45, 60);
                
                                    BotFixIdealYaw(pEdict);
                
                                    pBot->f_move_speed = 0.0;  // don't move while turning
                                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
                                }
                
                                pBot->f_wall_on_left = 0;  // reset wall detect time
                            }
                            else if (!BotCheckWallOnRight( pBot ))
                            {
                                // if there was a wall on the right over 1/2 a second ago then
                                // 20% of the time randomly turn between 45 and 60 degrees
                
                                if ((pBot->f_wall_on_right != 0) && (pBot->f_wall_on_right <= gpGlobals->time - 0.5) && (RANDOM_LONG(1, 100) <= 20))
                                {
                                    pEdict->v.ideal_yaw -= RANDOM_LONG(45, 60);
                
                                    BotFixIdealYaw(pEdict);
                
                                    pBot->f_move_speed = 0.0;  // don't move while turning
                                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
                                }
                
                                pBot->f_wall_on_right = 0;  // reset wall detect time
                            }
                        }
                    }
                
                    // check if bot is about to hit a wall.  TraceResult gets returned
                    if( (pBot->f_dont_avoid_wall_time <= gpGlobals->time) &&  BotCantMoveForward( pBot, &tr ) )
                    {
                        // ADD LATER
                        // need to check if bot can jump up or duck under here...
                        // ADD LATER

						if( BotCanJumpUp( pBot ) )  // can the bot jump onto something?
						{
							pEdict->v.button |= IN_JUMP;  // jump up and move forward
						}
						else if( BotCanDuckUnder( pBot ) )  // can the bot duck under something?
						{
							pEdict->v.button |= IN_DUCK;  // duck down and move forward
						}
						else
						{
							BotTurnAtWall( pBot, &tr );
						}
                    }
                }

                // check if bot is on a ladder and has been on a ladder for
                // more than 5 seconds...
                if ((pEdict->v.movetype == MOVETYPE_FLY) && (pBot->f_start_use_ladder_time > 0.0) && ((pBot->f_start_use_ladder_time + 5.0) <= gpGlobals->time))
                {
                    // bot is stuck on a ladder...

                    BotRandomTurn(pBot);

                    // don't look for items for 2 seconds
                    pBot->f_find_item = gpGlobals->time + 2.0;

                    pBot->f_start_use_ladder_time = 0.0;  // reset start ladder time
                }
                
                // check if the bot hasn't moved much since the last location
                // (and NOT on a ladder since ladder stuck handled elsewhere)
                // (don't check for stuck if f_dont_check_stuck in the future)
                if((moved_distance <= 1.0) && (pBot->prev_speed >= 1.0) && (pEdict->v.movetype != MOVETYPE_FLY) && (pBot->f_dont_check_stuck < gpGlobals->time))
                {
                    // the bot must be stuck!

                    pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
                    pBot->f_look_for_waypoint_time = gpGlobals->time + 1.0;
                
                    if( BotCanJumpUp( pBot ) )  // can the bot jump onto something?
                    {
                        if ((pBot->f_jump_time + 2.0) <= gpGlobals->time)
                        {
                            pBot->f_jump_time = gpGlobals->time;
                            pEdict->v.button |= IN_JUMP;  // jump up and move forward
                        }
                        else
                        {
                            // bot already tried jumping less than two seconds ago, just turn
                            BotRandomTurn(pBot);
                        }
                    }
                    else if( BotCanDuckUnder( pBot ) )  // can the bot duck under something?
                    {
                        pEdict->v.button |= IN_DUCK;  // duck down and move forward
                    }
                    else
                    {
                        BotRandomTurn( pBot );

                        // is the bot trying to get to an item?...
                        if (pBot->pBotPickupItem != NULL)
                        {
                            // don't look for items for a while since the bot
                            // could be stuck trying to get to an item
                            pBot->f_find_item = gpGlobals->time + 0.5;
                        }
                    }
                }
            }
        }
    }

    if( pBot->curr_waypoint_index != -1 )  // does the bot have a waypoint?
    {
        // check if the next waypoint is a door waypoint
        if( waypoints[pBot->curr_waypoint_index].flags & W_FL_DOOR )
        {
            pBot->f_move_speed = pBot->f_max_speed / 3.0;  // slow down for doors
        }

		// check if the next waypoint is a crouch waypoint
        if( waypoints[pBot->curr_waypoint_index].flags & W_FL_CROUCH )
		{
            pEdict->v.button |= IN_DUCK;  // duck down while moving forward
		}

        // check if the next waypoint is a ladder waypoint...
        if( waypoints[pBot->curr_waypoint_index].flags & W_FL_LADDER )
        {
            // check if the waypoint is at the top of a ladder AND
            // the bot isn't currenly on a ladder...
            if( (pBot->waypoint_top_of_ladder) && (pEdict->v.movetype != MOVETYPE_FLY) )
            {
                // is the bot on "ground" above the ladder?
                if( pEdict->v.flags & FL_ONGROUND )
                {
                    float waypoint_distance = (pEdict->v.origin - pBot->waypoint_origin).Length();

                    if (waypoint_distance <= 20.0)  // if VERY close...
                        pBot->f_move_speed = 20.0;  // go VERY slow
                    else if (waypoint_distance <= 100.0)  // if fairly close...
                        pBot->f_move_speed = 50.0;  // go fairly slow

                    pBot->ladder_dir = LADDER_DOWN;
                    pBot->f_dont_check_stuck = gpGlobals->time + 1.0;
                }
                else  // bot must be in mid-air, go BACKWARDS to touch ladder...
                {
                    pBot->f_move_speed = -pBot->f_max_speed;
                }
            }
            else
            {
                // don't avoid walls for a while
                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;

                pBot->waypoint_top_of_ladder = FALSE;
            }
        }

        // check if the waypoint is a sniper waypoint AND
        // bot isn't currently aiming at an ememy...
        if( (waypoints[pBot->curr_waypoint_index].flags & W_FL_SNIPER) && !pBot->pBotEnemy )
        {
            // check if it's time to adjust aim yet...
            if (pBot->f_sniper_aim_time <= gpGlobals->time)
            {
                int aim_index;

                aim_index = WaypointFindNearestAiming(waypoints[pBot->curr_waypoint_index].origin);

                if (aim_index != -1)
                {
                    Vector v_aim = waypoints[aim_index].origin - waypoints[pBot->curr_waypoint_index].origin;

                    Vector aim_angles = UTIL_VecToAngles( v_aim );

                    aim_angles.y += RANDOM_LONG(0, 30) - 15;

                    pEdict->v.ideal_yaw = aim_angles.y;

                    BotFixIdealYaw(pEdict);
                }

                // don't adjust aim again until after a few seconds...
                pBot->f_sniper_aim_time = gpGlobals->time + RANDOM_FLOAT(3.0, 5.0);
            }
        }
    }

    if( pBot->f_pause_time > gpGlobals->time )  // is the bot "paused"?
	{
        pBot->f_move_speed = 0.0;  // don't move while pausing
	}

    // make the body face the same way the bot is looking
    pEdict->v.angles.y = pEdict->v.v_angle.y;

    // save the previous speed (for checking if stuck)
    pBot->prev_speed = pBot->f_move_speed;

	if( g_iMod == GUNMAN_DLL )
	{
		if( pBot->iShotgunMode == SHOTGUN_SHOTGUN && !strcmp( STRING(pBot->pEdict->v.weaponmodel),"models/p_shotgun.mdl" ) )
		{
			FakeClientCommand( pBot->pEdict, "cust_11", NULL, NULL );
			// FakeClientCommand( pBot->pEdict, "cust_24", NULL, NULL );
			pBot->iShotgunMode = SHOTGUN_RIFLE;
		}
#if 0
		// a dropped minigun retains its spin, so a bot is effectively cooling the gun down by turning off the spin
		else if( !pBot->bMinigunSpin && !strcmp( STRING(pBot->pEdict->v.weaponmodel),"models/p_9mmAR.mdl" ) )
		{
			// TODO - work this out
			// pBot->pEdict->v.button |= IN_CUSTOM;
			// pBot->bMinigunSpin = true;
		}
#endif
	}

	if( g_iMod == NS_DLL )
	{
		// TODO - combat and classic branches, tidy up
		if( g_bInGame && UTIL_GetTeam( pBot->pEdict ) == TEAM_ALIEN )
		{
			// finish evolving
			if( UTIL_IsEvolved( pBot ) )
			{
				pBot->f_move_speed = pBot->f_max_speed;
				pBot->bEvolving = false;
				pBot->bEvolved = true;
			}

			// start evolving
			if( !UTIL_IsCombat() && UTIL_CanEvolve( pBot ) )
			{
				if( pBot->iClass == AVH_USER3_ALIEN_PLAYER2 && UTIL_GetResources( pBot->pEdict ) > (float)kGorgeCost )
				{
					pBot->f_move_speed = 0.0;
					pBot->pEdict->v.impulse = 114;
					pBot->bEvolving = true;
				}
				else if( pBot->iClass == AVH_USER3_ALIEN_PLAYER3 && UTIL_GetResources( pBot->pEdict ) > (float)kLerkCost )
				{
					pBot->f_move_speed = 0.0;
					pBot->pEdict->v.impulse = 115;
					pBot->bEvolving = true;
				}
				else if( pBot->iClass == AVH_USER3_ALIEN_PLAYER4 && UTIL_GetResources( pBot->pEdict ) > (float)kFadeCost )
				{
					pBot->f_move_speed = 0.0;
					pBot->pEdict->v.impulse = 116;
					pBot->bEvolving = true;
				}
				else if( pBot->iClass == AVH_USER3_ALIEN_PLAYER5 && UTIL_GetResources( pBot->pEdict ) > (float)kOnosCost )
				{
					pBot->f_move_speed = 0.0;
					pBot->pEdict->v.impulse = 117;
					pBot->bEvolving = true;
				}
			}
		}

		// 
		if( UTIL_IsCombat() )
		{
			if( UTIL_GetTeam( pBot->pEdict ) == TEAM_ALIEN )
			{
				// regerenation
				// carapace
				// adrenaline
				// celerity
				// other species...?
#if 0
				if( pBot->iClass == AVH_USER3_ALIEN_PLAYER2 /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 114;	// Gorge
				}	
				if( !pBot->bHasRegeneration /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 102;	// Regeneration
				}
				if( !pBot->bHasCarapace /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 101;	// Carapace
				}
#endif
			}
			else if( UTIL_GetTeam( pBot->pEdict ) == TEAM_MARINE )
			{
				// welder
				// resupply
				// armor one
				// weapons one
				// shotgun
				// armor two
#if 0
				if( !pBot->bHasResupply /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 31;	// Resupply
				}
				if( !pBot->bHasArmorOne /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 20;	// Armor One
				}
				if( !pBot->bHasDamageOne /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 23;	// Weapons Two
				}
				if( !pBot->bHasShotgun /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 64;	// Shotgun
				}
				if( !pBot->bHasArmorTwo /*&& UTIL_GetPoints( pBot->pEdict ) >= 1*/ )
				{
					pBot->pEdict->v.impulse = 21;	// Armor Two
				}
#endif
				// 
				if( pBot->pEdict->v.weapons & NS_WEAPON_WELDER )
				{
					ALERT( at_notice, "%s has a welder\n", STRING(pBot->pEdict->v.netname) );
				}

				if( !(pBot->pEdict->v.weapons & NS_WEAPON_WELDER ) )		// welder
				{
					// TODO - this isn't working?
					pBot->pEdict->v.impulse = 62;
				}
				// resupply? TODO - check for a set bit or something indicating whether resupply is on or not
				else if( !(pBot->pEdict->v.iuser4 & MASK_UPGRADE_4 ) )		// armor one
				{
					pBot->pEdict->v.impulse = 20;
				}
				else if( !(pBot->pEdict->v.iuser4 & MASK_UPGRADE_1 ) )		// weapons one
				{
					pBot->pEdict->v.impulse = 23;
				}
				else if( !(pBot->pEdict->v.weapons & NS_WEAPON_SHOTGUN ) )	// shotugn
				{
					pBot->pEdict->v.impulse = 64;
				}
				else if( !(pBot->pEdict->v.iuser4 & MASK_UPGRADE_5 ) )		// armor two
				{
					pBot->pEdict->v.impulse = 21;
				}
			}
		}
	}

    BotFixIdealPitch( pEdict );
    BotFixIdealYaw( pEdict );
    BotFixBodyAngles( pEdict );
    BotFixViewAngles( pEdict );

    g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, pBot->f_move_speed, 0, 0, pEdict->v.button, 0, msecval);

    return;
}

