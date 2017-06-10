/***
*
*  Copyright (c) 1999, Valve LLC. All rights reserved.
*
*  This product contains software technology licensed from Id 
*  Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*  All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
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
#include "dll.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"

#include <cctype>


extern int mod_id;
extern bot_t **pBots;
extern char team_names[MAX_TEAMS][MAX_TEAMNAME_LENGTH];
extern int num_teams;

int gmsgTextMsg = 0;
int gmsgSayText = 0;
int gmsgShowMenu = 0;


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
   SET_SIZE( ENT(pev), vecMin, vecMax );
}


void UTIL_SetOrigin( entvars_t *pev, const Vector &vecOrigin )
{
   SET_ORIGIN(ENT(pev), vecOrigin );
}


void ClientPrint( edict_t *pEntity, int msg_dest, const char *msg_name)
{
   if (gmsgTextMsg == 0)
      gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );

   MESSAGE_BEGIN( MSG_ONE, gmsgTextMsg, NULL, pEntity );

   WRITE_BYTE( msg_dest );
   WRITE_STRING( msg_name );

   MESSAGE_END();
}

void UTIL_SayText( const char *pText, edict_t *pEdict )
{
   if (gmsgSayText == 0)
      gmsgSayText = REG_USER_MSG( "SayText", -1 );

   MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, pEdict );

   WRITE_BYTE( ENTINDEX(pEdict) );
   WRITE_STRING( pText );

   MESSAGE_END();
}


void UTIL_HostSay( edict_t *pEntity, int teamonly, char *message )
{
   int   j;
   char  text[128];
   char *pc;
   int   sender_team, player_team;
   edict_t *client;

   // make sure the text has content
   for ( pc = message; pc != NULL && *pc != 0; pc++ )
   {
      if ( isprint( *pc ) && !isspace( *pc ) )
      {
         pc = NULL;   // we've found an alphanumeric character,  so text is valid
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

	  MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, client );

	  WRITE_BYTE( ENTINDEX(pEntity) );
	  WRITE_STRING( text );

	  MESSAGE_END();
   }

   // print to the sending client
   MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, pEntity );

   WRITE_BYTE( ENTINDEX(pEntity) );
   WRITE_STRING( text );

   MESSAGE_END();
   
   // echo to server console
   g_engfuncs.pfnServerPrint( text );
}


#ifdef   DEBUG
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
#endif //DEBUG


// return team number 0 through 3 based what MOD uses for team numbers
int UTIL_GetTeam(edict_t *pEntity)
{
   if (mod_id == TFC_DLL)
   {
      return pEntity->v.team - 1;  // TFC teams are 1-4 based
   }
   else if (mod_id == CSTRIKE_DLL)
   {
      char *infobuffer;
      char model_name[32];

      infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );
      strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));

      if ((strcmp(model_name, "terror") == 0) ||  // Phoenix Connektion
          (strcmp(model_name, "arab") == 0) ||    // old L337 Krew
          (strcmp(model_name, "leet") == 0) ||    // L337 Krew
          (strcmp(model_name, "arctic") == 0) ||   // Artic Avenger
          (strcmp(model_name, "guerilla") == 0))  // Gorilla Warfare
      {
         return 0;
      }
      else if ((strcmp(model_name, "urban") == 0) ||  // Seal Team 6
               (strcmp(model_name, "gsg9") == 0) ||   // German GSG-9
               (strcmp(model_name, "sas") == 0) ||    // UK SAS
               (strcmp(model_name, "gign") == 0) ||   // French GIGN
               (strcmp(model_name, "vip") == 0))      // VIP
      {
         return 1;
      }

      return 0;  // return zero if team is unknown
   }
	else if( mod_id == DOD_DLL || mod_id == NS_DLL )
	{
		return pEntity->v.team;
	}
   else if ((mod_id == GEARBOX_DLL) && pGame->IsCTF())
   {
      // OpFor CTF map...

      char *infobuffer;
      char model_name[32];

      infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );
      strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));

      if ((strcmp(model_name, "ctf_barney") == 0) ||
          (strcmp(model_name, "cl_suit") == 0) ||
          (strcmp(model_name, "ctf_gina") == 0) ||
          (strcmp(model_name, "ctf_gordon") == 0) ||
          (strcmp(model_name, "otis") == 0) ||
          (strcmp(model_name, "ctf_scientist") == 0))
      {
         return OpposingForceBot::TEAM_BLACK_MESA;
      }
      else if ((strcmp(model_name, "beret") == 0) ||
               (strcmp(model_name, "drill") == 0) ||
               (strcmp(model_name, "grunt") == 0) ||
               (strcmp(model_name, "recruit") == 0) ||
               (strcmp(model_name, "shephard") == 0) ||
               (strcmp(model_name, "tower") == 0))
      {
         return OpposingForceBot::TEAM_OPPOSING_FORCE;
      }

      return 0;  // return zero if team is unknown
   }
   else  // must be HL or OpFor deathmatch...
   {
      char *infobuffer;
      char model_name[32];

      if (team_names[0][0] == 0)
      {
         char *pName;
         char teamlist[MAX_TEAMS*MAX_TEAMNAME_LENGTH];
         int i;

         num_teams = 0;
         strcpy(teamlist, CVAR_GET_STRING("mp_teamlist"));
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
      strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));

      for (int index=0; index < num_teams; index++)
      {
         if (strcmp(model_name, team_names[index]) == 0)
            return index;
      }

      return 0;
   }
}


// return class number 0 through N
int UTIL_GetClass(edict_t *pEntity)
{
	char *infobuffer;
	char model_name[32];

	infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity );
	strcpy(model_name, (g_engfuncs.pfnInfoKeyValue(infobuffer, "model")));

	if( mod_id == NS_DLL )
	{
		// see AvHUser3
		return pEntity->v.iuser3;
	}

	return 0;
}


int UTIL_GetBotIndex(edict_t *pEdict)
{
   for (int index=0; index < MAX_PLAYERS; index++)
   {
      if (pBots && pBots[index] && pBots[index]->pEdict == pEdict)
      {
         return index;
      }
   }

   return -1;  // return -1 if edict is not a bot
}


bot_t *UTIL_GetBotPointer(edict_t *pEdict)
{
   int index;

   for (index=0; index < MAX_PLAYERS; index++)
   {
      if (pBots && pBots[index] && pBots[index]->pEdict == pEdict)
      {
         break;
      }
   }

   if (index < MAX_PLAYERS)
      return (pBots[index]);

   return NULL;  // return NULL if edict is not a bot
}


bool IsAlive(edict_t *pEdict)
{
	return ((pEdict->v.deadflag == DEAD_NO) && (pEdict->v.health > 0) && !(pEdict->v.flags & FL_NOTARGET));
}


bool FInViewCone(Vector *pOrigin, edict_t *pEdict)
{
   Vector2D vec2LOS;
   float    flDot;

   UTIL_MakeVectors ( pEdict->v.angles );

   vec2LOS = ( *pOrigin - pEdict->v.origin ).Make2D();
   vec2LOS = vec2LOS.Normalize();

   flDot = DotProduct (vec2LOS , gpGlobals->v_forward.Make2D() );

   if ( flDot > 0.50 )  // 60 degree field of view 
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
   Vector      vecLookerOrigin;

   // look through caller's eyes
   vecLookerOrigin = pEdict->v.origin + pEdict->v.view_ofs;

   int bInWater = (UTIL_PointContents (vecOrigin) == CONTENTS_WATER);
   int bLookerInWater = (UTIL_PointContents (vecLookerOrigin) == CONTENTS_WATER);

   // don't look through water
   if (bInWater != bLookerInWater)
      return FALSE;

   UTIL_TraceLine(vecLookerOrigin, vecOrigin, ignore_monsters, ignore_glass, pEdict, &tr);

   if (tr.flFraction != 1.0)
   {
      return FALSE;  // Line of sight is not established
   }
   else
   {
      return TRUE;  // line of sight is valid.
   }
}


Vector GetGunPosition(edict_t *pEdict)
{
	return (pEdict->v.origin + pEdict->v.view_ofs);
}


void UTIL_SelectItem(edict_t *pEdict, char *item_name)
{
	FakeClientCommand(pEdict, item_name);
}


Vector VecBModelOrigin(edict_t *pEdict)
{
	return pEdict->v.absmin + (pEdict->v.size * 0.5);
}


bool UpdateSounds(edict_t *pEdict, edict_t *pPlayer)
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
      footstep_sounds_on = CVAR_GET_FLOAT("mp_footsteps");
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
   if (gmsgShowMenu == 0)
      gmsgShowMenu = REG_USER_MSG( "ShowMenu", -1 );

   MESSAGE_BEGIN( MSG_ONE, gmsgShowMenu, NULL, pEdict );

   WRITE_SHORT( slots );
   WRITE_CHAR( displaytime );
   WRITE_BYTE( needmore );
   WRITE_STRING( pText );

   MESSAGE_END();
}

void UTIL_BuildFileName(char *filename, char *arg1, char *arg2)
{
	if (mod_id == VALVE_DLL)
	{
		strcpy(filename, "valve/");
	}
	else if (mod_id == BSHIFT_DLL)
	{
		strcpy(filename, "bshift/");
	}
	else if (mod_id == GEARBOX_DLL)
	{
		strcpy(filename, "gearbox/");
	}
	else if (mod_id == DECAY_DLL)
	{
		strcpy(filename, "decay/");
	}
	else if (mod_id == CSTRIKE_DLL)
	{
		strcpy(filename, "cstrike/");
	}
	else if (mod_id == CZERO_DLL)
	{
		strcpy(filename, "czero/");
	}
	else if (mod_id == CZEROR_DLL)
	{
		strcpy(filename, "czeror/");
	}
	else if (mod_id == DOD_DLL)
	{
		strcpy(filename, "dod/");
	}
	else if (mod_id == TFC_DLL)
	{
		strcpy(filename, "tfc/");
	}
	else if (mod_id == REWOLF_DLL)
	{
		strcpy(filename, "rewolf/");
	}
	else if (mod_id == NS_DLL)
	{
		strcpy(filename, "ns/");
	}
	else if (mod_id == HUNGER_DLL)
	{
		strcpy(filename, "hunger/");
	}
	else if (mod_id == SHIP_DLL)
	{
		strcpy(filename, "ship/");
	}
	else
	{
		filename[0] = 0;
		return;
	}

	if( arg1 && (*arg1) && (arg2) && (*arg2) )
	{
		strcat(filename, arg1);
		strcat(filename, "/");
		strcat(filename, arg2);
	}
	else if( arg1 && (*arg1) )
	{
		strcat(filename, arg1);
	}
}

//=========================================================
// UTIL_LogPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogPrintf( char *fmt, ... )
{
	va_list        argptr;
	static char    string[1024];

	va_start ( argptr, fmt );
	vsprintf ( string, fmt, argptr );
	va_end   ( argptr );

	// Print to server console
	ALERT( at_logged, "%s", string );
}

//=========================================================
// UTIL_LogDPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogDPrintf( char *fmt, ... )
{
	extern cvar_t *developer;

	if (DEBUG_CODE /*developer->value*/)
    {
		va_list        argptr;
		static char    string[1024];

		va_start ( argptr, fmt );
		vsprintf ( string, fmt, argptr );
		va_end   ( argptr );

		// Print to server console 
		// TODO: revert back to at_logged and get it working
		ALERT( at_console, "%s", string );
	}
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

float UTIL_GetExperience( edict_t *player )
{
    return player->v.vuser4.z / 100.0;
}

int UTIL_GetPoints( bot_t *player )
{
	// TODO: wrong? see http://www.modns.org/index.php?showtopic=2579
	// TODO: does it matter? the bot can you try to upgrade regardless of if it fails
	// TODO: it gets displayed on the HUD (i.e. from the client) so the client must be able to easily access it
	// int iSpent = *(int*)( (char*)(player) + 1581 );

	// return iSpent;
	return (int)(UTIL_GetExperience(player->pEdict) / 100.0) - player->points_spent;
}