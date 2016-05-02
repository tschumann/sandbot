//
// gbot - The GoldSource bot
//
// <no site>
//
// dll.cpp
//

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "entity_state.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"


#define MENU_NONE  0
#define MENU_1      1
#define MENU_2      2
#define MENU_3      3


#ifndef __linux__
extern HINSTANCE h_Library;
#else
extern void *h_Library;
#endif

extern enginefuncs_t g_engfuncs;
extern int debug_engine;
extern globalvars_t *gpGlobals;
extern char g_argv[1024];
extern bool g_waypoint_on;
extern bool g_auto_waypoint;
extern bool g_path_waypoint;
extern int num_waypoints;  // number of waypoints currently in use
extern WAYPOINT waypoints[MAX_WAYPOINTS];
extern float wp_display_time[MAX_WAYPOINTS];
extern bot_t bots[32];


static FILE *fp;

bool g_bInGame = false;
int g_iCountDown = 0;
float g_fCountDownTime = 0.0;

DLL_FUNCTIONS gFunctionTable;
NEW_DLL_FUNCTIONS gNewFunctionTable;

DLL_GLOBAL const Vector g_vecZero = Vector(0,0,0);

cvar_t bot_dontshoot = {"bot_dontshoot", "0", FCVAR_SERVER };
cvar_t bot_notarget = {"bot_notarget", "0", FCVAR_SERVER };
cvar_t bot_skill = {"bot_skill", "2", FCVAR_SERVER };

cvar_t bot_command = {"bot_command", "1", FCVAR_SERVER };

extern bot_player_t g_valveBots[32];
extern bot_player_t g_gearboxBots[32];
extern bot_player_t g_gunmanBots[32];
extern bot_player_t g_hungerBots[32];
extern bot_player_t g_nsBots[32];
extern bot_player_t g_shipBots[32];

int g_iMod = 0;
int m_spriteTexture = 0;
int isFakeClientCommand = 0;
int fake_arg_count;
float bot_check_time = 30.0;
int num_bots = 0;
int prev_num_bots = 0;
bool g_GameRules = FALSE;
edict_t *clients[32];
edict_t *pListenServer = NULL;
int g_menu_waypoint;
int g_menu_state = 0;

char team_names[MAX_TEAMS][MAX_TEAMNAME_LENGTH];
int num_teams = 0;

float respawn_time = 0.0;
bool spawn_time_reset = FALSE;


// TheFatal's method for calculating the msecval
int msecnum;
float msecdel;
float msecval;

char *show_menu_1 =
    {"Waypoint Tags\n\n1. Team Specific\n2. Wait for Lift\n3. Door\n4. Sniper Spot\n5. More..."};
char *show_menu_2 =
    {"Waypoint Tags\n\n1. Team 1\n2. Team 2\n3. Team 3\n4. Team 4\n5. CANCEL"};
char *show_menu_2_flf =
    {"Waypoint Tags\n\n1. Attackers\n2. Defenders\n\n5. CANCEL"};
char *show_menu_3 =
    {"Waypoint Tags\n\n1. Flag Location\n2. Flag Goal Location\n\n5. CANCEL"};
char *show_menu_3_flf =
    {"Waypoint Tags\n\n1. Capture Point\n2. Defend Point\n3. Prone\n\n5. CANCEL"};


void UpdateClientData( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd );

//
// DLL Functions
//

void GameDLLInit( void )
{
#if _DEBUG
	CVAR_REGISTER( &bot_dontshoot );
	CVAR_REGISTER( &bot_skill );
#endif

    for( int i = 0; i < 32; i++ )
	{
        clients[i] = NULL;
	}

    // initialize the bots array of structures...
    memset( bots, 0, sizeof(bots) );

    (*gFunctionTable.pfnGameInit)();
}

