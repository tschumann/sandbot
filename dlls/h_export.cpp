//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// h_export.cpp
//

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "engine.h"
#include "dll.h"

#include <io.h>

#ifndef __linux__

HINSTANCE h_Library = NULL;

#else

void *h_Library = NULL;

#endif

enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;
char g_argv[1024];

static FILE *fp;


extern DLL_FUNCTIONS other_gFunctionTable;

extern int mod_id;


#ifndef __linux__

// Required DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
   if (fdwReason == DLL_PROCESS_DETACH)
   {
      if (h_Library)
         FreeLibrary(h_Library);
   }

   return TRUE;
}

#endif

#ifndef __linux__
#ifdef __BORLANDC__
extern "C" DLLEXPORT void EXPORT GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals)
#else
void DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
#endif
#else
extern "C" void GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
#endif
{
   int pos;
   unsigned char *filebuf;
   int filesize;
   FILE *filep;
   char game_dir[256];
   char mod_name[32];

   // get the engine functions from the engine...

   // test if we're running Steam or not and shorten the engine functions table if we aren't
   if ((access("valve/steam.inf", 0) != -1) || (access("FileSystem_Steam.dll", 0) != -1))
	   memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t)); // steam
   else
      memcpy(&g_engfuncs, pengfuncsFromEngine, 144 * sizeof(uint32)); // non-steam

   gpGlobals = pGlobals;

   // find the directory name of the currently running MOD...
   GET_GAME_DIR (game_dir);

   pos = strlen(game_dir) - 1;

   // skip any eventual trailing slashes
   while ((pos) && (game_dir[pos] == '/'))
      pos--;

   // scan backwards till first directory separator...
   while ((pos) && (game_dir[pos] != '/'))
      pos--;

   if (pos > 0)
      pos++; // found one ? skip it

   strcpy(mod_name, &game_dir[pos]);

   if (strcmpi(mod_name, "valve") == 0)
   {
      mod_id = VALVE_DLL;

#ifndef __linux__
      // test if the game DLL file is NOT available outside of the Steam/PAK cache
      if (access("valve/dlls/hl.dll", 0) == -1)
      {
         filebuf = LOAD_FILE_FOR_ME ("dlls/hl.dll", &filesize);
         filep = fopen ("valve/dlls/hl.dll", "wb");
         if (filep != NULL)
         {
            fwrite (filebuf, 1, filesize, filep); // if in cache, then extract it
            fclose (filep);
         }
      }
      h_Library = LoadLibrary("valve/dlls/hl.dll"); // and load the library
#else
      h_Library = dlopen("valve/dlls/hl_i386.so", RTLD_NOW); // and load the library
#endif
   }
   else if (strcmpi(mod_name, "tfc") == 0)
   {
      mod_id = TFC_DLL;
#ifndef __linux__
      // test if the game DLL file is NOT available outside of the Steam/PAK cache
      if (access("tfc/dlls/tfc.dll", 0) == -1)
      {
         filebuf = LOAD_FILE_FOR_ME ("dlls/tfc.dll", &filesize);
         filep = fopen ("tfc/dlls/tfc.dll", "wb");
         if (filep != NULL)
         {
            fwrite (filebuf, 1, filesize, filep); // if in cache, then extract it
            fclose (filep);
         }
      }
      h_Library = LoadLibrary("tfc/dlls/tfc.dll");
#else
      h_Library = dlopen("tfc/dlls/tfc_i386.so", RTLD_NOW);
#endif
   }
   else if (strcmpi(mod_name, "cstrike") == 0)
   {
      mod_id = CSTRIKE_DLL;
#ifndef __linux__
      // test if the game DLL file is NOT available outside of the Steam/PAK cache
      if (access("cstrike/dlls/mp.dll", 0) == -1)
      {
         filebuf = LOAD_FILE_FOR_ME ("dlls/mp.dll", &filesize);
         filep = fopen ("cstrike/dlls/mp.dll", "wb");
         if (filep != NULL)
         {
            fwrite (filebuf, 1, filesize, filep); // if in cache, then extract it
            fclose (filep);
         }
      }
      h_Library = LoadLibrary("cstrike/dlls/mp.dll");
#else
      h_Library = dlopen("cstrike/dlls/cs_i386.so", RTLD_NOW);
#endif
   }
   else if (strcmpi(mod_name, "gearbox") == 0)
   {
      mod_id = GEARBOX_DLL;
#ifndef __linux__
      // test if the game DLL file is NOT available outside of the Steam/PAK cache
      if (access("gearbox/dlls/opfor.dll", 0) == -1)
      {
         filebuf = LOAD_FILE_FOR_ME ("dlls/opfor.dll", &filesize);
         filep = fopen ("gearbox/dlls/opfor.dll", "wb");
         if (filep != NULL)
         {
            fwrite (filebuf, 1, filesize, filep); // if in cache, then extract it
            fclose (filep);
         }
      }
      h_Library = LoadLibrary("gearbox/dlls/opfor.dll");
#else
      h_Library = dlopen("gearbox/dlls/opfor_i386.so", RTLD_NOW);
#endif
   }
   else if (strcmpi(mod_name, "frontline") == 0)
   {
      mod_id = FRONTLINE_DLL;
#ifndef __linux__
      // test if the game DLL file is NOT available outside of the Steam/PAK cache
      if (access("frontline/dlls/frontline.dll", 0) == -1)
      {
         filebuf = LOAD_FILE_FOR_ME ("dlls/frontline.dll", &filesize);
         filep = fopen ("frontline/dlls/frontline.dll", "wb");
         if (filep != NULL)
         {
            fwrite (filebuf, 1, filesize, filep); // if in cache, then extract it
            fclose (filep);
         }
      }
      h_Library = LoadLibrary("frontline/dlls/frontline.dll");
#else
      h_Library = dlopen("frontline/dlls/front_i386.so", RTLD_NOW);
#endif
   }

   if (h_Library == NULL)
   {
      // Directory error or Unsupported MOD!

		ALERT( at_error, "HPB_bot - MOD dll not found (or unsupported MOD)!" );
   }


   // and now we need to pass engine functions table to the game DLL (in fact it's our own
   // functions we are passing here, but the game DLL won't notice)...

   pengfuncsFromEngine->pfnPrecacheModel = pfnPrecacheModel;
   pengfuncsFromEngine->pfnPrecacheSound = pfnPrecacheSound;
   pengfuncsFromEngine->pfnSetModel = pfnSetModel;
   pengfuncsFromEngine->pfnModelIndex = pfnModelIndex;
   pengfuncsFromEngine->pfnModelFrames = pfnModelFrames;
   pengfuncsFromEngine->pfnSetSize = pfnSetSize;
   pengfuncsFromEngine->pfnChangeLevel = pfnChangeLevel;
   pengfuncsFromEngine->pfnGetSpawnParms = pfnGetSpawnParms;
   pengfuncsFromEngine->pfnSaveSpawnParms = pfnSaveSpawnParms;
   pengfuncsFromEngine->pfnVecToYaw = pfnVecToYaw;
   pengfuncsFromEngine->pfnVecToAngles = pfnVecToAngles;
   pengfuncsFromEngine->pfnMoveToOrigin = pfnMoveToOrigin;
   pengfuncsFromEngine->pfnChangeYaw = pfnChangeYaw;
   pengfuncsFromEngine->pfnChangePitch = pfnChangePitch;
   pengfuncsFromEngine->pfnFindEntityByString = pfnFindEntityByString;
   pengfuncsFromEngine->pfnGetEntityIllum = pfnGetEntityIllum;
   pengfuncsFromEngine->pfnFindEntityInSphere = pfnFindEntityInSphere;
   pengfuncsFromEngine->pfnFindClientInPVS = pfnFindClientInPVS;
   pengfuncsFromEngine->pfnEntitiesInPVS = pfnEntitiesInPVS;
   pengfuncsFromEngine->pfnMakeVectors = pfnMakeVectors;
   pengfuncsFromEngine->pfnAngleVectors = pfnAngleVectors;
   pengfuncsFromEngine->pfnCreateEntity = pfnCreateEntity;
   pengfuncsFromEngine->pfnRemoveEntity = pfnRemoveEntity;
   pengfuncsFromEngine->pfnCreateNamedEntity = pfnCreateNamedEntity;
   pengfuncsFromEngine->pfnMakeStatic = pfnMakeStatic;
   pengfuncsFromEngine->pfnEntIsOnFloor = pfnEntIsOnFloor;
   pengfuncsFromEngine->pfnDropToFloor = pfnDropToFloor;
   pengfuncsFromEngine->pfnWalkMove = pfnWalkMove;
   pengfuncsFromEngine->pfnSetOrigin = pfnSetOrigin;
   pengfuncsFromEngine->pfnEmitSound = pfnEmitSound;
   pengfuncsFromEngine->pfnEmitAmbientSound = pfnEmitAmbientSound;
   pengfuncsFromEngine->pfnTraceLine = pfnTraceLine;
   pengfuncsFromEngine->pfnTraceToss = pfnTraceToss;
   pengfuncsFromEngine->pfnTraceMonsterHull = pfnTraceMonsterHull;
   pengfuncsFromEngine->pfnTraceHull = pfnTraceHull;
   pengfuncsFromEngine->pfnTraceModel = pfnTraceModel;
   pengfuncsFromEngine->pfnTraceTexture = pfnTraceTexture;
   pengfuncsFromEngine->pfnTraceSphere = pfnTraceSphere;
   pengfuncsFromEngine->pfnGetAimVector = pfnGetAimVector;
   pengfuncsFromEngine->pfnServerCommand = pfnServerCommand;
   pengfuncsFromEngine->pfnServerExecute = pfnServerExecute;
   pengfuncsFromEngine->pfnClientCommand = pfnClientCommand;
   pengfuncsFromEngine->pfnParticleEffect = pfnParticleEffect;
   pengfuncsFromEngine->pfnLightStyle = pfnLightStyle;
   pengfuncsFromEngine->pfnDecalIndex = pfnDecalIndex;
   pengfuncsFromEngine->pfnPointContents = pfnPointContents;
   pengfuncsFromEngine->pfnMessageBegin = pfnMessageBegin;
   pengfuncsFromEngine->pfnMessageEnd = pfnMessageEnd;
   pengfuncsFromEngine->pfnWriteByte = pfnWriteByte;
   pengfuncsFromEngine->pfnWriteChar = pfnWriteChar;
   pengfuncsFromEngine->pfnWriteShort = pfnWriteShort;
   pengfuncsFromEngine->pfnWriteLong = pfnWriteLong;
   pengfuncsFromEngine->pfnWriteAngle = pfnWriteAngle;
   pengfuncsFromEngine->pfnWriteCoord = pfnWriteCoord;
   pengfuncsFromEngine->pfnWriteString = pfnWriteString;
   pengfuncsFromEngine->pfnWriteEntity = pfnWriteEntity;
   pengfuncsFromEngine->pfnCVarRegister = pfnCVarRegister;
   pengfuncsFromEngine->pfnCVarGetFloat = pfnCVarGetFloat;
   pengfuncsFromEngine->pfnCVarGetString = pfnCVarGetString;
   pengfuncsFromEngine->pfnCVarSetFloat = pfnCVarSetFloat;
   pengfuncsFromEngine->pfnCVarSetString = pfnCVarSetString;
   pengfuncsFromEngine->pfnPvAllocEntPrivateData = pfnPvAllocEntPrivateData;
   pengfuncsFromEngine->pfnPvEntPrivateData = pfnPvEntPrivateData;
   pengfuncsFromEngine->pfnFreeEntPrivateData = pfnFreeEntPrivateData;
   pengfuncsFromEngine->pfnSzFromIndex = pfnSzFromIndex;
   pengfuncsFromEngine->pfnAllocString = pfnAllocString;
   pengfuncsFromEngine->pfnGetVarsOfEnt = pfnGetVarsOfEnt;
   pengfuncsFromEngine->pfnPEntityOfEntOffset = pfnPEntityOfEntOffset;
   pengfuncsFromEngine->pfnEntOffsetOfPEntity = pfnEntOffsetOfPEntity;
   pengfuncsFromEngine->pfnIndexOfEdict = pfnIndexOfEdict;
   pengfuncsFromEngine->pfnPEntityOfEntIndex = pfnPEntityOfEntIndex;
   pengfuncsFromEngine->pfnFindEntityByVars = pfnFindEntityByVars;
   pengfuncsFromEngine->pfnGetModelPtr = pfnGetModelPtr;
   pengfuncsFromEngine->pfnRegUserMsg = pfnRegUserMsg;
   pengfuncsFromEngine->pfnAnimationAutomove = pfnAnimationAutomove;
   pengfuncsFromEngine->pfnGetBonePosition = pfnGetBonePosition;
   pengfuncsFromEngine->pfnFunctionFromName = pfnFunctionFromName;
   pengfuncsFromEngine->pfnNameForFunction = pfnNameForFunction;
   pengfuncsFromEngine->pfnClientPrintf = pfnClientPrintf;
   pengfuncsFromEngine->pfnServerPrint = pfnServerPrint;
   pengfuncsFromEngine->pfnCmd_Args = Cmd_Args;
   pengfuncsFromEngine->pfnCmd_Argv = Cmd_Argv;
   pengfuncsFromEngine->pfnCmd_Argc = Cmd_Argc;
   pengfuncsFromEngine->pfnGetAttachment = pfnGetAttachment;
   pengfuncsFromEngine->pfnCRC32_Init = pfnCRC32_Init;
   pengfuncsFromEngine->pfnCRC32_ProcessBuffer = pfnCRC32_ProcessBuffer;
   pengfuncsFromEngine->pfnCRC32_ProcessByte = pfnCRC32_ProcessByte;
   pengfuncsFromEngine->pfnCRC32_Final = pfnCRC32_Final;
   pengfuncsFromEngine->pfnRandomLong = pfnRandomLong;
   pengfuncsFromEngine->pfnRandomFloat = pfnRandomFloat;
   pengfuncsFromEngine->pfnSetView = pfnSetView;
   pengfuncsFromEngine->pfnTime = pfnTime;
   pengfuncsFromEngine->pfnCrosshairAngle = pfnCrosshairAngle;
   pengfuncsFromEngine->pfnLoadFileForMe = pfnLoadFileForMe;
   pengfuncsFromEngine->pfnFreeFile = pfnFreeFile;
   pengfuncsFromEngine->pfnEndSection = pfnEndSection;
   pengfuncsFromEngine->pfnCompareFileTime = pfnCompareFileTime;
   pengfuncsFromEngine->pfnGetGameDir = pfnGetGameDir;
   pengfuncsFromEngine->pfnCvar_RegisterVariable = pfnCvar_RegisterVariable;
   pengfuncsFromEngine->pfnFadeClientVolume = pfnFadeClientVolume;
   pengfuncsFromEngine->pfnSetClientMaxspeed = pfnSetClientMaxspeed;
   pengfuncsFromEngine->pfnCreateFakeClient = pfnCreateFakeClient;
   pengfuncsFromEngine->pfnRunPlayerMove = pfnRunPlayerMove;
   pengfuncsFromEngine->pfnNumberOfEntities = pfnNumberOfEntities;
   pengfuncsFromEngine->pfnGetInfoKeyBuffer = pfnGetInfoKeyBuffer;
   pengfuncsFromEngine->pfnInfoKeyValue = pfnInfoKeyValue;
   pengfuncsFromEngine->pfnSetKeyValue = pfnSetKeyValue;
   pengfuncsFromEngine->pfnSetClientKeyValue = pfnSetClientKeyValue;
   pengfuncsFromEngine->pfnIsMapValid = pfnIsMapValid;
   pengfuncsFromEngine->pfnStaticDecal = pfnStaticDecal;
   pengfuncsFromEngine->pfnPrecacheGeneric = pfnPrecacheGeneric;
   pengfuncsFromEngine->pfnGetPlayerUserId = pfnGetPlayerUserId;
   pengfuncsFromEngine->pfnBuildSoundMsg = pfnBuildSoundMsg;
   pengfuncsFromEngine->pfnIsDedicatedServer = pfnIsDedicatedServer;
   pengfuncsFromEngine->pfnCVarGetPointer = pfnCVarGetPointer;
   pengfuncsFromEngine->pfnGetPlayerWONId = pfnGetPlayerWONId;
   pengfuncsFromEngine->pfnInfo_RemoveKey = pfnInfo_RemoveKey;
   pengfuncsFromEngine->pfnGetPhysicsKeyValue = pfnGetPhysicsKeyValue;
   pengfuncsFromEngine->pfnSetPhysicsKeyValue = pfnSetPhysicsKeyValue;
   pengfuncsFromEngine->pfnGetPhysicsInfoString = pfnGetPhysicsInfoString;
   pengfuncsFromEngine->pfnPrecacheEvent = pfnPrecacheEvent;
   pengfuncsFromEngine->pfnPlaybackEvent = pfnPlaybackEvent;
   pengfuncsFromEngine->pfnSetFatPVS = pfnSetFatPVS;
   pengfuncsFromEngine->pfnSetFatPAS = pfnSetFatPAS;
   pengfuncsFromEngine->pfnCheckVisibility = pfnCheckVisibility;
   pengfuncsFromEngine->pfnDeltaSetField = pfnDeltaSetField;
   pengfuncsFromEngine->pfnDeltaUnsetField = pfnDeltaUnsetField;
   pengfuncsFromEngine->pfnDeltaAddEncoder = pfnDeltaAddEncoder;
   pengfuncsFromEngine->pfnGetCurrentPlayer = pfnGetCurrentPlayer;
   pengfuncsFromEngine->pfnCanSkipPlayer = pfnCanSkipPlayer;
   pengfuncsFromEngine->pfnDeltaFindField = pfnDeltaFindField;
   pengfuncsFromEngine->pfnDeltaSetFieldByIndex = pfnDeltaSetFieldByIndex;
   pengfuncsFromEngine->pfnDeltaUnsetFieldByIndex = pfnDeltaUnsetFieldByIndex;
   pengfuncsFromEngine->pfnSetGroupMask = pfnSetGroupMask;
   pengfuncsFromEngine->pfnCreateInstancedBaseline = pfnCreateInstancedBaseline;
   pengfuncsFromEngine->pfnCvar_DirectSet = pfnCvar_DirectSet;
   pengfuncsFromEngine->pfnForceUnmodified = pfnForceUnmodified;
   pengfuncsFromEngine->pfnGetPlayerStats = pfnGetPlayerStats;
   pengfuncsFromEngine->pfnAddServerCommand = pfnAddServerCommand;
   pengfuncsFromEngine->pfnVoice_GetClientListening = pfnVoice_GetClientListening;
   pengfuncsFromEngine->pfnVoice_SetClientListening = pfnVoice_SetClientListening;
   pengfuncsFromEngine->pfnGetPlayerAuthId = pfnGetPlayerAuthId; 
   pengfuncsFromEngine->pfnSequenceGet = pfnSequenceGet;
   pengfuncsFromEngine->pfnSequencePickSentence = pfnSequencePickSentence;
   pengfuncsFromEngine->pfnGetFileSize = pfnGetFileSize;
   pengfuncsFromEngine->pfnGetApproxWavePlayLen = pfnGetApproxWavePlayLen;
   pengfuncsFromEngine->pfnIsCareerMatch = pfnIsCareerMatch;
   pengfuncsFromEngine->pfnGetLocalizedStringLength = pfnGetLocalizedStringLength;
   pengfuncsFromEngine->pfnRegisterTutorMessageShown = pfnRegisterTutorMessageShown;
   pengfuncsFromEngine->pfnGetTimesTutorMessageShown = pfnGetTimesTutorMessageShown;
   pengfuncsFromEngine->pfnProcessTutorMessageDecayBuffer = pfnProcessTutorMessageDecayBuffer;
   pengfuncsFromEngine->pfnConstructTutorMessageDecayBuffer = pfnConstructTutorMessageDecayBuffer;
   pengfuncsFromEngine->pfnResetTutorMessageDecayData = pfnResetTutorMessageDecayData;

   // give the engine functions to the other DLL...
   (*(GIVEFNPTRSTODLL) GetProcAddress (h_Library, "GiveFnptrsToDll")) (pengfuncsFromEngine, pGlobals);

   return; // finished, interfacing from gamedll to engine complete
}


