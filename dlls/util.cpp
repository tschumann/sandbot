/***
*
*  Copyright (c) 1999, Valve LLC. All rights reserved.
*
*  This product contains software technology licensed from Id 
*  Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*  All Rights Reserved.
*
*    Use, distribution, and modification of this source code and/or resulting
*    object code is restricted to non-commercial enhancements to products from
*    Valve LLC.  All other use, distribution, or modification is prohibited
*    without written permission from Valve LLC.
*
****/

//
// gbot - The GoldSource bot
//
// <no site>
//
// util.cpp
//

#include "extdll.h"
#include "util.h"
#include "engine.h"
#include "dll.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"


extern int g_iMod;
extern bot_t bots[32];
extern char team_names[MAX_TEAMS][MAX_TEAMNAME_LENGTH];
extern int num_teams;

extern bool g_bInGame;

static int gmsgSayText = 0;
static int gmsgShowMenu = 0;
static int gmsgTextMsg = 0;

Vector UTIL_VecToAngles( const Vector &vec )
{
    float rgflVecOut[3];
    VEC_TO_ANGLES(vec, rgflVecOut);
    return Vector(rgflVecOut);
}


// Overloaded to add IGNORE_GLASS
void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr )
{
    TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0), pentIgnore, ptr );
}


void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr )
{
    TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), pentIgnore, ptr );
}


void UTIL_MakeVectors( const Vector &vecAngles )
{
    MAKE_VECTORS( vecAngles );
}


edict_t *UTIL_FindEntityInSphere( edict_t *pentStart, const Vector &vecCenter, float flRadius )
{
    edict_t  *pentEntity;

    pentEntity = FIND_ENTITY_IN_SPHERE( pentStart, vecCenter, flRadius);

    if (!FNullEnt(pentEntity))
        return pentEntity;

    return NULL;
}


edict_t *UTIL_FindEntityByString( edict_t *pentStart, const char *szKeyword, const char *szValue )
{
    edict_t *pentEntity;

    pentEntity = FIND_ENTITY_BY_STRING( pentStart, szKeyword, szValue );

    if (!FNullEnt(pentEntity))
        return pentEntity;
    return NULL;
}

edict_t *UTIL_FindEntityByClassname( edict_t *pentStart, const char *szName )
{
    return UTIL_FindEntityByString( pentStart, "classname", szName );
}

edict_t *UTIL_FindEntityByTargetname( edict_t *pentStart, const char *szName )
{
    return UTIL_FindEntityByString( pentStart, "targetname", szName );
}


int UTIL_PointContents( const Vector &vec )
{
    return POINT_CONTENTS(vec);
}


void UTIL_SetSize( entvars_t *pev, const Vector &vecMin, const Vector &vecMax )
{
    SET_SIZE(ENT(pev), vecMin, vecMax);
}


void UTIL_SetOrigin( entvars_t *pev, const Vector &vecOrigin )
{
    SET_ORIGIN(ENT(pev), vecOrigin );
}


void ClientPrint( edict_t *pEntity, int msg_dest, const char *msg_name)
{
    if (gmsgTextMsg == 0)
        gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );

    pfnMessageBegin( MSG_ONE, gmsgTextMsg, NULL, pEntity );
		pfnWriteByte( msg_dest );
		pfnWriteString( msg_name );
    pfnMessageEnd();
}

void UTIL_SayText( const char *pText, edict_t *pEdict )
{
    if (gmsgSayText == 0)
        gmsgSayText = REG_USER_MSG( "SayText", -1 );

    pfnMessageBegin( MSG_ONE, gmsgSayText, NULL, pEdict );
        pfnWriteByte( ENTINDEX(pEdict) );
        pfnWriteString( pText );
    pfnMessageEnd();
}