int DispatchSpawn( edict_t *pent )
{
    if( gpGlobals->deathmatch )
    {
        char *pClassname = (char *)STRING(pent->v.classname);

        if( !strcmp( pClassname, "worldspawn" ) )
        {
            // do level initialization stuff here...

            WaypointInit();
            WaypointLoad(NULL);

            PRECACHE_SOUND("weapons/xbow_hit1.wav");        // waypoint add
            PRECACHE_SOUND("weapons/mine_activate.wav");  // waypoint delete
            PRECACHE_SOUND("common/wpn_hudoff.wav");        // path add/delete start
            PRECACHE_SOUND("common/wpn_hudon.wav");         // path add/delete done
            PRECACHE_SOUND("common/wpn_moveselect.wav");  // path add/delete cancel
            PRECACHE_SOUND("common/wpn_denyselect.wav");  // path add/delete error

            m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );

            g_GameRules = TRUE;

            memset(team_names, 0, sizeof(team_names));
            num_teams = 0;

            respawn_time = 0.0;
            spawn_time_reset = FALSE;

            prev_num_bots = num_bots;
            num_bots = 0;

            bot_check_time = gpGlobals->time + 30.0;
        }
    }

    return (*gFunctionTable.pfnSpawn)( pent );
}

void DispatchThink( edict_t *pent )
{
    (*gFunctionTable.pfnThink)( pent );
}

void DispatchUse( edict_t *pentUsed, edict_t *pentOther )
{
    (*gFunctionTable.pfnUse)( pentUsed, pentOther );
}

void DispatchTouch( edict_t *pentTouched, edict_t *pentOther )
{
    (*gFunctionTable.pfnTouch)( pentTouched, pentOther );
}

void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther )
{
    (*gFunctionTable.pfnBlocked)( pentBlocked, pentOther );
}

void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd )
{
	(*gFunctionTable.pfnKeyValue)( pentKeyvalue, pkvd );
}

void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData )
{
	(*gFunctionTable.pfnSave)( pent, pSaveData );
}

int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity )
{
    return (*gFunctionTable.pfnRestore)( pent, pSaveData, globalEntity );
}

void DispatchObjectCollisionBox( edict_t *pent )
{
    (*gFunctionTable.pfnSetAbsBox)( pent );
}

void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
    (*gFunctionTable.pfnSaveWriteFields)( pSaveData, pname, pBaseData, pFields, fieldCount );
}

void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
    (*gFunctionTable.pfnSaveReadFields)( pSaveData, pname, pBaseData, pFields, fieldCount );
}

void SaveGlobalState( SAVERESTOREDATA *pSaveData )
{
    (*gFunctionTable.pfnSaveGlobalState)( pSaveData );
}

void RestoreGlobalState( SAVERESTOREDATA *pSaveData )
{
    (*gFunctionTable.pfnRestoreGlobalState)( pSaveData );
}

void ResetGlobalState( void )
{
    (*gFunctionTable.pfnResetGlobalState)();
}

BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128] )
{
    if( gpGlobals->deathmatch )
    {
		// TODO - count unused?
        int count = 0;

        // check if this client is the listen server client
        if( FStrEq( pszAddress, "loopback" ) )
        {
            // save the edict of the listen server client...
            pListenServer = pEntity;
        }

        // check if this is not a bot joining the server
        if( !FStrEq( pszAddress, "127.0.0.1" ) )
        {
            // don't try to add bots for 60 seconds, give client time to get added
            bot_check_time = gpGlobals->time + 60.0;

            for( int i = 0; i < 32; i++ )
            {
                if( bots[i].is_used )  // count the number of bots in use
				{
                    count++;
				}
            }
        }
    }

    return (*gFunctionTable.pfnClientConnect)( pEntity, pszName, pszAddress, szRejectReason );
}

void ClientDisconnect( edict_t *pEntity )
{
    if( gpGlobals->deathmatch )
    {
        int i = 0;

        while( ( i < 32 ) && ( clients[i] != pEntity ) )
		{
            i++;
		}

        if( i < 32 )
		{
            clients[i] = NULL;
		}

        for( i = 0; i < 32; i++ )
        {
            if( bots[i].pEdict == pEntity )
            {
                // someone kicked this bot off of the server...

                bots[i].is_used = false;  // this slot is now free to use

                bots[i].kick_time = gpGlobals->time;  // save the kicked time

                break;
            }
        }

		// set the bot as unused so it can be reused
		for( i = 0; i < 32; i++ )
		{
			g_valveBots[i].bIsUsed = false;
			g_gearboxBots[i].bIsUsed = false;
			g_gunmanBots[i].bIsUsed = false;
			g_hungerBots[i].bIsUsed = false;
			g_nsBots[i].bIsUsed = false;
			g_shipBots[i].bIsUsed = false;
		}

    }

    (*gFunctionTable.pfnClientDisconnect)( pEntity );
}

