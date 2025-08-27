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
#include "interface.h"

#include "bot.h"
#include "game.h"
#include "game_halflife.h"
#include "game_opposingforce.h"
#include "game_dayofdefeat.h"
#include "game_teamfortressclassic.h"
#include "game_deathmatchclassic.h"
#include "game_ricochet.h"
#include "game_gunmanchronicles.h"
#include "game_svencoop.h"
#include "game_naturalselection.h"
#include "game_theship.h"
#include "game_theyhunger.h"
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

/// <summary>
/// Metamod calls this functions so we can override what is in pengfuncsFromEngine - we can't do
/// this in GiveFnptrsToDll because it seems like with Metamod we don't actually pass pengfuncsFromEngine
/// to the downstream game .dll (Metamod itself takes care of that).
/// </summary>
/// <param name="pengfuncsFromEngine"></param>
/// <param name="interfaceVersion"></param>
/// <returns></returns>
extern "C" EXPORT int GetEngineFunctions( enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion )
{
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
	// NOTE: we don't bother hooking these functions as there's nothing worthwhile to do when hooking them
	// pengfuncsFromEngine->pfnAlertMessage = pengfuncsFromEngine->pfnAlertMessage;
	// pengfuncsFromEngine->pfnEngineFprintf = pengfuncsFromEngine->pfnEngineFprintf;
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
	// NOTE: really avoid doing anything but hooking/passing through with the below - they're recent additions to the engine and not all mod .dll files (including Metamod) will know about them
	pengfuncsFromEngine->pfnQueryClientCvarValue = pfnQueryClientCvarValue;
	pengfuncsFromEngine->pfnQueryClientCvarValue2 = pfnQueryClientCvarValue2;
	pengfuncsFromEngine->pfnCheckParm = pfnCheckParm;
	// NOTE: omit this one because it causes Metamod to crash - mismatch in sizes of enginefuncs_t maybe? Metamod has extra_functions at the end of enginefuncs_t so that shouldn't be it
	// pengfuncsFromEngine->pfnPEntityOfEntIndexAllEntities = pfnPEntityOfEntIndexAllEntities;

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
	char *pGameDir = szGameDir;
	char *szLibraryPath = "";
	GET_GAME_DIR(pGameDir);

	pengfuncsFromEngine->pfnAlertMessage( at_console, "Hooked GiveFnptrsToDll\n" );

	pGameDir = UTIL_ToLower( pGameDir );

	if( !strcmp( pGameDir, "valve" ) )
	{
		pGame = std::make_unique<ValveGame>(GameId::GAME_VALVE);

#ifndef __linux__
		szLibraryPath = "valve/dlls/hl.dll";
#else
		szLibraryPath = "valve/dlls/hl.so";
#endif
	}
	else if( !strcmp( pGameDir, "gearbox" ) )
	{
		pGame = std::make_unique<GearboxGame>(GameId::GAME_GEARBOX);

#ifndef __linux__
		szLibraryPath = "gearbox/dlls/opfor.dll";
#else
		szLibraryPath = "gearbox/dlls/opfor.so";
#endif
	}
	else if( !strcmp( pGameDir, "dod" ) )
	{
		pGame = std::make_unique<DODGame>(GameId::GAME_DOD);

#ifndef __linux__
		szLibraryPath = "dod/dlls/dod.dll";
#else
		szLibraryPath = "dod/dlls/dod.so";
#endif
	}
	else if( !strcmp( pGameDir, "tfc" ) )
	{
		pGame = std::make_unique<TFCGame>(GameId::GAME_TFC);

#ifndef __linux__
		szLibraryPath = "tfc/dlls/tfc.dll";
#else
		szLibraryPath = "tfc/dlls/tfc.so";
#endif
	}
	else if( !strcmp( pGameDir, "dmc" ) )
	{
		pGame = std::make_unique<DeathmatchClassicGame>(GameId::GAME_DMC);

#ifndef __linux__
		szLibraryPath = "dmc/dlls/dmc.dll";
#else
		szLibraryPath = "dmc/dlls/dmc.so";
#endif
	}
	else if( !strcmp( pGameDir, "ricochet" ) )
	{
		pGame = std::make_unique<RicochetGame>(GameId::GAME_RICOCHET);

#ifndef __linux__
		szLibraryPath = "ricochet/dlls/mp.dll";
#else
		szLibraryPath = "ricochet/dlls/ricochet.so";
#endif
	}
	else if( !strcmp( pGameDir, "rewolf" ) )
	{
		pGame = std::make_unique<RewolfGame>(GameId::GAME_REWOLF);

#ifndef __linux__
		szLibraryPath = "rewolf/dlls/gunman.dll";
#else
		szLibraryPath = nullptr;
#endif
	}
	else if( !strcmp( pGameDir, "svencoop" ) )
	{
		pGame = std::make_unique<SvenCoopGame>(GameId::GAME_SVENCOOP);

#ifndef __linux__
		szLibraryPath = "svencoop/dlls/server.dll";
#else
		szLibraryPath = "svencoop/dlls/server.so";
#endif
	}
	else if( !strcmp( pGameDir, "hunger" ) )
	{
		pGame = std::make_unique<HungerGame>(GameId::GAME_HUNGER);

#ifndef __linux__
		szLibraryPath = "hunger/dlls/einar.dll";
#else
		szLibraryPath = nullptr;
#endif
	}
	else if( !strcmp( pGameDir, "ns") )
	{
		pGame = std::make_unique<NSGame>(GameId::GAME_NS);

#ifndef __linux__
		szLibraryPath = "ns/dlls/ns.dll";
#else
		szLibraryPath = "ns/dlls/ns_i386.so";
#endif
	}
	else if( !strcmp( pGameDir, "ship" ) )
	{
		pGame = std::make_unique<ShipGame>(GameId::GAME_SHIP);

#ifndef __linux__
		szLibraryPath = "ship/dlls/ship.dll";
#else
		szLibraryPath = "ship/dlls/ship_i386.so";
#endif
	}

	int iError = 0;

	if( !g_bIsMMPlugin )
	{
#ifndef __linux__
		h_Library = LoadLibrary( (const char *)szLibraryPath );
		iError = GetLastError();
#else
		h_Library = dlopen( szLibraryPath, RTLD_NOW );
#endif
	}

	strncpy( g_szLibraryPath, szLibraryPath, strlen( szLibraryPath ) );

	if( h_Library == nullptr )
	{
		// TODO: this is because the function pointers haven't been copied across (and in this case haven't been found at all) - log this to a file if through metamod
		if( !g_bIsMMPlugin )
		{
			ALERT( at_error, "Library not found or not supported (%s) - error %d!\n", pGameDir, iError );
		}

		return;
	}

	pengfuncsFromEngine->pfnAlertMessage(at_console, "Determined game from szLibraryPath %s\n", szLibraryPath);

	InitBots();

	LoadExtraExports();

	GetEngineFunctions(pengfuncsFromEngine, nullptr);

	if( g_bIsMMPlugin )
	{
		return;
	}

	// give the engine functions to the other DLL...
	(*(GIVEFNPTRSTODLL)GetProcAddress( h_Library, "GiveFnptrsToDll" ))(pengfuncsFromEngine, pGlobals);

	// TODO: is this right? we won't want to Sys_LoadModule as we want to use the existing library that we're talking to, but why so many types that all look the same?
	CreateInterfaceFn factory = Sys_GetFactory( (HINTERFACEMODULE)h_Library );

	if( factory != NULL )
	{
		ALERT( at_console, "Got factory from library\n" );
	}
	else
	{
		ALERT( at_warning, "Unable to get factory from library\n" );
	}

	// finished, interfacing from gamedll to engine complete
	return;
}