void UTIL_HostSay( edict_t *pEntity, int teamonly, char *message )
{
    int    j;
    char  text[128];
    char *pc;
    int    sender_team, player_team;
    edict_t *client;

    // make sure the text has content
    for ( pc = message; pc != NULL && *pc != 0; pc++ )
    {
        if ( isprint( *pc ) && !isspace( *pc ) )
        {
            pc = NULL;    // we've found an alphanumeric character,  so text is valid
            break;
        }
    }

    if ( pc != NULL )
        return;  // no character found, so say nothing

    // turn on color set 2  (color on,  no sound)
    if ( teamonly )
        sprintf( text, "%c(TEAM) %s: ", 2, STRING( pEntity->v.netname ) );
    else
        sprintf( text, "%c%s: ", 2, STRING( pEntity->v.netname ) );

    j = sizeof(text) - 2 - strlen(text);  // -2 for /n and null terminator
    if ( (int)strlen(message) > j )
        message[j] = 0;

    strcat( text, message );
    strcat( text, "\n" );

    // loop through all players
    // Start with the first player.
    // This may return the world in single player if the client types something between levels or during spawn
    // so check it, or it will infinite loop

    if (gmsgSayText == 0)
        gmsgSayText = REG_USER_MSG( "SayText", -1 );

    sender_team = UTIL_GetTeam(pEntity);

    client = NULL;
    while ( ((client = UTIL_FindEntityByClassname( client, "player" )) != NULL) &&
              (!FNullEnt(client)) ) 
    {
        if ( client == pEntity )  // skip sender of message
            continue;

        player_team = UTIL_GetTeam(client);

        if ( teamonly && (sender_team != player_team) )
            continue;

        pfnMessageBegin( MSG_ONE, gmsgSayText, NULL, client );
            pfnWriteByte( ENTINDEX(pEntity) );
            pfnWriteString( text );
        pfnMessageEnd();
    }

    // print to the sending client
    pfnMessageBegin( MSG_ONE, gmsgSayText, NULL, pEntity );
        pfnWriteByte( ENTINDEX(pEntity) );
        pfnWriteString( text );
    pfnMessageEnd();
    
    // echo to server console
    g_engfuncs.pfnServerPrint( text );
}


#ifdef _DEBUG
edict_t *DBG_EntOfVars( const entvars_t *pev )
{
    if (pev->pContainingEntity != NULL)
        return pev->pContainingEntity;
    ALERT(at_console, "entvars_t pContainingEntity is NULL, calling into engine");
    edict_t* pent = (*g_engfuncs.pfnFindEntityByVars)((entvars_t*)pev);
    if (pent == NULL)
        ALERT(at_console, "DAMN!  Even the engine couldn't FindEntityByVars!");
    ((entvars_t *)pev)->pContainingEntity = pent;
    return pent;
}
#endif // _DEBUG


// return team number 0 through 3 based what MOD uses for team numbers
int UTIL_GetTeam( edict_t *pEntity )
{
	if( g_iMod == TFC_DLL )
	{
		// TODO - I think this is right
		return pEntity->v.team;
	}
	else if( g_iMod == NS_DLL )
	{
		return pEntity->v.team;		
	}
    else  // must be HL or OpFor deathmatch...
    {
        char *infobuffer;
        char model_name[32];

        if( team_names[0][0] == 0)
        {
            char *pName;
            char teamlist[MAX_TEAMS*MAX_TEAMNAME_LENGTH];
            int i;

            num_teams = 0;
            strcpy( teamlist, CVAR_GET_STRING( "mp_teamlist" ) );
            pName = teamlist;
            pName = strtok(pName, ";");

            while (pName != NULL && *pName)
            {
                // check that team isn't defined twice
                for (i=0; i < num_teams; i++)
                    if (strcmp(pName, team_names[i]) == 0)
                        break;
                if (i == num_teams)
                {
                    strcpy(team_names[num_teams], pName);
                    num_teams++;
                }
                pName = strtok(NULL, ";");
            }
        }

        infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );
        strcpy( model_name, g_engfuncs.pfnInfoKeyValue( infobuffer, "model" ) );

        for( int i = 0; i < num_teams; i++ )
        {
            if( !strcmp( model_name, team_names[i] ) )
			{
                return i;
			}
        }

        return 0;
    }
}