void ClientKill( edict_t *pEntity )
{
    (*gFunctionTable.pfnClientKill)( pEntity );
}

void ClientPutInServer( edict_t *pEntity )
{
    int i = 0;

	// find the next free slot
    while( ( i < 32 ) && ( clients[i] != NULL ) )
	{
        i++;
	}

    if( i < 32 )
	{
		// store this clients edict in the clients array
        clients[i] = pEntity; 
	}

    (*gFunctionTable.pfnClientPutInServer)( pEntity );
}

void ClientCommand( edict_t *pEntity )
{
    // only allow custom commands if deathmatch mode and NOT dedicated server and
    // client sending command is the listen server client...

    if( gpGlobals->deathmatch && !IS_DEDICATED_SERVER() && ( pEntity == pListenServer ) )
    {
        const char *pcmd = Cmd_Argv(0);
        const char *arg1 = Cmd_Argv(1);
        const char *arg2 = Cmd_Argv(2);
        const char *arg3 = Cmd_Argv(3);
        const char *arg4 = Cmd_Argv(4);
        char msg[80];

        if (debug_engine)
        {
            fopen_s(&fp,"bot.txt","a"); fprintf(fp,"ClientCommand: %s",pcmd);
            if ((arg1 != NULL) && (*arg1 != 0))
                fprintf(fp," %s", arg1);
            if ((arg2 != NULL) && (*arg2 != 0))
                fprintf(fp," %s", arg2);
            if ((arg3 != NULL) && (*arg3 != 0))
                fprintf(fp," %s", arg3);
            if ((arg4 != NULL) && (*arg4 != 0))
                fprintf(fp," %s", arg4);
            fprintf(fp, "\n");
            fclose(fp);
        }

        if (FStrEq(pcmd, "addbot"))
        {
            BotCreate();

            bot_check_time = gpGlobals->time + 5.0;

            return;
        }
#if _DEBUG
        else if (FStrEq(pcmd, "debug_engine"))
        {
            debug_engine = 1;

            ClientPrint(pEntity, HUD_PRINTNOTIFY, "debug_engine enabled!\n");

            return;
        }
#endif
        else if (FStrEq(pcmd, "waypoint"))
        {
            if (FStrEq(arg1, "on"))
            {
                g_waypoint_on = TRUE;

                ClientPrint(pEntity, HUD_PRINTNOTIFY, "waypoints are ON\n");
            }
            else if (FStrEq(arg1, "off"))
            {
                g_waypoint_on = FALSE;

                ClientPrint(pEntity, HUD_PRINTNOTIFY, "waypoints are OFF\n");
            }
            else if (FStrEq(arg1, "add"))
            {
                if (!g_waypoint_on)
                    g_waypoint_on = TRUE;  // turn waypoints on if off

                WaypointAdd(pEntity);
            }
            else if (FStrEq(arg1, "delete"))
            {
                if (!g_waypoint_on)
                    g_waypoint_on = TRUE;  // turn waypoints on if off

                WaypointDelete(pEntity);
            }
            else if (FStrEq(arg1, "save"))
            {
                WaypointSave();

                ClientPrint(pEntity, HUD_PRINTNOTIFY, "waypoints saved\n");
            }
            else if (FStrEq(arg1, "load"))
            {
                if (WaypointLoad(pEntity))
                    ClientPrint(pEntity, HUD_PRINTNOTIFY, "waypoints loaded\n");
            }
            else if (FStrEq(arg1, "menu"))
            {
                int index;

                if (num_waypoints < 1)
                    return;

                index = WaypointFindNearest(pEntity, 50.0, -1);

                if (index == -1)
                    return;

                g_menu_waypoint = index;
                g_menu_state = MENU_1;

                UTIL_ShowMenu(pEntity, 0x1F, -1, FALSE, show_menu_1);
            }
            else if (FStrEq(arg1, "info"))
            {
                WaypointPrintInfo(pEntity);
            }
            else
            {
                if (g_waypoint_on)
                    ClientPrint(pEntity, HUD_PRINTNOTIFY, "waypoints are ON\n");
                else
                    ClientPrint(pEntity, HUD_PRINTNOTIFY, "waypoints are OFF\n");
            }

            return;
        }
        else if (FStrEq(pcmd, "autowaypoint"))
        {
            if (FStrEq(arg1, "on"))
            {
                g_auto_waypoint = TRUE;
                g_waypoint_on = TRUE;  // turn this on just in case
            }
            else if (FStrEq(arg1, "off"))
            {
                g_auto_waypoint = FALSE;
            }

            if (g_auto_waypoint)
                sprintf(msg, "autowaypoint is ON\n");
            else
                sprintf(msg, "autowaypoint is OFF\n");

            ClientPrint(pEntity, HUD_PRINTNOTIFY, msg);

            return;
        }
        else if (FStrEq(pcmd, "pathwaypoint"))
        {
            if (FStrEq(arg1, "on"))
            {
                g_path_waypoint = TRUE;
                g_waypoint_on = TRUE;  // turn this on just in case

                ClientPrint(pEntity, HUD_PRINTNOTIFY, "pathwaypoint is ON\n");
            }
            else if (FStrEq(arg1, "off"))
            {
                g_path_waypoint = FALSE;

                ClientPrint(pEntity, HUD_PRINTNOTIFY, "pathwaypoint is OFF\n");
            }
            else if (FStrEq(arg1, "create1"))
            {
                WaypointCreatePath(pEntity, 1);
            }
            else if (FStrEq(arg1, "create2"))
            {
                WaypointCreatePath(pEntity, 2);
            }
            else if (FStrEq(arg1, "remove1"))
            {
                WaypointRemovePath(pEntity, 1);
            }
            else if (FStrEq(arg1, "remove2"))
            {
                WaypointRemovePath(pEntity, 2);
            }

            return;
        }
        else if (FStrEq(pcmd, "menuselect") && (g_menu_state != MENU_NONE))
        {
            if (g_menu_state == MENU_1)  // main menu...
            {
                if (FStrEq(arg1, "1"))  // team specific...
                {
                    g_menu_state = MENU_2;  // display team specific menu...

                    UTIL_ShowMenu(pEntity, 0x1F, -1, FALSE, show_menu_2);

                    return;
                }
                else if (FStrEq(arg1, "2"))  // wait for lift...
                {
                    if (waypoints[g_menu_waypoint].flags & W_FL_LIFT)
                        waypoints[g_menu_waypoint].flags &= ~W_FL_LIFT;  // off
                    else
                        waypoints[g_menu_waypoint].flags |= W_FL_LIFT;  // on
                }
                else if (FStrEq(arg1, "3"))  // door waypoint
                {
                    if (waypoints[g_menu_waypoint].flags & W_FL_DOOR)
                        waypoints[g_menu_waypoint].flags &= ~W_FL_DOOR;  // off
                    else
                        waypoints[g_menu_waypoint].flags |= W_FL_DOOR;  // on
                }
                else if (FStrEq(arg1, "4"))  // sniper spot
                {
                    if (waypoints[g_menu_waypoint].flags & W_FL_SNIPER)
                        waypoints[g_menu_waypoint].flags &= ~W_FL_SNIPER;  // off
                    else
                    {
                        waypoints[g_menu_waypoint].flags |= W_FL_SNIPER;  // on

                        // set the aiming waypoint...

                        WaypointAddAiming(pEntity);
                    }
                }
                else if (FStrEq(arg1, "5"))  // more...
                {
                    g_menu_state = MENU_3;

                    UTIL_ShowMenu(pEntity, 0x13, -1, FALSE, show_menu_3);

                    return;
                }
            }
            else if (g_menu_state == MENU_2)  // team specific menu
            {
                if (waypoints[g_menu_waypoint].flags & W_FL_TEAM_SPECIFIC)
                {
                    waypoints[g_menu_waypoint].flags &= ~W_FL_TEAM;
                    waypoints[g_menu_waypoint].flags &= ~W_FL_TEAM_SPECIFIC; // off
                }
                else
                {
                    int team = atoi(arg1);

                    team--;  // make 0 to 3

                    // this is kind of a kludge (team bits MUST be LSB!!!)
                    waypoints[g_menu_waypoint].flags |= team;
                    waypoints[g_menu_waypoint].flags |= W_FL_TEAM_SPECIFIC; // on
                }
            }
            else if (g_menu_state == MENU_3)  // second menu...
            {
                if (FStrEq(arg1, "1"))  // flag location
                {
                    if (waypoints[g_menu_waypoint].flags & W_FL_TFC_FLAG)
                        waypoints[g_menu_waypoint].flags &= ~W_FL_TFC_FLAG;  // off
                    else
                        waypoints[g_menu_waypoint].flags |= W_FL_TFC_FLAG;  // on
                }
                else if (FStrEq(arg1, "2"))  // flag goal
                {
                    if (waypoints[g_menu_waypoint].flags & W_FL_TFC_BASE)
                        waypoints[g_menu_waypoint].flags &= ~W_FL_TFC_BASE;  // off
                    else
                        waypoints[g_menu_waypoint].flags |= W_FL_TFC_BASE;  // on
                }
            }

            g_menu_state = MENU_NONE;

            return;
        }
        else if (FStrEq(pcmd, "search"))
        {
            edict_t *pent = NULL;
            float radius = 50;
            char str[80];

            ClientPrint(pEntity, HUD_PRINTCONSOLE, "searching...\n");

            while ((pent = UTIL_FindEntityInSphere( pent, pEntity->v.origin, radius )) != NULL)
            {
                sprintf(str, "Found %s at %5.2f %5.2f %5.2f\n",
                              STRING(pent->v.classname),
                              pent->v.origin.x, pent->v.origin.y,
                              pent->v.origin.z);
                ClientPrint(pEntity, HUD_PRINTCONSOLE, str);

                FILE *fp;
				fopen_s(&fp,"bot.txt", "a");
                fprintf(fp, "ClientCommmand: search %s", str);
                fclose(fp);
            }

            return;
        }
    }

    (*gFunctionTable.pfnClientCommand)( pEntity );
}

