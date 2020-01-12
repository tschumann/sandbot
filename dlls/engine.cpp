//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// engine.cpp
//

#include "extdll.h"
#include "meta_api.h"
#include "metamod.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_client.h"
#include "engine.h"
#include "game.h"
#include "dll.h"
#include "linkfunc.h"
#include "util.h"

extern enginefuncs_t g_engfuncs;

int debug_engine = 0;

void (*botMsgFunction)(void *, int) = NULL;
void (*botMsgEndFunction)(void *, int) = NULL;
int botMsgIndex;

// messages created in RegUserMsg which will be "caught"
int message_VGUI = 0;
int message_ShowMenu = 0;
int message_WeaponList = 0;
int message_CurWeapon = 0;
int message_AmmoX = 0;
int message_AmmoPickup = 0;
int message_Damage = 0;
int message_DeathMsg = 0;
int message_TextMsg = 0;
int message_ScreenFade = 0;
int message_HLTV = 0; // for Counter-Strike
int message_Money = 0;  // for Counter-Strike
int gmsgCountdown = 0;	// Natural Selection
int gmsgGameStatus = 0; // Natural Selection
int gmsgQuarry = 0;		// The Ship

static FILE *fp;

const char *AssetRemap( const char *szPath )
{
	if( mod_id == REWOLF_DLL )
	{
		// this seems to be precached but not used
		if( !strcmp(szPath, "sprites/glow_aicore_no.spr") )
		{
			return "sprites/glow_beamgun_no.spr";
		}
	}
	else if( mod_id == SHIP_DLL )
	{
		// it's unclear where these invalid model names come from
		// they cause crashes when developer > 0
		if( !strncmp(szPath, "models/player/models/player/Jane.mdl/models/player/Jane.mdl.mdl", strlen("models/player/models/player/Jane.mdl") ) )
		{
			return "models/player/Jane.mdl";
		}
		else if( !strncmp(szPath, "models/player/models/player/John.mdl/models/player/John.mdl.mdl", strlen("models/player/models/player/John.mdl") ) )
		{
			return "models/player/John.mdl";
		}
		else if( !strncmp(szPath, "models/player/models/player/john2.mdl/models/player/john2.mdl.m", strlen("models/player/models/player/john2.mdl") ) )
		{
			return "models/player/john2.mdl";
		}
		else if( !strncmp(szPath, "models/player/models/player/Holliday.mdl/models/player/Holliday", strlen("models/player/models/player/Holliday.mdl") ) )
		{
			return "models/player/Holliday.mdl";
		}
	}

	return szPath;
}

int pfnPrecacheModel(char* s)
{
	const char *szPath = AssetRemap( s );

	if( g_bIsMMPlugin && strcmp(szPath, s) != 0 )
	{
		int result = PRECACHE_MODEL((char*)szPath);
		return METAMOD_RETURN_VALUE( MRES_SUPERCEDE, result );
	}

	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnPrecacheModel)((char*)szPath);
}

int pfnPrecacheSound(char* s)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnPrecacheSound)(s);
}

void pfnSetModel(edict_t *e, const char *m)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnSetModel)(e, AssetRemap(m));
}

int pfnModelIndex(const char *m)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnModelIndex)(m);
}

int pfnModelFrames(int modelIndex)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnModelFrames)(modelIndex);
}

void pfnSetSize(edict_t *e, const float *rgflMin, const float *rgflMax)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnSetSize)(e, rgflMin, rgflMax);
}

void pfnChangeLevel(char* s1, char* s2)
{
	KickAllBots();
	pGame->Cleanup();

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnChangeLevel)(s1, s2);
}

void pfnGetSpawnParms(edict_t *ent)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnGetSpawnParms)(ent);
}

void pfnSaveSpawnParms(edict_t *ent)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnSaveSpawnParms)(ent);
}

float pfnVecToYaw(const float *rgflVector)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnVecToYaw)(rgflVector);
}

void pfnVecToAngles(const float *rgflVectorIn, float *rgflVectorOut)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnVecToAngles)(rgflVectorIn, rgflVectorOut);
}

void pfnMoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnMoveToOrigin)(ent, pflGoal, dist, iMoveType);
}

void pfnChangeYaw(edict_t* ent)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnChangeYaw)(ent);
}

void pfnChangePitch(edict_t* ent)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnChangePitch)(ent);
}

