//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// h_export.cpp
//

#include "extdll.h"
#include "h_export.h"
#include "meta_api.h"
#include "cbase.h"

#include "bot.h"
#include "engine.h"
#include "dll.h"
#include "linkfunc.h"
#include "h_export.h"

#ifndef __linux__
#include <io.h>
#else
#include <sys/io.h>
#endif

#ifndef __linux__
HINSTANCE h_Library = nullptr;
#else
void* h_Library = nullptr;
#endif

const char *Cmd_Args( void );
const char *Cmd_Argv( int argc );
int Cmd_Argc( void );

enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;

char g_szLibraryPath[64];

extern DLL_FUNCTIONS other_gFunctionTable;

extern int mod_id;

extern bot_player_t *pBotData;


#ifndef __linux__
// required DLL entry point
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	if( fdwReason == DLL_PROCESS_DETACH )
	{
		if( h_Library )
		{
			FreeLibrary( h_Library );
		}
	}

	return TRUE;
}
#endif

extern "C" EXPORT int GetEngineFunctions( enginefuncs_t * pengfuncsFromEngine, int *interfaceVersion )
{
	if( g_bIsMMPlugin )
		memset( pengfuncsFromEngine, 0, sizeof( enginefuncs_t ) );

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
	pengfuncsFromEngine->ProcessTutorMessageDecayBuffer = pfnProcessTutorMessageDecayBuffer;
	pengfuncsFromEngine->ConstructTutorMessageDecayBuffer = pfnConstructTutorMessageDecayBuffer;
	pengfuncsFromEngine->ResetTutorMessageDecayData = pfnResetTutorMessageDecayData;

	// no overrides for the last few because there's no need

	return TRUE;
}

