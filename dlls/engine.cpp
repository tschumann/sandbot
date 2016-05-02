//
// gbot - The GoldSource bot
//
// <no site>
//
// engine.cpp
//

#include "extdll.h"
#include "util.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_client.h"
#include "engine.h"
#include "dll.h"


extern enginefuncs_t g_engfuncs;
extern bot_t bots[32];
extern int g_iMod;

int debug_engine = 0;

void (*botMsgFunction)( void *, int ) = NULL;
void (*botMsgEndFunction)( void *, int ) = NULL;

int botMsgIndex;

// messages created in RegUserMsg which will be "caught"
int gmsgVGUI = 0;
int gmsgShowMenu = 0;
int gmsgWeaponList = 0;
int gmsgCurWeapon = 0;
int gmsgAmmoX = 0;
int gmsgAmmoPickup = 0;
int gmsgDamage = 0;
int gmsgDeathMsg = 0;
int gmsgTextMsg = 0;
int gmsgScreenFade = 0;
int gmsgCountdown = 0;	// Natural Selection

static FILE *fp;

int pfnPrecacheModel(char* s)
{
    return (*g_engfuncs.pfnPrecacheModel)(s);
}
int pfnPrecacheSound(char* s)
{
    return (*g_engfuncs.pfnPrecacheSound)(s);
}
void pfnSetModel(edict_t *e, const char *m)
{
    (*g_engfuncs.pfnSetModel)(e, m);
}
int pfnModelIndex(const char *m)
{
    return (*g_engfuncs.pfnModelIndex)(m);
}
int pfnModelFrames(int modelIndex)
{
    return (*g_engfuncs.pfnModelFrames)(modelIndex);
}
void pfnSetSize(edict_t *e, const float *rgflMin, const float *rgflMax)
{
    (*g_engfuncs.pfnSetSize)(e, rgflMin, rgflMax);
}
void pfnChangeLevel(char* s1, char* s2)
{
	// kick any bot off of the server after time/frag limit...
    for( int i = 0; i < 32; i++)
    {
        if( bots[i].is_used )  // is this slot used?
        {
            char szCommand[40];

            sprintf( szCommand, "kick \"%s\"\n", bots[i].name );

            bots[i].respawn_state = RESPAWN_NEED_TO_RESPAWN;

            SERVER_COMMAND(szCommand);  // kick the bot using (kick "name")
        }
    }

    (*g_engfuncs.pfnChangeLevel)(s1, s2);
}
void pfnGetSpawnParms(edict_t *ent)
{
    (*g_engfuncs.pfnGetSpawnParms)(ent);
}
void pfnSaveSpawnParms(edict_t *ent)
{
    (*g_engfuncs.pfnSaveSpawnParms)(ent);
}
float pfnVecToYaw(const float *rgflVector)
{
    return (*g_engfuncs.pfnVecToYaw)(rgflVector);
}
void pfnVecToAngles(const float *rgflVectorIn, float *rgflVectorOut)
{
    (*g_engfuncs.pfnVecToAngles)(rgflVectorIn, rgflVectorOut);
}
void pfnMoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType)
{
    (*g_engfuncs.pfnMoveToOrigin)(ent, pflGoal, dist, iMoveType);
}
void pfnChangeYaw(edict_t* ent)
{
    (*g_engfuncs.pfnChangeYaw)(ent);
}
void pfnChangePitch(edict_t* ent)
{
    (*g_engfuncs.pfnChangePitch)(ent);
}
edict_t* pfnFindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue)
{
    return (*g_engfuncs.pfnFindEntityByString)(pEdictStartSearchAfter, pszField, pszValue);
}
int pfnGetEntityIllum(edict_t* pEnt)
{
    return (*g_engfuncs.pfnGetEntityIllum)(pEnt);
}
edict_t* pfnFindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad)
{
    return (*g_engfuncs.pfnFindEntityInSphere)(pEdictStartSearchAfter, org, rad);
}
edict_t* pfnFindClientInPVS(edict_t *pEdict)
{
    return (*g_engfuncs.pfnFindClientInPVS)(pEdict);
}
edict_t* pfnEntitiesInPVS(edict_t *pplayer)
{
    return (*g_engfuncs.pfnEntitiesInPVS)(pplayer);
}
void pfnMakeVectors(const float *rgflVector)
{
    (*g_engfuncs.pfnMakeVectors)(rgflVector);
}
void pfnAngleVectors(const float *rgflVector, float *forward, float *right, float *up)
{
    (*g_engfuncs.pfnAngleVectors)(rgflVector, forward, right, up);
}
edict_t* pfnCreateEntity(void)
{
    return  (*g_engfuncs.pfnCreateEntity)();
}
void pfnRemoveEntity(edict_t* e)
{
	(*g_engfuncs.pfnRemoveEntity)(e);
}
edict_t* pfnCreateNamedEntity(int className)
{
    return (*g_engfuncs.pfnCreateNamedEntity)(className);
}
void pfnMakeStatic(edict_t *ent)
{
    (*g_engfuncs.pfnMakeStatic)(ent);
}
int pfnEntIsOnFloor(edict_t *e)
{
    return (*g_engfuncs.pfnEntIsOnFloor)(e);
}
int pfnDropToFloor(edict_t* e)
{
    return (*g_engfuncs.pfnDropToFloor)(e);
}
int pfnWalkMove(edict_t *ent, float yaw, float dist, int iMode)
{
    return (*g_engfuncs.pfnWalkMove)(ent, yaw, dist, iMode);
}
void pfnSetOrigin(edict_t *e, const float *rgflOrigin)
{
    (*g_engfuncs.pfnSetOrigin)(e, rgflOrigin);
}
void pfnEmitSound(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch)
{
    (*g_engfuncs.pfnEmitSound)(entity, channel, sample, volume, attenuation, fFlags, pitch);
}
void pfnEmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch)
{
    (*g_engfuncs.pfnEmitAmbientSound)(entity, pos, samp, vol, attenuation, fFlags, pitch);
}
void pfnTraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
    (*g_engfuncs.pfnTraceLine)(v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr)
{
    (*g_engfuncs.pfnTraceToss)(pent, pentToIgnore, ptr);
}
int pfnTraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
    return (*g_engfuncs.pfnTraceMonsterHull)(pEdict, v1, v2, fNoMonsters, pentToSkip, ptr);
}
void pfnTraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr)
{
    (*g_engfuncs.pfnTraceHull)(v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr);
}
void pfnTraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr)
{
    (*g_engfuncs.pfnTraceModel)(v1, v2, hullNumber, pent, ptr);
}
const char *pfnTraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 )
{
    return (*g_engfuncs.pfnTraceTexture)(pTextureEntity, v1, v2);
}
void pfnTraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr)
{
    (*g_engfuncs.pfnTraceSphere)(v1, v2, fNoMonsters, radius, pentToSkip, ptr);
}
void pfnGetAimVector(edict_t* ent, float speed, float *rgflReturn)
{
    (*g_engfuncs.pfnGetAimVector)(ent, speed, rgflReturn);
}
void pfnServerCommand(char* str)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnServerCommand: %s\n",str); fclose(fp); }
    (*g_engfuncs.pfnServerCommand)(str);
}
void pfnServerExecute(void)
{
    (*g_engfuncs.pfnServerExecute)();
}
void pfnClientCommand(edict_t* pEdict, char* szFmt, ...)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnClientCommand=%s\n",szFmt); fclose(fp); }
    return;
}
void pfnParticleEffect(const float *org, const float *dir, float color, float count)
{
    (*g_engfuncs.pfnParticleEffect)(org, dir, color, count);
}
void pfnLightStyle(int style, char* val)
{
    (*g_engfuncs.pfnLightStyle)(style, val);
}
int pfnDecalIndex(const char *name)
{
    return (*g_engfuncs.pfnDecalIndex)(name);
}
int pfnPointContents(const float *rgflVector)
{
    return (*g_engfuncs.pfnPointContents)(rgflVector);
}
void pfnMessageBegin( int msg_dest, int msg_type, const float *pOrigin, edict_t *ed )
{
    if( gpGlobals->deathmatch )
    {
        int index = -1;

        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnMessageBegin: edict=%x dest=%d type=%d\n",ed,msg_dest,msg_type); fclose(fp); }

        if( ed )
        {
            index = UTIL_GetBotIndex( ed );

            // is this message for a bot?
            if( index != -1 )
            {
                botMsgFunction = NULL;      // no msg function until known otherwise
                botMsgEndFunction = NULL;  // no msg end function until known otherwise
                botMsgIndex = index;         // index of bot receiving message

                if( g_iMod == VALVE_DLL || g_iMod == HUNGER_DLL || g_iMod == SHIP_DLL )
                {
                    if( msg_type == gmsgWeaponList )
					{
                        botMsgFunction = BotClient_Valve_WeaponList;
					}
                    else if( msg_type == gmsgCurWeapon )
					{
                        botMsgFunction = BotClient_Valve_CurrentWeapon;
					}
                    else if( msg_type == gmsgAmmoX )
					{
                        botMsgFunction = BotClient_Valve_AmmoX;
					}
                    else if( msg_type == gmsgAmmoPickup )
					{
                        botMsgFunction = BotClient_Valve_AmmoPickup;
					}
                    else if( msg_type == gmsgDamage )
					{
                        botMsgFunction = BotClient_Valve_Damage;
					}
                    else if( msg_type == gmsgScreenFade )
					{
                        botMsgFunction = BotClient_Valve_ScreenFade;
					}
                }
				else if( g_iMod == GEARBOX_DLL )
				{
					if( msg_type == gmsgWeaponList )
					{
                        botMsgFunction = BotClient_Gearbox_WeaponList;
					}
                    else if( msg_type == gmsgCurWeapon )
					{
                        botMsgFunction = BotClient_Gearbox_CurrentWeapon;
					}
                    else if( msg_type == gmsgAmmoX )
					{
                        botMsgFunction = BotClient_Gearbox_AmmoX;
					}
                    else if( msg_type == gmsgAmmoPickup )
					{
                        botMsgFunction = BotClient_Gearbox_AmmoPickup;
					}
                    else if( msg_type == gmsgDamage )
					{
                        botMsgFunction = BotClient_Gearbox_Damage;
					}
                    else if( msg_type == gmsgScreenFade )
					{
                        botMsgFunction = BotClient_Gearbox_ScreenFade;
					}
				}
				else if( g_iMod == GUNMAN_DLL )
                {
                    if (msg_type == gmsgWeaponList)
                        botMsgFunction = BotClient_Gunman_WeaponList;
                    else if (msg_type == gmsgCurWeapon)
                        botMsgFunction = BotClient_Valve_CurrentWeapon;
                    else if (msg_type == gmsgAmmoX)
                        botMsgFunction = BotClient_Valve_AmmoX;
                    else if (msg_type == gmsgAmmoPickup)
                        botMsgFunction = BotClient_Valve_AmmoPickup;
                    else if (msg_type == gmsgDamage)
                        botMsgFunction = BotClient_Valve_Damage;
                    else if (msg_type == gmsgScreenFade)
                        botMsgFunction = BotClient_Valve_ScreenFade;
                }
				else if( g_iMod == NS_DLL )
                {
                    if (msg_type == gmsgWeaponList)
                        botMsgFunction = BotClient_NS_WeaponList;
                    else if (msg_type == gmsgCurWeapon)
                        botMsgFunction = BotClient_NS_CurrentWeapon;
                    else if (msg_type == gmsgAmmoX)
                        botMsgFunction = BotClient_NS_AmmoX;
                    else if (msg_type == gmsgAmmoPickup)
                        botMsgFunction = BotClient_NS_AmmoPickup;
                    else if (msg_type == gmsgDamage)
                        botMsgFunction = BotClient_NS_Damage;
                    else if (msg_type == gmsgScreenFade)
                        botMsgFunction = BotClient_NS_ScreenFade;
                }
            }
        }
        else if( msg_dest == MSG_ALL )
        {
            botMsgFunction = NULL;  // no msg function until known otherwise
            botMsgIndex = -1;         // index of bot receiving message (none)

            if( g_iMod == VALVE_DLL || g_iMod == HUNGER_DLL || g_iMod == SHIP_DLL )
            {
                if( msg_type == gmsgDeathMsg )
				{
                    botMsgFunction = BotClient_Valve_DeathMsg;
				}
            }
			else if( g_iMod == GEARBOX_DLL )
			{
				if( msg_type == gmsgDeathMsg )
				{
                    botMsgFunction = BotClient_Gearbox_DeathMsg;
				}
			}
			else if( g_iMod == GUNMAN_DLL )
			{
				if( msg_type == gmsgDeathMsg )
				{
                    botMsgFunction = BotClient_Gunman_DeathMsg;
				}
			}
			else if( g_iMod == NS_DLL )
            {
                if( msg_type == gmsgDeathMsg )
				{
                    botMsgFunction = BotClient_NS_DeathMsg;
				}
				else if( msg_type == gmsgCountdown )
				{
					botMsgFunction = BotClient_NS_CountDown;
				}
            }
        }
        else
        {
            // Steam makes the WeaponList message be sent differently

            botMsgFunction = NULL;	// no msg function until known otherwise
            botMsgIndex = -1;		// index of bot receiving message (none)

            if( g_iMod == VALVE_DLL || g_iMod == HUNGER_DLL || g_iMod == SHIP_DLL )
            {
                if( msg_type == gmsgWeaponList )
				{
                    botMsgFunction = BotClient_Valve_WeaponList;
				}
            }
			else if( g_iMod == GEARBOX_DLL )
			{
				if( msg_type == gmsgWeaponList )
				{
                    botMsgFunction = BotClient_Gearbox_WeaponList;
				}
			}
			else if( g_iMod == GUNMAN_DLL )
            {
                if( msg_type == gmsgWeaponList )
				{
                    botMsgFunction = BotClient_Gunman_WeaponList;
				}
            }
			else if( g_iMod == NS_DLL )
            {
                if( msg_type == gmsgWeaponList )
				{
                    botMsgFunction = BotClient_NS_WeaponList;
				}
            }
        }
    }

    (*g_engfuncs.pfnMessageBegin)( msg_dest, msg_type, pOrigin, ed );
}