edict_t* pfnFindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue)
{
	if (mod_id == CSTRIKE_DLL)
	{
		// new round in CS 1.5
		if (strcmp ("info_map_parameters", pszValue) == 0)
		{
			for (int bot_index = 0; bot_index < MAX_PLAYERS; bot_index++)
			{
				bot_t *pBot = pBots[bot_index];

				if (pBot->is_used)
					BotSpawnInit (pBot); // reset bots for new round
			}
		}
	}

	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	edict_t *pFound = (*g_engfuncs.pfnFindEntityByString)(pEdictStartSearchAfter, pszField, pszValue);

	UTIL_LogTPrintf( "pfnFindEntityByString: pEdictStartSearchAfter=%x, pszField=%s, pszValue=%s; result=%x\n", pEdictStartSearchAfter, pszField, pszValue, pFound );

	return pFound;
}

int pfnGetEntityIllum(edict_t* pEnt)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnGetEntityIllum)(pEnt);
}

edict_t* pfnFindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad)
{
	UTIL_LogTPrintf( "pfnFindEntityInSphere: pEdictStartSearchAfter=%x, org=(%f,%f,%f), rad=%f\n", pEdictStartSearchAfter, org[0], org[1], org[2], rad );

	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnFindEntityInSphere)(pEdictStartSearchAfter, org, rad);
}

edict_t* pfnFindClientInPVS(edict_t *pEdict)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnFindClientInPVS)(pEdict);
}

edict_t* pfnEntitiesInPVS(edict_t *pplayer)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnEntitiesInPVS)(pplayer);
}

void pfnMakeVectors(const float *rgflVector)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnMakeVectors)(rgflVector);
}

void pfnAngleVectors(const float *rgflVector, float *forward, float *right, float *up)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnAngleVectors)(rgflVector, forward, right, up);
}

edict_t* pfnCreateEntity(void)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	edict_t *pent = (*g_engfuncs.pfnCreateEntity)();
	UTIL_LogDPrintf("pfnCreateEntity\n");
	return pent;
}

void pfnRemoveEntity(edict_t* e)
{
   UTIL_LogDPrintf("pfnRemoveEntity: classname=%s\n", STRING(e->v.classname));

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnRemoveEntity)(e);
}

edict_t* pfnCreateNamedEntity(int className)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	edict_t *pent = (*g_engfuncs.pfnCreateNamedEntity)(className);
	UTIL_LogDPrintf("pfnCreateNamedEntity: className=%s\n", STRING(className));
	return pent;
}

void pfnMakeStatic(edict_t *ent)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnMakeStatic)(ent);
}

int pfnEntIsOnFloor(edict_t *e)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnEntIsOnFloor)(e);
}

int pfnDropToFloor(edict_t* e)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnDropToFloor)(e);
}

int pfnWalkMove(edict_t *ent, float yaw, float dist, int iMode)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnWalkMove)(ent, yaw, dist, iMode);
}

void pfnSetOrigin(edict_t *e, const float *rgflOrigin)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnSetOrigin)(e, rgflOrigin);
}

void pfnEmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch)
{
	UTIL_LogTPrintf("pfnEmitSound: entity=%x, channel=%d, sample=%s, volume=%f, attenuation=%f, fFlags=%d, pitch=%d\n", entity, channel, sample, volume, attenuation, fFlags, pitch);

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnEmitSound)(entity, channel, sample, volume, attenuation, fFlags, pitch);
}

void pfnEmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch)
{
	UTIL_LogTPrintf("pfnEmitAmbientSound: entity=%x, pos=(%f,%f,%f), samp=%s, vol=%f, attenuation=%f, fFlags=%d, pitch=%d\n", entity, pos[0], pos[1], pos[2], samp, vol, attenuation, fFlags, pitch);

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnEmitAmbientSound)(entity, pos, samp, vol, attenuation, fFlags, pitch);
}

void pfnTraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnTraceLine)(v1, v2, fNoMonsters, pentToSkip, ptr);
}

void pfnTraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnTraceToss)(pent, pentToIgnore, ptr);
}

int pfnTraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnTraceMonsterHull)(pEdict, v1, v2, fNoMonsters, pentToSkip, ptr);
}

void pfnTraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnTraceHull)(v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr);
}

void pfnTraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnTraceModel)(v1, v2, hullNumber, pent, ptr);
}

const char *pfnTraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 )
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, "" );

	return (*g_engfuncs.pfnTraceTexture)(pTextureEntity, v1, v2);
}

void pfnTraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnTraceSphere)(v1, v2, fNoMonsters, radius, pentToSkip, ptr);
}

void pfnGetAimVector(edict_t* ent, float speed, float *rgflReturn)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnGetAimVector)(ent, speed, rgflReturn);
}