#ifdef __BORLANDC__
int EXPORT GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
#else
extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
#endif
{
   // check if engine's pointer is valid and version is correct...
   if ((pFunctionTable == NULL) || (interfaceVersion != INTERFACE_VERSION))
      return (FALSE);

   // pass gamedll functions table to engine (in fact it's our own functions we are passing
   // here, but the engine won't notice)...

   pFunctionTable->pfnGameInit = GameDLLInit;
   pFunctionTable->pfnSpawn = DispatchSpawn;
   pFunctionTable->pfnThink = DispatchThink;
   pFunctionTable->pfnUse = DispatchUse;
   pFunctionTable->pfnTouch = DispatchTouch;
   pFunctionTable->pfnBlocked = DispatchBlocked;
   pFunctionTable->pfnKeyValue = DispatchKeyValue;
   pFunctionTable->pfnSave = DispatchSave;
   pFunctionTable->pfnRestore = DispatchRestore;
   pFunctionTable->pfnSetAbsBox = DispatchObjectCollisionBox;
   pFunctionTable->pfnSaveWriteFields = SaveWriteFields;
   pFunctionTable->pfnSaveReadFields = SaveReadFields;
   pFunctionTable->pfnSaveGlobalState = SaveGlobalState;
   pFunctionTable->pfnRestoreGlobalState = RestoreGlobalState;
   pFunctionTable->pfnResetGlobalState = ResetGlobalState;
   pFunctionTable->pfnClientConnect = ClientConnect;
   pFunctionTable->pfnClientDisconnect = ClientDisconnect;
   pFunctionTable->pfnClientKill = ClientKill;
   pFunctionTable->pfnClientPutInServer = ClientPutInServer;
   pFunctionTable->pfnClientCommand = ClientCommand;
   pFunctionTable->pfnClientUserInfoChanged = ClientUserInfoChanged;
   pFunctionTable->pfnServerActivate = ServerActivate;
   pFunctionTable->pfnServerDeactivate = ServerDeactivate;
   pFunctionTable->pfnPlayerPreThink = PlayerPreThink;
   pFunctionTable->pfnPlayerPostThink = PlayerPostThink;
   pFunctionTable->pfnStartFrame = StartFrame;
   pFunctionTable->pfnParmsNewLevel = ParmsNewLevel;
   pFunctionTable->pfnParmsChangeLevel = ParmsChangeLevel;
   pFunctionTable->pfnGetGameDescription = GetGameDescription;
   pFunctionTable->pfnPlayerCustomization = PlayerCustomization;
   pFunctionTable->pfnSpectatorConnect = SpectatorConnect;
   pFunctionTable->pfnSpectatorDisconnect = SpectatorDisconnect;
   pFunctionTable->pfnSpectatorThink = SpectatorThink;
   pFunctionTable->pfnSys_Error = Sys_Error;
   pFunctionTable->pfnPM_Move = PM_Move;
   pFunctionTable->pfnPM_Init = PM_Init;
   pFunctionTable->pfnPM_FindTextureType = PM_FindTextureType;
   pFunctionTable->pfnSetupVisibility = SetupVisibility;
   pFunctionTable->pfnUpdateClientData = UpdateClientData;
   pFunctionTable->pfnAddToFullPack = AddToFullPack;
   pFunctionTable->pfnCreateBaseline = CreateBaseline;
   pFunctionTable->pfnRegisterEncoders = RegisterEncoders;
   pFunctionTable->pfnGetWeaponData = GetWeaponData;
   pFunctionTable->pfnCmdStart = CmdStart;
   pFunctionTable->pfnCmdEnd = CmdEnd;
   pFunctionTable->pfnConnectionlessPacket = ConnectionlessPacket;
   pFunctionTable->pfnGetHullBounds = GetHullBounds;
   pFunctionTable->pfnCreateInstancedBaselines = CreateInstancedBaselines;
   pFunctionTable->pfnInconsistentFile = InconsistentFile;
   pFunctionTable->pfnAllowLagCompensation = AllowLagCompensation;

   // and now we need to get the REAL gamedll functions, for our own use (call GetEntityAPI()
   // in the game DLL and have it fill in our functions table)...

   // was the call NOT successful ?
   if (!(*(GETENTITYAPI) GetProcAddress (h_Library, "GetEntityAPI")) (&other_gFunctionTable, INTERFACE_VERSION))
      return (FALSE);  // error initializing function table!!!

   return (TRUE); // finished, interfacing from engine to gamedll complete
}