void pfnMessageEnd(void)
{
    if( gpGlobals->deathmatch )
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnMessageEnd:\n"); fclose(fp); }

        if( botMsgEndFunction )
		{
            (*botMsgEndFunction)( NULL, botMsgIndex );  // NULL indicated msg end
		}

        // clear out the bot message function pointers...
        botMsgFunction = NULL;
        botMsgEndFunction = NULL;
    }

    (*g_engfuncs.pfnMessageEnd)();
}

void pfnWriteByte(int iValue)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteByte: %d\n",iValue); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
		{
            (*botMsgFunction)( (void *)&iValue, botMsgIndex );
		}
    }

    (*g_engfuncs.pfnWriteByte)(iValue);
}

void pfnWriteChar(int iValue)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteChar: %d\n",iValue); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
            (*botMsgFunction)((void *)&iValue, botMsgIndex);
    }

    (*g_engfuncs.pfnWriteChar)(iValue);
}

void pfnWriteShort(int iValue)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteShort: %d\n",iValue); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
            (*botMsgFunction)((void *)&iValue, botMsgIndex);
    }

    (*g_engfuncs.pfnWriteShort)(iValue);
}

void pfnWriteLong(int iValue)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteLong: %d\n",iValue); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
            (*botMsgFunction)((void *)&iValue, botMsgIndex);
    }

    (*g_engfuncs.pfnWriteLong)(iValue);
}