void pfnServerCommand(char* str)
{
	// Natural Selection 3.2 forces these every frame
	if( mod_id == NS_DLL && !strncmp(str, "sv_airmove", strlen("sv_airmove")) && !strncmp(str, "sv_airaccelerate", strlen("sv_airaccelerate")) )
	{
		UTIL_LogDPrintf("pfnServerCommand: str=%s\n", str);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );
	
	(*g_engfuncs.pfnServerCommand)(str);
}

void pfnServerExecute(void)
{
	UTIL_LogDPrintf("pfnServerExecute\n");

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnServerExecute)();
}

void pfnClientCommand(edict_t* pEdict, char* szFmt, ...)
{
	va_list argptr;
	static char string[1024];

	va_start( argptr, szFmt );
	_vsnprintf( string, sizeof( string ), szFmt, argptr );
	va_end( argptr );

	UTIL_LogDPrintf( "pfnClientCommand: string=%s\n", string );

	// is the target entity an official bot, a third party bot or a real player ?
	if( pEdict->v.flags & FL_FAKECLIENT )
	{
		FakeClientCommand( pEdict, string );
		if( g_bIsMMPlugin )
			METAMOD_RETURN( MRES_SUPERCEDE );
	}

	(*g_engfuncs.pfnClientCommand)(pEdict, string);

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_SUPERCEDE );
}

void pfnParticleEffect(const float *org, const float *dir, float color, float count)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnParticleEffect)(org, dir, color, count);
}

void pfnLightStyle(int style, char* val)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnLightStyle)(style, val);
}

int pfnDecalIndex(const char *name)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnDecalIndex)(name);
}

int pfnPointContents(const float *rgflVector)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnPointContents)(rgflVector);
}