// return class number
int UTIL_GetClass( edict_t *pEntity )
{
	if( g_iMod == TFC_DLL )
	{
		// TODO - I think this is right
		return pEntity->v.playerclass;
	}
	else if( g_iMod == NS_DLL )
	{
		return pEntity->v.iuser3;
	}

    return 0;
}


int UTIL_GetBotIndex( edict_t *pEdict )
{
    for( int i = 0; i < 32; i++)
    {
        if( bots[i].pEdict == pEdict )
        {
            return i;
        }
    }

    return -1;  // return -1 if edict is not a bot
}


bot_t *UTIL_GetBotPointer( edict_t *pEdict )
{
    int i;

    for( i = 0; i < 32; i++ )
    {
        if( bots[i].pEdict == pEdict )
        {
            break;
        }
    }

    if( i < 32 )
	{
        return &bots[i];
	}

    return NULL;  // return NULL if edict is not a bot
}


bool IsAlive( edict_t *pEdict )
{
    return ( (pEdict->v.deadflag == DEAD_NO) && (pEdict->v.health > 0) && !(pEdict->v.flags & FL_NOTARGET) );
}

bool FInViewCone(Vector *pOrigin, edict_t *pEdict)
{
    UTIL_MakeVectors( pEdict->v.angles );

	Vector2D vec2LOS = ( *pOrigin - pEdict->v.origin ).Make2D();
    vec2LOS = vec2LOS.Normalize();

	float flDot = DotProduct (vec2LOS , gpGlobals->v_forward.Make2D() );

    if( flDot > 0.50 )  // 60 degree field of view 
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


bool FVisible( const Vector &vecOrigin, edict_t *pEdict )
{
    TraceResult tr;
	// look through caller's eyes
    Vector vecLookerOrigin = pEdict->v.origin + pEdict->v.view_ofs;

    int bInWater = ( UTIL_PointContents( vecOrigin ) == CONTENTS_WATER );
    int bLookerInWater = ( UTIL_PointContents( vecLookerOrigin ) == CONTENTS_WATER );

    // don't look through water
    if( bInWater != bLookerInWater )
	{
        return false;
	}

    UTIL_TraceLine( vecLookerOrigin, vecOrigin, ignore_monsters, ignore_glass, pEdict, &tr );

    if( tr.flFraction != 1.0 )
    {
        return false;  // Line of sight is not established
    }
    else
    {
        return true;  // line of sight is valid.
    }
}


Vector GetGunPosition(edict_t *pEdict)
{
    return pEdict->v.origin + pEdict->v.view_ofs;
}


void UTIL_SelectItem( edict_t *pEdict, char *item_name )
{
    FakeClientCommand( pEdict, item_name, NULL, NULL );
}


Vector VecBModelOrigin( edict_t *pEdict )
{
    return pEdict->v.absmin + ( pEdict->v.size * 0.5 );
}


bool UpdateSounds( edict_t *pEdict, edict_t *pPlayer )
{
    float distance;
    static bool check_footstep_sounds = TRUE;
    static float footstep_sounds_on;
    float sensitivity = 1.0;
    float volume;

    // update sounds made by this player, alert bots if they are nearby...

    if (check_footstep_sounds)
    {
        check_footstep_sounds = FALSE;
        footstep_sounds_on = CVAR_GET_FLOAT( "mp_footsteps" );
    }

    if (footstep_sounds_on > 0.0)
    {
        // check if this player is moving fast enough to make sounds...
        if (pPlayer->v.velocity.Length2D() > 220.0)
        {
            volume = 500.0;  // volume of sound being made (just pick something)

            Vector v_sound = pPlayer->v.origin - pEdict->v.origin;

            distance = v_sound.Length();

            // is the bot close enough to hear this sound?
            if (distance < (volume * sensitivity))
            {
                Vector bot_angles = UTIL_VecToAngles( v_sound );

                pEdict->v.ideal_yaw = bot_angles.y;

                BotFixIdealYaw(pEdict);

                return TRUE;
            }
        }
    }

    return FALSE;
}


void UTIL_ShowMenu( edict_t *pEdict, int slots, int displaytime, bool needmore, char *pText )
{
    if( gmsgShowMenu == 0 )
	{
        gmsgShowMenu = REG_USER_MSG( "ShowMenu", -1 );
	}

    pfnMessageBegin( MSG_ONE, gmsgShowMenu, NULL, pEdict );
		pfnWriteShort( slots );
		pfnWriteChar( displaytime );
		pfnWriteByte( needmore );
		pfnWriteString( pText );
    pfnMessageEnd();
}

void UTIL_BuildFileName( char *filename, char *arg1, char *arg2 )
{
    if( g_iMod == VALVE_DLL )
	{
        strcpy( filename, "valve/" );
	}
	else if( g_iMod == GEARBOX_DLL )
	{
        strcpy( filename, "gearbox/" );
	}
	else if( g_iMod == CSTRIKE_DLL )
	{
        strcpy( filename, "cstrike/" );
	}
	else if( g_iMod == DOD_DLL )
	{
        strcpy( filename, "dod/" );
	}
	else if( g_iMod == TFC_DLL )
	{
        strcpy( filename, "tfc/" );
	}
    else if( g_iMod == HUNGER_DLL )
	{
        strcpy( filename, "hunger/" );
	}
	else if( g_iMod == NS_DLL )
	{
        strcpy( filename, "ns/" );
	}
	else if( g_iMod == GUNMAN_DLL )
	{
        strcpy( filename, "rewolf/" );
	}
	else if( g_iMod == SHIP_DLL )
	{
        strcpy( filename, "ship/" );
	}
    else
    {
        filename[0] = 0;
        return;
    }

    if( arg1 && *arg1 && arg2 && *arg2 )
    {
        strcat( filename, arg1 );
        strcat( filename, "/" );
        strcat( filename, arg2 );
    }
    else if( arg1 && *arg1 )
    {
        strcat( filename, arg1 );
    }
}

//=========================================================
// UTIL_LogPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogPrintf( char *fmt, ... )
{
    va_list argptr;
    static char string[1024];
    
    va_start( argptr, fmt );
    vsprintf( string, fmt, argptr );
    va_end( argptr );

    // Print to server console
    ALERT( at_logged, "%s", string );
}

Vector UTIL_GetOrigin( edict_t *pEdict )
{
	if( !strncmp( STRING(pEdict->v.classname), "func_", 5 ) )
	{
		return VecBModelOrigin(pEdict);
	}

	return pEdict->v.origin; 
}

// Natural Selection

bool UTIL_IsBuilt( edict_t *pent )
{
	if( pent->v.iuser4 & MASK_BUILDABLE )
    {
        return false;
    }

    return true;
}

bool UTIL_IsCombat()
{
	const char *szMap = STRING(gpGlobals->mapname);

	if( szMap[0] == 'c' /*|| szMap[0] == 'C'*/ )
	{
		return true;
	}
	else
	{
		return false;
	}
}

float UTIL_GetResources( edict_t *player )
{
	return player->v.vuser4.z / 100.0;
}

float UTIL_GetExperience( edict_t *player )
{
    return player->v.vuser4.z / 100.0;
}

int UTIL_GetPoints( edict_t *player )
{
	int iSpent = *(int*)( (char*)(player) + 1581 );

	return iSpent;
}

BOOL UTIL_IsEvolved( const bot_t *pBot )
{
	return ( ( pBot->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER2 ) || ( pBot->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER3 ) || ( pBot->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER4 ) || ( pBot->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER5 ) ) && !pBot->bEvolved && pBot->bEvolving; 
}

bool UTIL_CanEvolve( const bot_t *pBot )
{
	if( pBot->pBotEnemy )
	{
		return false;
	}

	if( pBot->iClass == AVH_USER3_ALIEN_PLAYER1 )
	{
		return false;
	}

	if( pBot->bEvolved && pBot->bEvolving )
	{
		return false;
	}

	return true;
}