void pfnWriteAngle(float flValue)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteAngle: %f\n",flValue); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
            (*botMsgFunction)((void *)&flValue, botMsgIndex);
    }

    (*g_engfuncs.pfnWriteAngle)(flValue);
}

void pfnWriteCoord(float flValue)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteCoord: %f\n",flValue); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
            (*botMsgFunction)((void *)&flValue, botMsgIndex);
    }

    (*g_engfuncs.pfnWriteCoord)(flValue);
}

void pfnWriteString(const char *sz)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteString: %s\n",sz); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
            (*botMsgFunction)((void *)sz, botMsgIndex);
    }

    (*g_engfuncs.pfnWriteString)(sz);
}

void pfnWriteEntity(int iValue)
{
    if (gpGlobals->deathmatch)
    {
        if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnWriteEntity: %d\n",iValue); fclose(fp); }

        // if this message is for a bot, call the client message function...
        if (botMsgFunction)
            (*botMsgFunction)((void *)&iValue, botMsgIndex);
    }

    (*g_engfuncs.pfnWriteEntity)(iValue);
}

void pfnCVarRegister(cvar_t *pCvar)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnCVarRegister:\n"); fclose(fp); }
    (*g_engfuncs.pfnCVarRegister)(pCvar);
}

float pfnCVarGetFloat(const char *szVarName)
{
    return (*g_engfuncs.pfnCVarGetFloat)(szVarName);
}