void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	int index = -1;
	UTIL_LogTPrintf( "pfnMessageBegin: msg_dest=%d msg_type=%d ed=%x\n", msg_dest, msg_type, ed );

  if (ed)
  {
     index = UTIL_GetBotIndex(ed);

     // is this message for a bot?
     if (index != -1)
     {
        botMsgFunction = NULL;     // no msg function until known otherwise
        botMsgEndFunction = NULL;  // no msg end function until known otherwise
        botMsgIndex = index;       // index of bot receiving message

        if (mod_id == VALVE_DLL)
        {
           if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_Valve_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_Valve_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_Valve_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_Valve_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_Valve_Damage;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_Valve_ScreenFade;
        }
		else if (mod_id == GEARBOX_DLL)
        {
           if (msg_type == message_VGUI)
              botMsgFunction = BotClient_Gearbox_VGUI;
           else if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_Gearbox_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_Gearbox_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_Gearbox_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_Gearbox_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_Gearbox_Damage;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_Gearbox_ScreenFade;
        }
        else if (mod_id == CSTRIKE_DLL)
        {
           if (msg_type == message_VGUI)
              botMsgFunction = BotClient_CS_VGUI;
           else if (msg_type == message_ShowMenu)
              botMsgFunction = BotClient_CS_ShowMenu;
           else if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_CS_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_CS_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_CS_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_CS_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_CS_Damage;
           else if (msg_type == message_Money)
              botMsgFunction = BotClient_CS_Money;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_CS_ScreenFade;
        }
		else if (mod_id == DOD_DLL)
        {
           if (msg_type == message_VGUI)
              botMsgFunction = BotClient_DOD_VGUI;
           else if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_DOD_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_Valve_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_Valve_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_Valve_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_Valve_Damage;
        }
		else if (mod_id == TFC_DLL)
        {
           if (msg_type == message_VGUI)
              botMsgFunction = BotClient_TFC_VGUI;
           else if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_TFC_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_TFC_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_TFC_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_TFC_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_TFC_Damage;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_TFC_ScreenFade;
        }
		else if (mod_id == REWOLF_DLL)
        {
           if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_Gunman_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_Gunman_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_Gunman_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_Gunman_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_Gunman_Damage;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_Gunman_ScreenFade;
        }
		else if (mod_id == HUNGER_DLL)
        {
           if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_Hunger_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_Hunger_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_Hunger_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_Hunger_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_Hunger_Damage;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_Hunger_ScreenFade;
        }
		else if (mod_id == SHIP_DLL)
        {
           if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_Ship_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_Ship_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_Ship_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_Ship_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_Ship_Damage;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_Ship_ScreenFade;
        }
		else if (mod_id == NS_DLL)
        {
           if (msg_type == message_WeaponList)
              botMsgFunction = BotClient_NS_WeaponList;
           else if (msg_type == message_CurWeapon)
              botMsgFunction = BotClient_NS_CurrentWeapon;
           else if (msg_type == message_AmmoX)
              botMsgFunction = BotClient_NS_AmmoX;
           else if (msg_type == message_AmmoPickup)
              botMsgFunction = BotClient_NS_AmmoPickup;
           else if (msg_type == message_Damage)
              botMsgFunction = BotClient_NS_Damage;
           else if (msg_type == message_ScreenFade)
              botMsgFunction = BotClient_NS_ScreenFade;
        }
     }
  }
  else if (msg_dest == MSG_ALL)
  {
     botMsgFunction = NULL;  // no msg function until known otherwise
     botMsgIndex = -1;       // index of bot receiving message (none)

     if (mod_id == VALVE_DLL)
     {
        if (msg_type == message_DeathMsg)
           botMsgFunction = BotClient_Valve_DeathMsg;
     }
     else if (mod_id == TFC_DLL)
     {
        if (msg_type == message_DeathMsg)
           botMsgFunction = BotClient_TFC_DeathMsg;
     }
     else if (mod_id == CSTRIKE_DLL)
     {
        if (msg_type == message_DeathMsg)
           botMsgFunction = BotClient_CS_DeathMsg;
     }
     else if (mod_id == GEARBOX_DLL)
     {
        if (msg_type == message_DeathMsg)
           botMsgFunction = BotClient_Gearbox_DeathMsg;
     }
	 else if (mod_id == REWOLF_DLL)
     {
        if (msg_type == message_DeathMsg)
           botMsgFunction = BotClient_Gunman_DeathMsg;
     }
	 else if (mod_id == NS_DLL)
     {
        if (msg_type == message_DeathMsg)
		{
           botMsgFunction = BotClient_NS_DeathMsg;
		}
		else if( msg_type == gmsgCountdown )
		{
			botMsgFunction = BotClient_NS_CountDown;
		}
		else if( msg_type == gmsgGameStatus )
		{
			botMsgFunction = BotClient_NS_GameStatus;
		}
     }
	 else if (mod_id == HUNGER_DLL)
     {
        if (msg_type == message_DeathMsg)
           botMsgFunction = BotClient_Hunger_DeathMsg;
     }
	 else if (mod_id == SHIP_DLL)
     {
        if (msg_type == message_DeathMsg)
		{
           botMsgFunction = BotClient_Valve_DeathMsg;
		}
		else if (msg_type == gmsgQuarry)
		{
           botMsgFunction = BotClient_Ship_Quarry;
		}
     }
  }
  else
  {
	  // TODO: still needed?
     // Steam makes the WeaponList message be sent differently

     botMsgFunction = NULL;  // no msg function until known otherwise
     botMsgIndex = -1;       // index of bot receiving message (none)

     if (mod_id == VALVE_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_Valve_WeaponList;
     }
	 else if (mod_id == DOD_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_TFC_WeaponList;
     }
     else if (mod_id == TFC_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_TFC_WeaponList;
     }
     else if (mod_id == CSTRIKE_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_CS_WeaponList;
        else if (msg_type == message_HLTV)
           botMsgFunction = BotClient_CS_HLTV;
     }
     else if (mod_id == GEARBOX_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_Gearbox_WeaponList;
     }
	 else if (mod_id == REWOLF_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_Gunman_WeaponList;
     }
	 else if (mod_id == NS_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_NS_WeaponList;
     }
	 else if (mod_id == HUNGER_DLL)
     {
        if (msg_type == message_WeaponList)
           botMsgFunction = BotClient_Hunger_WeaponList;
     }
	 else if (mod_id == SHIP_DLL)
     {
        if (msg_type == message_WeaponList)
		{
           botMsgFunction = BotClient_Ship_WeaponList;
		}
		else if (msg_type == gmsgQuarry)
		{
           botMsgFunction = BotClient_Ship_Quarry;
		}
     }
  }

  if( g_bIsMMPlugin )
	  METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ed);
}

void pfnMessageEnd(void)
{
	UTIL_LogTPrintf( "pfnMessageEnd\n" );

	if( botMsgEndFunction )
	{
		(*botMsgEndFunction)(NULL, botMsgIndex);  // NULL indicated msg end
	}

	// clear out the bot message function pointers
	botMsgFunction = NULL;
	botMsgEndFunction = NULL;

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnMessageEnd)();
}

void pfnWriteByte(int iValue)
{
	UTIL_LogTPrintf( "pfnWriteByte: %d\n", iValue );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)&iValue, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteByte)(iValue);
}

void pfnWriteChar(int iValue)
{
	UTIL_LogTPrintf( "pfnWriteChar: %d\n", iValue );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)&iValue, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteChar)(iValue);
}

void pfnWriteShort(int iValue)
{
	UTIL_LogTPrintf( "pfnWriteShort: %d\n", iValue );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)&iValue, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteShort)(iValue);
}