void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
    (*gFunctionTable.pfnClientUserInfoChanged)( pEntity, infobuffer );
}

void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
    (*gFunctionTable.pfnServerActivate)( pEdictList, edictCount, clientMax );
}

void ServerDeactivate( void )
{
    (*gFunctionTable.pfnServerDeactivate)();
}

void PlayerPreThink( edict_t *pEntity )
{
    (*gFunctionTable.pfnPlayerPreThink)( pEntity );
}

void PlayerPostThink( edict_t *pEntity )
{
    (*gFunctionTable.pfnPlayerPostThink)( pEntity );
}

void StartFrame( void )
{
    if( gpGlobals->deathmatch )
    {
        edict_t *pPlayer;
        static float check_server_cmd = 0.0;
        static int i, index, player_index, bot_index;
        static float previous_time = -1.0;
        int count;

		if( !g_bInGame )
		{
			if( g_iMod == NS_DLL )
			{
				// if the Countdown message has been sent, make a note of the time
				if( g_iCountDown && !g_fCountDownTime )
				{
					g_fCountDownTime = gpGlobals->time;
				}

				// if the Countdown time (plus a bit) has passed, we're in-game
				if( g_iCountDown && g_fCountDownTime + (float)g_iCountDown + 1.0 < gpGlobals->time )
				{
					g_bInGame = true;
				}
			}
		}

        // if a new map has started then (MUST BE FIRST IN StartFrame)...
        if( ( gpGlobals->time + 0.1 ) < previous_time )
        {
            check_server_cmd = 0.0;  // reset at start of map

            msecnum = 0;
            msecdel = 0;
            msecval = 0;

            count = 0;

            // mark the bots as needing to be respawned...
            for( index = 0; index < 32; index++ )
            {
                if (count >= prev_num_bots)
                {
                    bots[index].is_used = FALSE;
                    bots[index].respawn_state = 0;
                    bots[index].kick_time = 0.0;
                }

                if (bots[index].is_used)  // is this slot used?
                {
                    bots[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
                    count++;
                }

                // check for any bots that were very recently kicked...
                if ((bots[index].kick_time + 5.0) > previous_time)
                {
                    bots[index].respawn_state = RESPAWN_NEED_TO_RESPAWN;
                    count++;
                }
                else
                    bots[index].kick_time = 0.0;  // reset to prevent false spawns later
            }

            // set the respawn time
            if (IS_DEDICATED_SERVER())
                respawn_time = gpGlobals->time + 5.0;
            else
                respawn_time = gpGlobals->time + 20.0;


            bot_check_time = gpGlobals->time + 30.0;
        }

        // adjust the millisecond delay based on the frame rate interval...
        if (msecdel <= gpGlobals->time)
        {
            msecdel = gpGlobals->time + 0.5;
            if (msecnum > 0)
                msecval = 450.0/msecnum;
            msecnum = 0;
        }
        else
            msecnum++;

        if (msecval < 1)     // don't allow msec to be less than 1...
            msecval = 1;

        if (msecval > 100)  // ...or greater than 100
            msecval = 100;

        count = 0;

        for( int i = 0; i < gpGlobals->maxClients; i++ )
        {
            if( bots[i].is_used && ( bots[i].respawn_state == RESPAWN_IDLE ) )  // is this slot used AND not respawning
            {
                BotThink( &bots[i] );

                count++;
            }
        }

        if (count > num_bots)
		{
            num_bots = count;
		}

        for( player_index = 1; player_index <= gpGlobals->maxClients; player_index++ )
        {
            pPlayer = INDEXENT(player_index);

            if( pPlayer && !pPlayer->free )
            {
                if( g_waypoint_on && FBitSet(pPlayer->v.flags, FL_CLIENT) )
                {
                        WaypointThink( pPlayer );
                }
            }
        }

        // are we currently respawning bots and is it time to spawn one yet?
        if ((respawn_time > 1.0) && (respawn_time <= gpGlobals->time))
        {
            int index = 0;

            // find bot needing to be respawned...
            while ((index < 32) && (bots[index].respawn_state != RESPAWN_NEED_TO_RESPAWN))
			{
                index++;
			}

            if( index < 32 )
            {
                bots[index].respawn_state = RESPAWN_IS_RESPAWNING;
                bots[index].is_used = FALSE;        // free up this slot

                // respawn 1 bot then wait a while (otherwise engine crashes)
				BotCreate();

                respawn_time = gpGlobals->time + 2.0;  // set next respawn time

                bot_check_time = gpGlobals->time + 5.0;
            }
            else
            {
                respawn_time = 0.0;
            }
        }

        if (g_GameRules)
        {
            if( !IS_DEDICATED_SERVER() && !spawn_time_reset )
            {
                if( pListenServer )
                {
                    if( IsAlive( pListenServer ) )
                    {
                        spawn_time_reset = TRUE;

                        if (respawn_time >= 1.0)
						{
                            respawn_time = min(respawn_time, gpGlobals->time + (float)1.0);
						}
                    }
                }
            }

        }        

        // check if time to see if a bot needs to be created...
        if (bot_check_time < gpGlobals->time)
        {
            int count = 0;

            bot_check_time = gpGlobals->time + 5.0;

            for (i = 0; i < 32; i++)
            {
                if (clients[i] != NULL)
                    count++;
            }
        }

        previous_time = gpGlobals->time;
    }

    (*gFunctionTable.pfnStartFrame)();
}

void ParmsNewLevel( void )
{
	g_iCountDown = 0;

    (*gFunctionTable.pfnParmsNewLevel)();
}

void ParmsChangeLevel( void )
{
	g_iCountDown = 0;

    (*gFunctionTable.pfnParmsChangeLevel)();
}

const char *GetGameDescription( void )
{
    return (*gFunctionTable.pfnGetGameDescription)();
}

void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
	(*gFunctionTable.pfnPlayerCustomization)( pEntity, pCust );
}