const char* pfnCVarGetString(const char *szVarName)
{
    return (*g_engfuncs.pfnCVarGetString)(szVarName);
}

void pfnCVarSetFloat(const char *szVarName, float flValue)
{
    (*g_engfuncs.pfnCVarSetFloat)(szVarName, flValue);
}

void pfnCVarSetString(const char *szVarName, const char *szValue)
{
    (*g_engfuncs.pfnCVarSetString)(szVarName, szValue);
}

void pfnEngineFprintf( void *pfile, char *szFmt, ... )
{
    va_list argptr;
    static char string[1024];

    va_start( argptr, szFmt );
    vsprintf( string, szFmt, argptr );
    va_end( argptr );

    (*g_engfuncs.pfnEngineFprintf)( pfile, string );
}

void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... )
{
    va_list argptr;
    static char string[1024];

    va_start (argptr, szFmt);
    vsprintf (string, szFmt, argptr);
    va_end (argptr);

    (*g_engfuncs.pfnAlertMessage)(atype, string);
}

void* pfnPvAllocEntPrivateData(edict_t *pEdict, int32 cb)
{
    return (*g_engfuncs.pfnPvAllocEntPrivateData)(pEdict, cb);
}

void* pfnPvEntPrivateData(edict_t *pEdict)
{
    return (*g_engfuncs.pfnPvEntPrivateData)(pEdict);
}