void pfnWriteLong(int iValue)
{
	UTIL_LogTPrintf( "pfnWriteLong: %d\n", iValue );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)&iValue, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteLong)(iValue);
}

void pfnWriteAngle(float flValue)
{
	UTIL_LogTPrintf( "pfnWriteAngle: flValue=%f\n", flValue );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)&flValue, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteAngle)(flValue);
}

void pfnWriteCoord(float flValue)
{
	UTIL_LogTPrintf( "pfnWriteCoord: flValue=%f\n", flValue );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)&flValue, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteCoord)(flValue);
}

void pfnWriteString(const char *sz)
{
	UTIL_LogTPrintf( "pfnWriteString: sz=%s\n", sz );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)sz, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteString)(sz);
}

void pfnWriteEntity(int iValue)
{
	UTIL_LogTPrintf( "pfnWriteEntity: iValue=%d\n", iValue );

	// if this message is for a bot, call the client message function...
	if( botMsgFunction )
	{
		(*botMsgFunction)((void *)&iValue, botMsgIndex);
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnWriteEntity)(iValue);
}

void pfnCVarRegister(cvar_t *pCvar)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCVarRegister)(pCvar);
}

float pfnCVarGetFloat(const char *szVarName)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnCVarGetFloat)(szVarName);
}

const char* pfnCVarGetString(const char *szVarName)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, "" );

	return (*g_engfuncs.pfnCVarGetString)(szVarName);
}

void pfnCVarSetFloat(const char *szVarName, float flValue)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCVarSetFloat)(szVarName, flValue);
}

void pfnCVarSetString(const char *szVarName, const char *szValue)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCVarSetString)(szVarName, szValue);
}

void pfnEngineFprintf( void *pfile, char *szFmt, ... )
{
	va_list argptr;
	static char string[1024];

	va_start( argptr, szFmt );
	_vsnprintf( string, sizeof( string ), szFmt, argptr );
	va_end( argptr );

	(*g_engfuncs.pfnEngineFprintf)(pfile, string);

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_SUPERCEDE );
}

void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... )
{
	va_list argptr;
	static char string[1024];

	va_start( argptr, szFmt );
	_vsnprintf( string, sizeof( string ), szFmt, argptr );
	va_end( argptr );

	(*g_engfuncs.pfnAlertMessage)(atype, string);

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_SUPERCEDE );
}

void* pfnPvAllocEntPrivateData(edict_t *pEdict, int32 cb)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnPvAllocEntPrivateData)(pEdict, cb);
}

void* pfnPvEntPrivateData(edict_t *pEdict)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnPvEntPrivateData)(pEdict);
}

void pfnFreeEntPrivateData(edict_t *pEdict)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnFreeEntPrivateData)(pEdict);
}

const char* pfnSzFromIndex(int iString)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, "" );

	return (*g_engfuncs.pfnSzFromIndex)(iString);
}

int pfnAllocString(const char *szValue)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnAllocString)(szValue);
}

entvars_t* pfnGetVarsOfEnt(edict_t *pEdict)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnGetVarsOfEnt)(pEdict);
}

edict_t* pfnPEntityOfEntOffset(int iEntOffset)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnPEntityOfEntOffset)(iEntOffset);
}

int pfnEntOffsetOfPEntity(const edict_t *pEdict)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnEntOffsetOfPEntity)(pEdict);
}

int pfnIndexOfEdict(const edict_t *pEdict)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnIndexOfEdict)(pEdict);
}

edict_t* pfnPEntityOfEntIndex(int iEntIndex)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnPEntityOfEntIndex)(iEntIndex);
}

edict_t* pfnFindEntityByVars(entvars_t* pvars)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnFindEntityByVars)(pvars);
}

void* pfnGetModelPtr(edict_t* pEdict)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnGetModelPtr)(pEdict);
}