void SpectatorConnect( edict_t *pEntity )
{
    (*gFunctionTable.pfnSpectatorConnect)( pEntity );
}

void SpectatorDisconnect( edict_t *pEntity )
{
    (*gFunctionTable.pfnSpectatorDisconnect)( pEntity );
}

void SpectatorThink( edict_t *pEntity )
{
    (*gFunctionTable.pfnSpectatorThink)( pEntity );
}

void Sys_Error( const char *error_string )
{
    (*gFunctionTable.pfnSys_Error)( error_string );
}

void PM_Move( struct playermove_s *ppmove, int server )
{
    (*gFunctionTable.pfnPM_Move)( ppmove, server );
}

void PM_Init( struct playermove_s *ppmove )
{
    (*gFunctionTable.pfnPM_Init)( ppmove );
}

char PM_FindTextureType( char *name )
{
    return (*gFunctionTable.pfnPM_FindTextureType)( name );
}

void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas )
{
    (*gFunctionTable.pfnSetupVisibility)( pViewEntity, pClient, pvs, pas );
}

void UpdateClientData( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd )
{
    (*gFunctionTable.pfnUpdateClientData)( ent, sendweapons, cd );
}

int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet )
{
    return (*gFunctionTable.pfnAddToFullPack)( state, e, ent, host, hostflags, player, pSet );
}

