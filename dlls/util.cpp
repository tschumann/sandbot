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
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// util.cpp
//

#include "extdll.h"
#include "util.h"
#include "engine.h"
#include "dll.h"
#include "studio.h"

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

bool IsAlive(edict_t *pEdict)
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
    Vector vecLookerOrigin;

    // look through caller's eyes
    vecLookerOrigin = pEdict->v.origin + pEdict->v.view_ofs;

    int bInWater = (UTIL_PointContents (vecOrigin) == CONTENTS_WATER);
    int bLookerInWater = (UTIL_PointContents (vecLookerOrigin) == CONTENTS_WATER);

    // don't look through water
    if (bInWater != bLookerInWater)
	{
        return FALSE;
	}

    UTIL_TraceLine(vecLookerOrigin, vecOrigin, ignore_monsters, ignore_glass, pEdict, &tr);

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
    return (pEdict->v.origin + pEdict->v.view_ofs);
}

Vector VecBModelOrigin(edict_t *pEdict)
{
    return pEdict->v.absmin + (pEdict->v.size * 0.5);
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