int pfnRegUserMsg(const char *pszName, int iSize)
{
	int msg = (*g_engfuncs.pfnRegUserMsg)(pszName, iSize);

	UTIL_LogDPrintf("pfnRegUserMsg: pszName=%s iSize=%d msg=%d\n", pszName, iSize, msg);

	if( mod_id == VALVE_DLL )
	{
		if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
	}
	else if( mod_id == GEARBOX_DLL )
	{
		if (strcmp(pszName, "VGUIMenu") == 0)
			message_VGUI = msg;
		else if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
	}
	else if( mod_id == CSTRIKE_DLL )
	{
		if (strcmp(pszName, "VGUIMenu") == 0)
			message_VGUI = msg;
		else if (strcmp(pszName, "ShowMenu") == 0)
			message_ShowMenu = msg;
		else if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "Money") == 0)
			message_Money = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
		else if (strcmp(pszName, "HLTV") == 0)
			message_HLTV = msg;
	}
	else if( mod_id == DOD_DLL )
	{
		if (strcmp(pszName, "VGUIMenu") == 0)
			message_VGUI = msg;
		else if (strcmp(pszName, "ShowMenu") == 0)
			message_ShowMenu = msg;
		else if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
		else if (strcmp(pszName, "HLTV") == 0)
			message_HLTV = msg;
	}
	else if( mod_id == TFC_DLL )
	{
		if (strcmp(pszName, "VGUIMenu") == 0)
			message_VGUI = msg;
		else if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
	}
	else if( mod_id == REWOLF_DLL )
	{
		if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
	}
	else if( mod_id == NS_DLL )
	{
		if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
		else if( !strcmp( pszName, "Countdown" ) )
		{
			gmsgCountdown = msg;
		}
		else if( !strcmp( pszName, "GameStatus" ) )
		{
			gmsgGameStatus = msg;
		}
	}
	if( mod_id == HUNGER_DLL )
	{
		if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
	}
	if( mod_id == SHIP_DLL )
	{
		if (strcmp(pszName, "WeaponList") == 0)
			message_WeaponList = msg;
		else if (strcmp(pszName, "CurWeapon") == 0)
			message_CurWeapon = msg;
		else if (strcmp(pszName, "AmmoX") == 0)
			message_AmmoX = msg;
		else if (strcmp(pszName, "AmmoPickup") == 0)
			message_AmmoPickup = msg;
		else if (strcmp(pszName, "Damage") == 0)
			message_Damage = msg;
		else if (strcmp(pszName, "DeathMsg") == 0)
			message_DeathMsg = msg;
		else if (strcmp(pszName, "ScreenFade") == 0)
			message_ScreenFade = msg;
		// TODO: check the name
		else if( !strcmp( pszName, "Quarry" ) )
		{
			gmsgQuarry = msg;
		}
	}

	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_SUPERCEDE, msg );

	return msg;
}

void pfnAnimationAutomove(const edict_t* pEdict, float flTime)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnAnimationAutomove)(pEdict, flTime);
}

void pfnGetBonePosition(const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnGetBonePosition)(pEdict, iBone, rgflOrigin, rgflAngles);
}

uint32 pfnFunctionFromName( const char *pName )
{
	uint32 iAddress = NameToAddress( pName );

	if( g_bIsMMPlugin )
	{
		if( iAddress )
		{
			return METAMOD_RETURN_VALUE( MRES_SUPERCEDE, iAddress );
		}
		else
		{
			return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );
		}
	}

	if( iAddress )
	{
		return iAddress;
	}
	else
	{
		return (*g_engfuncs.pfnFunctionFromName)(pName);
	}
}

const char *pfnNameForFunction( uint32 function )
{
	const char *szName = AddressToName( function );

	if( g_bIsMMPlugin )
	{
		if( szName )
		{
			return METAMOD_RETURN_VALUE( MRES_SUPERCEDE, szName );
		}
		else
		{
			return METAMOD_RETURN_VALUE( MRES_IGNORED, "" );
		}
	}

	if( szName )
	{
		return szName;
	}
	else
	{
		return (*g_engfuncs.pfnNameForFunction)(function);
	}
}

void pfnClientPrintf( edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg )
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnClientPrintf:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnClientPrintf)(pEdict, ptype, szMsg);
}

void pfnServerPrint( const char *szMsg )
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnServerPrint: %s\n",szMsg); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnServerPrint)(szMsg);
}

void pfnGetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles )
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnGetAttachment)(pEdict, iAttachment, rgflOrigin, rgflAngles);
}

void pfnCRC32_Init(CRC32_t *pulCRC)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCRC32_Init)(pulCRC);
}

void pfnCRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCRC32_ProcessBuffer)(pulCRC, p, len);
}

void pfnCRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCRC32_ProcessByte)(pulCRC, ch);
}

CRC32_t pfnCRC32_Final(CRC32_t pulCRC)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnCRC32_Final)(pulCRC);
}

int32 pfnRandomLong(int32 lLow, int32 lHigh)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnRandomLong)(lLow, lHigh);
}

float pfnRandomFloat(float flLow, float flHigh)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnRandomFloat)(flLow, flHigh);
}

void pfnSetView(const edict_t *pClient, const edict_t *pViewent )
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnSetView)(pClient, pViewent);
}

float pfnTime( void )
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnTime)();
}

void pfnCrosshairAngle(const edict_t *pClient, float pitch, float yaw)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCrosshairAngle)(pClient, pitch, yaw);
}