gamedll_funcs_t gGameDLLFunc;

extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
	ALERT( at_console, "Hooked GetEntityAPI with interfaceVersion %d\n", interfaceVersion );
   // check if engine's pointer is valid and version is correct...
   if ((pFunctionTable == nullptr) || (interfaceVersion != INTERFACE_VERSION))
   {
	   return 0;
   }

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

   return 1; // finished, interfacing from engine to gamedll complete
}

extern "C" EXPORT int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable, int *interfaceVersion)
{
	ALERT(at_console, "Hooked GetEntityAPI2 with interfaceVersion %d\n", *interfaceVersion);
	// check if engine's pointer is valid and version is correct...
	if ((pFunctionTable == nullptr) || (*interfaceVersion != INTERFACE_VERSION))
	{
		return 0;
	}

	memset(pFunctionTable, 0, sizeof(DLL_FUNCTIONS));

	if (!g_bIsMMPlugin)
	{
		// pass other DLLs engine callbacks to function table...
		if (!(*(GETENTITYAPI2)GetProcAddress(h_Library, "GetEntityAPI2")) (&other_gFunctionTable, interfaceVersion))
			return FALSE;  // error initializing function table!!!

		gGameDLLFunc.dllapi_table = &other_gFunctionTable;
		gpGamedllFuncs = &gGameDLLFunc;

		memcpy(pFunctionTable, &other_gFunctionTable, sizeof(DLL_FUNCTIONS));
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

	return 1; // finished, interfacing from engine to gamedll complete
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