void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs )
{
    (*gFunctionTable.pfnCreateBaseline)( player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs );
}

void RegisterEncoders( void )
{
    (*gFunctionTable.pfnRegisterEncoders)();
}

int GetWeaponData( struct edict_s *player, struct weapon_data_s *info )
{
    return (*gFunctionTable.pfnGetWeaponData)( player, info );
}

void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed )
{
    (*gFunctionTable.pfnCmdStart)( player, cmd, random_seed );
}

void CmdEnd( const edict_t *player )
{
    (*gFunctionTable.pfnCmdEnd)( player );
}

int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
    return (*gFunctionTable.pfnConnectionlessPacket)( net_from, args, response_buffer, response_buffer_size );
}

int GetHullBounds( int hullnumber, float *mins, float *maxs )
{
    return (*gFunctionTable.pfnGetHullBounds)( hullnumber, mins, maxs );
}

void CreateInstancedBaselines( void )
{
    (*gFunctionTable.pfnCreateInstancedBaselines)();
}

int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message )
{
	return (*gFunctionTable.pfnInconsistentFile)( player, filename, disconnect_message );
}

int AllowLagCompensation( void )
{
    return (*gFunctionTable.pfnAllowLagCompensation)();
}

//
// New DLL Functions
//
void OnFreeEntPrivateData( edict_t *pEnt )
{
	(*gNewFunctionTable.pfnOnFreeEntPrivateData)( pEnt );
}