#ifdef __BORLANDC__
int EXPORT GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
#else
extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
#endif
{
   static GETNEWDLLFUNCTIONS other_GetNewDLLFunctions = NULL;
   static bool missing = FALSE;

   // if the new DLL functions interface has been formerly reported as missing, give up
   if (missing)
      return (FALSE);

   // do we NOT know if the new DLL functions interface is provided ? if so, look for its address
   if (other_GetNewDLLFunctions == NULL)
      other_GetNewDLLFunctions = (GETNEWDLLFUNCTIONS) GetProcAddress (h_Library, "GetNewDLLFunctions");

   // have we NOT found it ?
   if (other_GetNewDLLFunctions == NULL)
   {
      missing = TRUE; // then mark it as missing, no use to look for it again in the future
      return (FALSE); // and give up
   }

   // else call the function that provides the new DLL functions interface on request
   return (!(*other_GetNewDLLFunctions) (pFunctionTable, interfaceVersion));
}


#ifdef __BORLANDC__
int EXPORT Server_GetBlendingInterface( int version, struct sv_blending_interface_s **ppinterface, struct engine_studio_api_s *pstudio, float (*rotationmatrix)[3][4], float (*bonetransform)[MAXSTUDIOBONES][3][4] )
#else
extern "C" EXPORT int Server_GetBlendingInterface( int version, struct sv_blending_interface_s **ppinterface, struct engine_studio_api_s *pstudio, float (*rotationmatrix)[3][4], float (*bonetransform)[MAXSTUDIOBONES][3][4] )
#endif
{
   static SERVER_GETBLENDINGINTERFACE other_Server_GetBlendingInterface = NULL;
   static bool missing = FALSE;

   // if the blending interface has been formerly reported as missing, give up
   if (missing)
      return (FALSE);

   // do we NOT know if the blending interface is provided ? if so, look for its address
   if (other_Server_GetBlendingInterface == NULL)
      other_Server_GetBlendingInterface = (SERVER_GETBLENDINGINTERFACE) GetProcAddress (h_Library, "Server_GetBlendingInterface");

   // have we NOT found it ?
   if (!other_Server_GetBlendingInterface)
   {
      missing = TRUE; // then mark it as missing, no use to look for it again in the future
      return (FALSE); // and give up
   }

   // else call the function that provides the blending interface on request
   return ((other_Server_GetBlendingInterface) (version, ppinterface, pstudio, rotationmatrix, bonetransform));
}