byte *pfnLoadFileForMe(char *filename, int *pLength)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnLoadFileForMe: filename=%s\n",filename); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

   return (*g_engfuncs.pfnLoadFileForMe)(filename, pLength);
}

void pfnFreeFile(void *buffer)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnFreeFile)(buffer);
}

void pfnEndSection(const char *pszSectionName)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnEndSection)(pszSectionName);
}

int pfnCompareFileTime(char *filename1, char *filename2, int *iCompare)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnCompareFileTime)(filename1, filename2, iCompare);
}

void pfnGetGameDir(char *szGetGameDir)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnGetGameDir)(szGetGameDir);
}

void pfnCvar_RegisterVariable(cvar_t *variable)
{
	UTIL_LogTPrintf("pfnCvar_RegisterVariable: variable->name=%s\n", variable->name );

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnCvar_RegisterVariable)(variable);
}

void pfnFadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
	UTIL_LogTPrintf("pfnFadeClientVolume: pEdict=%x fadePercent=%d fadeOutSeconds=%d holdTime=%d fadeInSeconds=%d\n", pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnFadeClientVolume)(pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);
}

void pfnSetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed)
{
	UTIL_LogTPrintf("pfnSetClientMaxspeed: pEdict=%x fNewMaxSpeed=%f\n", pEdict, fNewMaxspeed);

	// TODO: NS_DLL and TFC_DLL too? different classes have different speeds
	if( mod_id == CSTRIKE_DLL || mod_id == DOD_DLL )
	{
		bot_t *pBot = UTIL_GetBotPointer((edict_t *)pEdict);

		if ( pBot )
		{
			pBot->SetMaxSpeed( fNewMaxspeed );
			pBot->pEdict->v.maxspeed = fNewMaxspeed;
		}
	}

	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnSetClientMaxspeed)(pEdict, fNewMaxspeed);
}

edict_t * pfnCreateFakeClient(const char *netname)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnCreateFakeClient)(netname);
}

void pfnRunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec )
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnRunPlayerMove)(fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec);
}

int pfnNumberOfEntities(void)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnNumberOfEntities)();
}

char* pfnGetInfoKeyBuffer(edict_t *e)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetInfoKeyBuffer:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

   return (*g_engfuncs.pfnGetInfoKeyBuffer)(e);
}

char* pfnInfoKeyValue(char *infobuffer, char *key)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnInfoKeyValue: %s %s\n",infobuffer,key); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

   return (*g_engfuncs.pfnInfoKeyValue)(infobuffer, key);
}

void pfnSetKeyValue(char *infobuffer, char *key, char *value)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetKeyValue: %s %s\n",key,value); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnSetKeyValue)(infobuffer, key, value);
}

void pfnSetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetClientKeyValue: %s %s\n",key,value); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnSetClientKeyValue)(clientIndex, infobuffer, key, value);
}

int pfnIsMapValid(char *filename)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnIsMapValid)(filename);
}

void pfnStaticDecal( const float *origin, int decalIndex, int entityIndex, int modelIndex )
{
   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnStaticDecal)(origin, decalIndex, entityIndex, modelIndex);
}

int pfnPrecacheGeneric(char* s)
{
   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

   return (*g_engfuncs.pfnPrecacheGeneric)(s);
}

int pfnGetPlayerUserId(edict_t *e )
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerUserId: %p\n",e); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

   return (*g_engfuncs.pfnGetPlayerUserId)(e);
}

void pfnBuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnBuildSoundMsg)(entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed);
}

int pfnIsDedicatedServer(void)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnIsDedicatedServer)();
}

cvar_t* pfnCVarGetPointer(const char *szVarName)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnCVarGetPointer)(szVarName);
}

unsigned int pfnGetPlayerWONId(edict_t *e)
{
   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

   return (*g_engfuncs.pfnGetPlayerWONId)(e);
}


// new stuff for SDK 2.0

void pfnInfo_RemoveKey(char *s, const char *key)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnInfo_RemoveKey:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnInfo_RemoveKey)(s, key);
}

const char *pfnGetPhysicsKeyValue(const edict_t *pClient, const char *key)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPhysicsKeyValue:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, "" );

   return (*g_engfuncs.pfnGetPhysicsKeyValue)(pClient, key);
}

void pfnSetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetPhysicsKeyValue:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnSetPhysicsKeyValue)(pClient, key, value);
}

const char *pfnGetPhysicsInfoString(const edict_t *pClient)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPhysicsInfoString:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

   return (*g_engfuncs.pfnGetPhysicsInfoString)(pClient);
}

unsigned short pfnPrecacheEvent(int type, const char *psz)
{
   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

   return (*g_engfuncs.pfnPrecacheEvent)(type, psz);
}

void pfnPlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1,float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnPlaybackEvent)(flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2);
}

unsigned char *pfnSetFatPVS(float *org)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnSetFatPVS)(org);
}

unsigned char *pfnSetFatPAS(float *org)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnSetFatPAS)(org);
}

int pfnCheckVisibility(const edict_t *entity, unsigned char *pset)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnCheckVisibility)(entity, pset);
}

void pfnDeltaSetField(struct delta_s *pFields, const char *fieldname)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnDeltaSetField)(pFields, fieldname);
}

void pfnDeltaUnsetField(struct delta_s *pFields, const char *fieldname)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnDeltaUnsetField)(pFields, fieldname);
}

void pfnDeltaAddEncoder(char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to))
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnDeltaAddEncoder)(name, conditionalencode);
}

int pfnGetCurrentPlayer(void)
{
   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

   return (*g_engfuncs.pfnGetCurrentPlayer)();
}

int pfnCanSkipPlayer(const edict_t *player)
{
   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

   return (*g_engfuncs.pfnCanSkipPlayer)(player);
}

int pfnDeltaFindField(struct delta_s *pFields, const char *fieldname)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnDeltaFindField)(pFields, fieldname);
}

void pfnDeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnDeltaSetFieldByIndex)(pFields, fieldNumber);
}

void pfnDeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnDeltaUnsetFieldByIndex)(pFields, fieldNumber);
}

void pfnSetGroupMask(int mask, int op)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnSetGroupMask:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnSetGroupMask)(mask, op);
}

int pfnCreateInstancedBaseline(int classname, struct entity_state_s *baseline)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCreateInstancedBaseline:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

   return (*g_engfuncs.pfnCreateInstancedBaseline)(classname, baseline);
}

void pfnCvar_DirectSet(struct cvar_s *var, char *value)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCvar_DirectSet:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnCvar_DirectSet)(var, value);
}

void pfnForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnForceUnmodified:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnForceUnmodified)(type, mins, maxs, filename);
}

void pfnGetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss)
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerStats:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnGetPlayerStats)(pClient, ping, packet_loss);
}


// new stuff for SDK 2.3

void pfnAddServerCommand( char *cmd_name, void (*function) (void) )
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAddServerCommand:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   METAMOD_RETURN( MRES_IGNORED );

   (*g_engfuncs.pfnAddServerCommand)(cmd_name, function);
}

qboolean pfnVoice_GetClientListening(int iReceiver, int iSender)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnVoice_GetClientListening)(iReceiver, iSender);
}

qboolean pfnVoice_SetClientListening(int iReceiver, int iSender, qboolean bListen)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnVoice_SetClientListening)(iReceiver, iSender, bListen);
}

const char *pfnGetPlayerAuthId( edict_t *e )
{
   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetPlayerAuthId:\n"); fclose(fp); }

   if( g_bIsMMPlugin )
	   return METAMOD_RETURN_VALUE( MRES_IGNORED, "" );

   return (*g_engfuncs.pfnGetPlayerAuthId)(e);
}

void* pfnSequenceGet( const char* fileName, const char* entryName )
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnSequenceGet)(fileName, entryName);
}

void* pfnSequencePickSentence( const char* groupName, int pickMethod, int *picked )
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, nullptr );

	return (*g_engfuncs.pfnSequencePickSentence)(groupName, pickMethod, picked);
}

int pfnGetFileSize( char *filename )
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnGetFileSize)(filename);
}

unsigned int pfnGetApproxWavePlayLen(const char *filepath)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnGetApproxWavePlayLen)(filepath);
}

int pfnIsCareerMatch( void )
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnIsCareerMatch)();
}

int pfnGetLocalizedStringLength(const char *label)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnGetLocalizedStringLength)(label);
}

void pfnRegisterTutorMessageShown(int mid)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.pfnRegisterTutorMessageShown)(mid);
}

int pfnGetTimesTutorMessageShown(int mid)
{
	if( g_bIsMMPlugin )
		return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );

	return (*g_engfuncs.pfnGetTimesTutorMessageShown)(mid);
}

void pfnProcessTutorMessageDecayBuffer(int *buffer, int bufferLength)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.ProcessTutorMessageDecayBuffer)(buffer, bufferLength);
}

void pfnConstructTutorMessageDecayBuffer(int *buffer, int bufferLength)
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.ConstructTutorMessageDecayBuffer)(buffer, bufferLength);
}

void pfnResetTutorMessageDecayData( void )
{
	if( g_bIsMMPlugin )
		METAMOD_RETURN( MRES_IGNORED );

	(*g_engfuncs.ResetTutorMessageDecayData)();
}