void pfnFreeEntPrivateData(edict_t *pEdict)
{
    (*g_engfuncs.pfnFreeEntPrivateData)(pEdict);
}

const char* pfnSzFromIndex(int iString)
{
    return (*g_engfuncs.pfnSzFromIndex)(iString);
}

int pfnAllocString(const char *szValue)
{
    return (*g_engfuncs.pfnAllocString)(szValue);
}

entvars_t* pfnGetVarsOfEnt(edict_t *pEdict)
{
    return (*g_engfuncs.pfnGetVarsOfEnt)(pEdict);
}

edict_t* pfnPEntityOfEntOffset(int iEntOffset)
{
    return (*g_engfuncs.pfnPEntityOfEntOffset)(iEntOffset);
}

int pfnEntOffsetOfPEntity(const edict_t *pEdict)
{
    return (*g_engfuncs.pfnEntOffsetOfPEntity)(pEdict);
}

int pfnIndexOfEdict(const edict_t *pEdict)
{
    return (*g_engfuncs.pfnIndexOfEdict)(pEdict);
}

edict_t* pfnPEntityOfEntIndex(int iEntIndex)
{
    return (*g_engfuncs.pfnPEntityOfEntIndex)(iEntIndex);
}

edict_t* pfnFindEntityByVars(entvars_t* pvars)
{
    return (*g_engfuncs.pfnFindEntityByVars)(pvars);
}

void* pfnGetModelPtr(edict_t* pEdict)
{
    return (*g_engfuncs.pfnGetModelPtr)(pEdict);
}

int pfnRegUserMsg(const char *pszName, int iSize)
{
    int iMessage = (*g_engfuncs.pfnRegUserMsg)( pszName, iSize );

    if( gpGlobals->deathmatch )
    {
#ifdef _DEBUG
        fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnRegUserMsg: pszName=%s msg=%d\n",pszName,iMessage); fclose(fp);
#endif

		// TODO - split this
        if ( g_iMod == VALVE_DLL || g_iMod == GUNMAN_DLL || g_iMod == HUNGER_DLL || g_iMod == SHIP_DLL )
        {
			if( !strcmp( pszName, "WeaponList" ) )
			{
			   gmsgWeaponList = iMessage;
			}
			else if( !strcmp( pszName, "CurWeapon" ) )
			{
				gmsgCurWeapon = iMessage;
			}
			else if( !strcmp( pszName, "AmmoX" ) )
			{
				gmsgAmmoX = iMessage;
			}
			else if( !strcmp( pszName, "AmmoPickup" ) )
			{
				gmsgAmmoPickup = iMessage;
			}
			else if( !strcmp( pszName, "Damage" ) )
			{
				gmsgDamage = iMessage;
			}
			else if( !strcmp( pszName, "DeathMsg" ) )
			{
				gmsgDeathMsg = iMessage;
			}
			else if( !strcmp( pszName, "ScreenFade" ) )
			{
				gmsgScreenFade = iMessage;
			}
        }
		else if( g_iMod == NS_DLL )
		{
			if( !strcmp( pszName, "WeaponList" ) )
			{
			   gmsgWeaponList = iMessage;
			}
			else if( !strcmp( pszName, "CurWeapon" ) )
			{
				gmsgCurWeapon = iMessage;
			}
			else if( !strcmp( pszName, "AmmoX" ) )
			{
				gmsgAmmoX = iMessage;
			}
			else if( !strcmp( pszName, "AmmoPickup" ) )
			{
				gmsgAmmoPickup = iMessage;
			}
			else if( !strcmp( pszName, "Damage" ) )
			{
				gmsgDamage = iMessage;
			}
			else if( !strcmp( pszName, "DeathMsg" ) )
			{
				gmsgDeathMsg = iMessage;
			}
			else if( !strcmp( pszName, "ScreenFade" ) )
			{
				gmsgScreenFade = iMessage;
			}
			else if( !strcmp( pszName, "Countdown" ) )
			{
				gmsgCountdown = iMessage;
			}
		}
    }

    return iMessage;
}
void pfnAnimationAutomove( const edict_t* pEdict, float flTime )
{
    (*g_engfuncs.pfnAnimationAutomove)( pEdict, flTime );
}