void GameShutdown( void )
{
	(*gNewFunctionTable.pfnGameShutdown)();
}

int ShouldCollide( edict_t *pentTouched, edict_t *pentOther )
{
	return (*gNewFunctionTable.pfnShouldCollide)( pentTouched, pentOther );
}

void CvarValue( const edict_t *pEnt, const char *value )
{
	(*gNewFunctionTable.pfnCvarValue)( pEnt, value );
}

void CvarValue2( const edict_t *pEnt, int requestID, const char *cvarName, const char *value )
{
	(*gNewFunctionTable.pfnCvarValue2)( pEnt, requestID, cvarName, value );
}

// TODO - investigate this whole fake client command stuff and see if it can be scrapped

void FakeClientCommand(edict_t *pBot, char *arg1, char *arg2, char *arg3)
{
    int length;

    memset(g_argv, 0, sizeof(g_argv));

    isFakeClientCommand = 1;

    if ((arg1 == NULL) || (*arg1 == 0))
        return;

    if ((arg2 == NULL) || (*arg2 == 0))
    {
        length = sprintf(&g_argv[0], "%s", arg1);
        fake_arg_count = 1;
    }
    else if ((arg3 == NULL) || (*arg3 == 0))
    {
        length = sprintf(&g_argv[0], "%s %s", arg1, arg2);
        fake_arg_count = 2;
    }
    else
    {
        length = sprintf(&g_argv[0], "%s %s %s", arg1, arg2, arg3);
        fake_arg_count = 3;
    }

    g_argv[length] = 0;  // null terminate just in case

    strcpy(&g_argv[64], arg1);

    if (arg2)
        strcpy(&g_argv[128], arg2);

    if (arg3)
        strcpy(&g_argv[192], arg3);

    // allow the MOD DLL to execute the ClientCommand...
    ClientCommand( pBot );

    isFakeClientCommand = 0;
}


const char *Cmd_Args( void )
{
    if (isFakeClientCommand)
    {
        return &g_argv[0];
    }
    else
    {
        return (*g_engfuncs.pfnCmd_Args)();
    }
}


const char *Cmd_Argv( int argc )
{
    if (isFakeClientCommand)
    {
        if (argc == 0)
        {
            return &g_argv[64];
        }
        else if (argc == 1)
        {
            return &g_argv[128];
        }
        else if (argc == 2)
        {
            return &g_argv[192];
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return (*g_engfuncs.pfnCmd_Argv)(argc);
    }
}


int Cmd_Argc( void )
{
    if (isFakeClientCommand)
    {
        return fake_arg_count;
    }
    else
    {
        return (*g_engfuncs.pfnCmd_Argc)();
    }
}