#ifndef __linux__
void WINAPI GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
#else
extern "C" void GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
#endif
{
	// get the engine functions from the engine...
	memcpy( &g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t) );
	// get the globals from the engine
	gpGlobals = pGlobals;

	// get the directory name of the currently running game or mod
	// this now returns just the mod directory's name: http://metamod.org/engine_notes.html#GetGameDir
	char szGameDir[256];
	char *szLibraryPath = "";
	GET_GAME_DIR(szGameDir);

	// be great if libc gave this out of the box
	for( unsigned int i = 0; szGameDir[i]; i++ ) {
		szGameDir[i] = tolower(szGameDir[i]);
	}

	if( !strcmp( szGameDir, "valve" ) )
	{
		mod_id = VALVE_DLL;

#ifndef __linux__
		szLibraryPath = "valve/dlls/hl.dll";
#else
		szLibraryPath = "valve/dlls/hl.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "bshift" ) )
	{
		mod_id = BSHIFT_DLL;

#ifndef __linux__
		szLibraryPath = "bshift/dlls/hl.dll";
#else
		szLibraryPath = "bshift/dlls/bshift.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "gearbox" ) )
	{
		mod_id = GEARBOX_DLL;

#ifndef __linux__
		szLibraryPath = "gearbox/dlls/opfor.dll";
#else
		szLibraryPath = "gearbox/dlls/opfor.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "decay" ) )
	{
		mod_id = DECAY_DLL;

#ifndef __linux__
		szLibraryPath = "decay/dlls/decay.dll";
#else
		szLibraryPath = "decay/dlls/decay_i386.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "cstrike" ) )
	{
		mod_id = CSTRIKE_DLL;

#ifndef __linux__
		szLibraryPath = "cstrike/dlls/mp.dll";
#else
		szLibraryPath = "cstrike/dlls/cs.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "czero" ) )
	{
		mod_id = CZERO_DLL;

#ifndef __linux__
		szLibraryPath = "czero/dlls/mp.dll";
#else
		szLibraryPath = "czero/dlls/cs.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "czeror" ) )
	{
		mod_id = CZEROR_DLL;

#ifndef __linux__
		szLibraryPath = "czeror/dlls/cz.dll";
#else
		szLibraryPath = "czeror/dlls/cz.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "dod" ) )
	{
		mod_id = DOD_DLL;

#ifndef __linux__
		szLibraryPath = "dod/dlls/dod.dll";
#else
		szLibraryPath = "dod/dlls/dod.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "tfc" ) )
	{
		mod_id = TFC_DLL;

#ifndef __linux__
		szLibraryPath = "tfc/dlls/tfc.dll";
#else
		szLibraryPath = "tfc/dlls/tfc.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "rewolf" ) )
	{
		mod_id = REWOLF_DLL;

#ifndef __linux__
		szLibraryPath = "rewolf/dlls/gunman.dll";
#else
		szLibraryPath = nullptr;
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = nullptr;
#endif
		}
	}
	else if( !strcmp( szGameDir, "hunger" ) )
	{
		mod_id = HUNGER_DLL;

#ifndef __linux__
		szLibraryPath = "hunger/dlls/einar.dll";
#else
		szLibraryPath = nullptr;
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = nullptr;
#endif
		}
	}
	else if( !strcmp( szGameDir , "ns") )
	{
		mod_id = NS_DLL;

#ifndef __linux__
		szLibraryPath = "ns/dlls/ns.dll";
#else
		szLibraryPath = "ns/dlls/ns_i386.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}
	else if( !strcmp( szGameDir, "ship" ) )
	{
		mod_id = SHIP_DLL;

#ifndef __linux__
		szLibraryPath = "ship/dlls/ship.dll";
#else
		szLibraryPath = "ship/dlls/ship_i386.so";
#endif

		if( !g_bIsMMPlugin )
		{
#ifndef __linux__
			h_Library = LoadLibrary( szLibraryPath );
#else
			h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
		}
	}

	strncpy( g_szLibraryPath, szLibraryPath, strlen( szLibraryPath ) );

	if( !g_bIsMMPlugin && h_Library == nullptr )
	{
		ALERT( at_error, "Library not found or not supported!\n" );

		return;
	}

	extern bot_player_t g_valveBots[];
	extern bot_player_t g_gearboxBots[];
	extern bot_player_t g_dodBots[];
	extern bot_player_t g_gunmanBots[];
	extern bot_player_t g_nsBots[];
	extern bot_player_t g_hungerBots[];
	extern bot_player_t g_shipBots[];

	if( mod_id == VALVE_DLL || mod_id == TFC_DLL )
	{
		pBotData = g_valveBots;
	}
	else if( mod_id == GEARBOX_DLL )
	{
		pBotData = g_gearboxBots;
	}
	else if( mod_id == DOD_DLL )
	{
		pBotData = g_dodBots;
	}
	else if( mod_id == REWOLF_DLL )
	{
		pBotData = g_gunmanBots;
	}
	else if( mod_id == NS_DLL )
	{
		pBotData = g_nsBots;
	}
	else if( mod_id == HUNGER_DLL )
	{
		pBotData = g_hungerBots;
	}
	else if( mod_id == SHIP_DLL )
	{
		pBotData = g_shipBots;
	}

	LoadExtraExports();

	if( g_bIsMMPlugin )
	{
		return;
	}

	GetEngineFunctions( pengfuncsFromEngine, nullptr );

	// give the engine functions to the other DLL...
	(*(GIVEFNPTRSTODLL)GetProcAddress( h_Library, "GiveFnptrsToDll" ))(pengfuncsFromEngine, pGlobals);

	// finished, interfacing from gamedll to engine complete
	return;
}

gamedll_funcs_t gGameDLLFunc;

extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
   // check if engine's pointer is valid and version is correct...
   if ((pFunctionTable == nullptr) || (interfaceVersion != INTERFACE_VERSION))
      return FALSE;

   memset( pFunctionTable, 0, sizeof( DLL_FUNCTIONS ) );

   if( !g_bIsMMPlugin )
   {
	   // pass other DLLs engine callbacks to function table...
	   if( !(*(GETENTITYAPI)GetProcAddress( h_Library, "GetEntityAPI" )) (&other_gFunctionTable, INTERFACE_VERSION) )
		   return FALSE;  // error initializing function table!!!

	   gGameDLLFunc.dllapi_table = &other_gFunctionTable;
	   gpGamedllFuncs = &gGameDLLFunc;

	   memcpy( pFunctionTable, &other_gFunctionTable, sizeof( DLL_FUNCTIONS ) );
   }

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

   return TRUE; // finished, interfacing from engine to gamedll complete
}

extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
{
   if( !g_bIsMMPlugin )
   {
	   static GETNEWDLLFUNCTIONS pGetNewDLLFunctions = nullptr;
	   static bool bIsMissing = false;

	   // if the game .dll has no GetNewDLLFunctions exported
	   if( bIsMissing )
	   {
		   return FALSE;
	   }

	   // do we NOT know if the new DLL functions interface is provided ? if so, look for its address
	   if( pGetNewDLLFunctions == nullptr )
		   pGetNewDLLFunctions = (GETNEWDLLFUNCTIONS)GetProcAddress( h_Library, "GetNewDLLFunctions" );

	   // if it wasn't found
	   if( pGetNewDLLFunctions == nullptr )
	   {
		   bIsMissing = true; // then mark it as missing, no use to look for it again in the future
		   return FALSE; // and give up
	   }

	   gGameDLLFunc.newapi_table = pFunctionTable;

	   // else call the function that provides the new DLL functions interface on request
	   return (!(*pGetNewDLLFunctions)(pFunctionTable, interfaceVersion));
   }

   return FALSE;
}

extern "C" EXPORT int Server_GetBlendingInterface( int version, struct sv_blending_interface_s **ppinterface, struct engine_studio_api_s *pstudio, float (*rotationmatrix)[3][4], float (*bonetransform)[MAXSTUDIOBONES][3][4] )
{
	static SERVER_GETBLENDINGINTERFACE pServer_GetBlendingInterface = nullptr;
	static bool bIsMissing = false;

	// if the game .dll has no Server_GetBlendingInterface exported
	if( bIsMissing )
	{
		return FALSE;
	}

	// if the server blending function is unknown
	if( !pServer_GetBlendingInterface )
	{
		pServer_GetBlendingInterface = (SERVER_GETBLENDINGINTERFACE)GetProcAddress(h_Library, "Server_GetBlendingInterface");
	}

	// if it wasn't found
	if( !pServer_GetBlendingInterface )
	{
		bIsMissing = true;
		return FALSE;
	}

	return (pServer_GetBlendingInterface)(version, ppinterface, pstudio, rotationmatrix, bonetransform);
}

extern "C" EXPORT void SV_SaveGameComment( char *pBuffer, int maxLength )
{
	SV_SAVEGAMECOMMENT pSV_SaveGameComment = (SV_SAVEGAMECOMMENT)GetProcAddress(h_Library, "SV_SaveGameComment");

	// if it wasn't found
	if( !pSV_SaveGameComment )
	{
		if( pGame )
		{
			pGame->GetSaveGameComment( pBuffer, maxLength );
		}
		else
		{
			ALERT( at_error, "Call to SV_SaveGameComment before pGame is initialised!\n" );
		}

		return;
	}

	(pSV_SaveGameComment)(pBuffer, maxLength);
}