void pfnGetBonePosition( const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
{
    (*g_engfuncs.pfnGetBonePosition)( pEdict, iBone, rgflOrigin, rgflAngles );
}

uint32 pfnFunctionFromName( const char *pName )
{
    return (*g_engfuncs.pfnFunctionFromName)( pName );
}

const char *pfnNameForFunction( uint32 function )
{
    return (*g_engfuncs.pfnNameForFunction)( function );
}

void pfnClientPrintf( edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg )
{
    (*g_engfuncs.pfnClientPrintf)( pEdict, ptype, szMsg );
}

void pfnServerPrint( const char *szMsg )
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnServerPrint: %s\n",szMsg); fclose(fp); }
    (*g_engfuncs.pfnServerPrint)(szMsg);
}

void pfnGetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles )
{
    (*g_engfuncs.pfnGetAttachment)(pEdict, iAttachment, rgflOrigin, rgflAngles);
}

void pfnCRC32_Init(CRC32_t *pulCRC)
{
    (*g_engfuncs.pfnCRC32_Init)(pulCRC);
}

void pfnCRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len)
{
    (*g_engfuncs.pfnCRC32_ProcessBuffer)(pulCRC, p, len);
}

void pfnCRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch)
{
    (*g_engfuncs.pfnCRC32_ProcessByte)(pulCRC, ch);
}

CRC32_t pfnCRC32_Final(CRC32_t pulCRC)
{
    return (*g_engfuncs.pfnCRC32_Final)(pulCRC);
}

int32 pfnRandomLong(int32 lLow, int32 lHigh)
{
    return (*g_engfuncs.pfnRandomLong)(lLow, lHigh);
}

float pfnRandomFloat(float flLow, float flHigh)
{
    return (*g_engfuncs.pfnRandomFloat)(flLow, flHigh);
}

void pfnSetView(const edict_t *pClient, const edict_t *pViewent )
{
    (*g_engfuncs.pfnSetView)(pClient, pViewent);
}

float pfnTime( void )
{
    return (*g_engfuncs.pfnTime)();
}

void pfnCrosshairAngle(const edict_t *pClient, float pitch, float yaw)
{
    (*g_engfuncs.pfnCrosshairAngle)(pClient, pitch, yaw);
}

byte *pfnLoadFileForMe(char *filename, int *pLength)
{
    return (*g_engfuncs.pfnLoadFileForMe)(filename, pLength);
}

void pfnFreeFile(void *buffer)
{
    (*g_engfuncs.pfnFreeFile)(buffer);
}

void pfnEndSection(const char *pszSectionName)
{
    (*g_engfuncs.pfnEndSection)(pszSectionName);
}

int pfnCompareFileTime(char *filename1, char *filename2, int *iCompare)
{
    return (*g_engfuncs.pfnCompareFileTime)(filename1, filename2, iCompare);
}

void pfnGetGameDir(char *szGetGameDir)
{
    (*g_engfuncs.pfnGetGameDir)(szGetGameDir);
}

void pfnCvar_RegisterVariable(cvar_t *variable)
{
    (*g_engfuncs.pfnCvar_RegisterVariable)(variable);
}

void pfnFadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
    (*g_engfuncs.pfnFadeClientVolume)(pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);
}

void pfnSetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnSetClientMaxspeed: edict=%x %f\n",pEdict,fNewMaxspeed); fclose(fp); }

    if( gpGlobals->deathmatch )
    {
        if( g_iMod == CSTRIKE_DLL )
        {
            bot_t *pBot = UTIL_GetBotPointer( (edict_t *)pEdict );

            if( pBot )
            {
                pBot->f_max_speed = fNewMaxspeed;
                pBot->pEdict->v.maxspeed = fNewMaxspeed;
            }
        }
    }

    (*g_engfuncs.pfnSetClientMaxspeed)(pEdict, fNewMaxspeed);
}

edict_t * pfnCreateFakeClient(const char *netname)
{
    return (*g_engfuncs.pfnCreateFakeClient)(netname);
}

void pfnRunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec )
{
    (*g_engfuncs.pfnRunPlayerMove)(fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec);
}

int pfnNumberOfEntities(void)
{
    return (*g_engfuncs.pfnNumberOfEntities)();
}

char* pfnGetInfoKeyBuffer(edict_t *e)
{
    return (*g_engfuncs.pfnGetInfoKeyBuffer)(e);
}

char* pfnInfoKeyValue(char *infobuffer, char *key)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnInfoKeyValue: %s %s\n",infobuffer,key); fclose(fp); }
    return (*g_engfuncs.pfnInfoKeyValue)(infobuffer, key);
}

void pfnSetKeyValue(char *infobuffer, char *key, char *value)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnSetKeyValue: %s %s\n",key,value); fclose(fp); }
    (*g_engfuncs.pfnSetKeyValue)(infobuffer, key, value);
}

void pfnSetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnSetClientKeyValue: %s %s\n",key,value); fclose(fp); }
    (*g_engfuncs.pfnSetClientKeyValue)(clientIndex, infobuffer, key, value);
}

int pfnIsMapValid(char *filename)
{
    return (*g_engfuncs.pfnIsMapValid)(filename);
}

void pfnStaticDecal( const float *origin, int decalIndex, int entityIndex, int modelIndex )
{
    (*g_engfuncs.pfnStaticDecal)(origin, decalIndex, entityIndex, modelIndex);
}

int pfnPrecacheGeneric(char* s)
{
    return (*g_engfuncs.pfnPrecacheGeneric)(s);
}

int pfnGetPlayerUserId(edict_t *e )
{
    return (*g_engfuncs.pfnGetPlayerUserId)(e);
}

void pfnBuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
    (*g_engfuncs.pfnBuildSoundMsg)(entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed);
}

int pfnIsDedicatedServer(void)
{
    return (*g_engfuncs.pfnIsDedicatedServer)();
}

cvar_t* pfnCVarGetPointer(const char *szVarName)
{
    if (debug_engine) { fopen_s(&fp,"bot.txt","a"); fprintf(fp,"pfnCVarGetPointer: %s\n",szVarName); fclose(fp); }
    return (*g_engfuncs.pfnCVarGetPointer)(szVarName);
}

unsigned int pfnGetPlayerWONId(edict_t *e)
{
    return (*g_engfuncs.pfnGetPlayerWONId)(e);
}


// new stuff for SDK 2.0

void pfnInfo_RemoveKey(char *s, const char *key)
{
    (*g_engfuncs.pfnInfo_RemoveKey)(s, key);
}

const char *pfnGetPhysicsKeyValue(const edict_t *pClient, const char *key)
{
    return (*g_engfuncs.pfnGetPhysicsKeyValue)(pClient, key);
}

void pfnSetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value)
{
    (*g_engfuncs.pfnSetPhysicsKeyValue)(pClient, key, value);
}

const char *pfnGetPhysicsInfoString(const edict_t *pClient)
{
    return (*g_engfuncs.pfnGetPhysicsInfoString)(pClient);
}

unsigned short pfnPrecacheEvent(int type, const char *psz)
{
    return (*g_engfuncs.pfnPrecacheEvent)(type, psz);
}

void pfnPlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1,float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
    (*g_engfuncs.pfnPlaybackEvent)(flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2);
}

unsigned char *pfnSetFatPVS(float *org)
{
    return (*g_engfuncs.pfnSetFatPVS)(org);
}

unsigned char *pfnSetFatPAS(float *org)
{
    return (*g_engfuncs.pfnSetFatPAS)(org);
}

int pfnCheckVisibility(const edict_t *entity, unsigned char *pset)
{
    return (*g_engfuncs.pfnCheckVisibility)(entity, pset);
}

void pfnDeltaSetField(struct delta_s *pFields, const char *fieldname)
{
    (*g_engfuncs.pfnDeltaSetField)(pFields, fieldname);
}

void pfnDeltaUnsetField(struct delta_s *pFields, const char *fieldname)
{
    (*g_engfuncs.pfnDeltaUnsetField)(pFields, fieldname);
}

void pfnDeltaAddEncoder(char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to))
{
    (*g_engfuncs.pfnDeltaAddEncoder)(name, conditionalencode);
}

int pfnGetCurrentPlayer(void)
{
    return (*g_engfuncs.pfnGetCurrentPlayer)();
}

int pfnCanSkipPlayer(const edict_t *player)
{
    return (*g_engfuncs.pfnCanSkipPlayer)(player);
}

int pfnDeltaFindField(struct delta_s *pFields, const char *fieldname)
{
    return (*g_engfuncs.pfnDeltaFindField)(pFields, fieldname);
}

void pfnDeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
    (*g_engfuncs.pfnDeltaSetFieldByIndex)(pFields, fieldNumber);
}

void pfnDeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
    (*g_engfuncs.pfnDeltaUnsetFieldByIndex)(pFields, fieldNumber);
}

void pfnSetGroupMask(int mask, int op)
{
    (*g_engfuncs.pfnSetGroupMask)(mask, op);
}

int pfnCreateInstancedBaseline(int classname, struct entity_state_s *baseline)
{
    return (*g_engfuncs.pfnCreateInstancedBaseline)(classname, baseline);
}

void pfnCvar_DirectSet(struct cvar_s *var, char *value)
{
    (*g_engfuncs.pfnCvar_DirectSet)(var, value);
}

void pfnForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename)
{
    (*g_engfuncs.pfnForceUnmodified)(type, mins, maxs, filename);
}

void pfnGetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss)
{
    (*g_engfuncs.pfnGetPlayerStats)(pClient, ping, packet_loss);
}


// new stuff for SDK 2.3 and beyond

void pfnAddServerCommand( char *cmd_name, void (*function) (void) )
{
    (*g_engfuncs.pfnAddServerCommand)( cmd_name, function );
}

qboolean pfnVoice_GetClientListening(int iReceiver, int iSender)
{
    return (*g_engfuncs.pfnVoice_GetClientListening)(iReceiver, iSender);
}

qboolean pfnVoice_SetClientListening(int iReceiver, int iSender, qboolean bListen)
{
    return (*g_engfuncs.pfnVoice_SetClientListening)( iReceiver, iSender, bListen );
}

const char *pfnGetPlayerAuthId( edict_t *e )
{
    return (*g_engfuncs.pfnGetPlayerAuthId)(e);
}

sequenceEntry_s* pfnSequenceGet( const char* fileName, const char* entryName )
{
    return (*g_engfuncs.pfnSequenceGet)( fileName, entryName );
}

sequenceEntry_s* pfnSequencePickSentence( const char* groupName, int pickMethod, int *picked )
{
    return (*g_engfuncs.pfnSequencePickSentence)( groupName, pickMethod, picked );
}

int pfnGetFileSize( char *filename )
{
    return (*g_engfuncs.pfnGetFileSize)( filename );
}

unsigned int pfnGetApproxWavePlayLen( const char *filepath )
{
    return (*g_engfuncs.pfnGetApproxWavePlayLen)( filepath );
}

int pfnIsCareerMatch( void )
{
    return (*g_engfuncs.pfnIsCareerMatch)();
}

int pfnGetLocalizedStringLength( const char *label )
{
    return (*g_engfuncs.pfnGetLocalizedStringLength)( label );
}

void pfnRegisterTutorMessageShown( int mid )
{
    (*g_engfuncs.pfnRegisterTutorMessageShown)( mid );
}

int pfnGetTimesTutorMessageShown( int mid )
{
    return (*g_engfuncs.pfnGetTimesTutorMessageShown)( mid );
}

void pfnProcessTutorMessageDecayBuffer( int *buffer, int bufferLength )
{
    (*g_engfuncs.pfnProcessTutorMessageDecayBuffer)( buffer, bufferLength );
}

void pfnConstructTutorMessageDecayBuffer( int *buffer, int bufferLength )
{
    (*g_engfuncs.pfnConstructTutorMessageDecayBuffer)( buffer, bufferLength );
}

void pfnResetTutorMessageDecayData( void )
{
    (*g_engfuncs.pfnResetTutorMessageDecayData)();
}
