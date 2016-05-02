//
// gbot - The GoldSource bot
//
// <no site>
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

#ifndef __linux__
HINSTANCE hLibrary = NULL;
#else
void *hLibrary = NULL;
#endif

enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;
char g_argv[1024];

extern DLL_FUNCTIONS gFunctionTable;
extern NEW_DLL_FUNCTIONS gNewFunctionTable;

extern int g_iMod;

#ifndef __linux__

// Required DLL entry point
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
    if( fdwReason == DLL_PROCESS_DETACH )
    {
        if( hLibrary )
		{
            FreeLibrary( hLibrary );
		}
    }

    return TRUE;
}

#endif

#ifndef __linux__
void DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
#else
extern "C" void GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
#endif
{
	char szGamedir[256];

    // get the engine functions from the engine...
	memcpy( &g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t) );

    gpGlobals = pGlobals;

    // find the directory name of the currently running MOD...
    GET_GAME_DIR( szGamedir );

	if( !_strcmpi( szGamedir, "valve" ) )
    {
		g_iMod = VALVE_DLL;

#ifndef __linux__
		hLibrary = LoadLibrary( "valve/dlls/hl.dll" );
#else
		hLibrary = dlopen( "valve/dlls/hl_i386.so", RTLD_NOW );
#endif
    }
	else if( !_strcmpi( szGamedir, "gearbox" ) )
    {
		g_iMod = GEARBOX_DLL;

		hLibrary = LoadLibrary( "gearbox/dlls/opfor.dll" );
    }
	else if( !_strcmpi( szGamedir, "cstrike" ) )
    {
		g_iMod = CSTRIKE_DLL;

#ifndef __linux__
		hLibrary = LoadLibrary( "cstrike/dlls/mp.dll" );
#else
		hLibrary = dlopen( "cstrike/dlls/cs_i386.so", RTLD_NOW );
#endif
    }
	else if( !_strcmpi( szGamedir, "dod" ) )
    {
		g_iMod = DOD_DLL;

#ifndef __linux__
		hLibrary = LoadLibrary( "dod/dlls/dod.dll" );
#else
		hLibrary = dlopen( "dod/dlls/dod_i386.so", RTLD_NOW );
#endif
    }
	else if( !_strcmpi( szGamedir, "tfc" ) )
    {
		g_iMod = TFC_DLL;

#ifndef __linux__
		hLibrary = LoadLibrary( "tfc/dlls/tfc.dll" );
#else
		hLibrary = dlopen( "tfc/dlls/tfc_i386.so", RTLD_NOW );
#endif
    }
    else if( !_strcmpi( szGamedir, "rewolf" ) )
    {
		g_iMod = GUNMAN_DLL;

		hLibrary = LoadLibrary( "rewolf/dlls/gunman.dll" );
    }
    else if( !_strcmpi( szGamedir, "hunger" ) )
    {
		g_iMod = HUNGER_DLL;

		hLibrary = LoadLibrary( "hunger/dlls/einar.dll" );
    }
	else if( !_strcmpi( szGamedir, "ns" ) )
    {
		g_iMod = NS_DLL;

#ifndef __linux__
		hLibrary = LoadLibrary( "ns/dlls/ns.dll" );
#else
		hLibrary = dlopen( "ns/dlls/ns_i386.so", RTLD_NOW );
#endif
    }
    else if( !_strcmpi( szGamedir, "ship" ) )
    {
		g_iMod = SHIP_DLL;

#ifndef __linux__
		hLibrary = LoadLibrary( "ship/dlls/ship.dll" );
#else
		hLibrary = dlopen( "ship/dlls/ship_i386.so", RTLD_NOW );
#endif
    }

    if( !hLibrary )
    {
		// Directory error or Unsupported MOD!
		ALERT( at_error, "Error: mod .dll not found or mod not supported!" );
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
    (*(GIVEFNPTRSTODLL)GetProcAddress( hLibrary, "GiveFnptrsToDll" ))( pengfuncsFromEngine, pGlobals );

    return; // finished, interfacing from gamedll to engine complete
}

extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
    // check if engine's pointer is valid and version is correct...
    if( !pFunctionTable || ( interfaceVersion != INTERFACE_VERSION ) )
	{
        return FALSE;
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

    // and now we need to get the REAL gamedll functions, for our own use (call GetEntityAPI()
    // in the game DLL and have it fill in our functions table)...

    // was the call NOT successful ?
    if( !(*(GETENTITYAPI)GetProcAddress( hLibrary, "GetEntityAPI" ))( &gFunctionTable, INTERFACE_VERSION ) )
	{
        return FALSE;  // error initializing function table!!!
	}

    return TRUE; // finished, interfacing from engine to gamedll complete
}

extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
{
	// check if engine's pointer is valid and version is correct...
    if( !pFunctionTable || ( *interfaceVersion != NEW_DLL_FUNCTIONS_VERSION ) )
	{
        return FALSE;
	}

	if( !GetProcAddress( hLibrary, "GetNewDLLFunctions" ) )
	{
		ALERT( at_console, "No GetNewDLLFunctions found\n" );

		return FALSE;
	}

	pFunctionTable->pfnOnFreeEntPrivateData = OnFreeEntPrivateData;
	pFunctionTable->pfnGameShutdown = GameShutdown;
	pFunctionTable->pfnShouldCollide = ShouldCollide;
	pFunctionTable->pfnCvarValue = CvarValue;
	pFunctionTable->pfnCvarValue2 = CvarValue2;

	// and now we need to get the REAL gamedll functions, for our own use (call GetNewDLLFunctions()
    // in the game DLL and have it fill in our functions table)...

    // was the call NOT successful ?
    if( !(*(GETNEWDLLFUNCTIONS)GetProcAddress( hLibrary, "GetNewDLLFunctions" ))( &gNewFunctionTable, interfaceVersion ) )
	{
        return FALSE;  // error initializing function table!!!
	}

    return TRUE; // finished, interfacing from engine to gamedll complete
}


extern "C" EXPORT int Server_GetBlendingInterface( int version, struct sv_blending_interface_s **ppinterface, struct engine_studio_api_s *pstudio, float (*rotationmatrix)[3][4], float (*bonetransform)[MAXSTUDIOBONES][3][4] )
{
    static SERVER_GETBLENDINGINTERFACE other_Server_GetBlendingInterface = NULL;
    static bool missing = FALSE;

    // if the blending interface has been formerly reported as missing, give up
    if( missing )
	{
        return FALSE;
	}

    // do we NOT know if the blending interface is provided ? if so, look for its address
    if (other_Server_GetBlendingInterface == NULL)
        other_Server_GetBlendingInterface = (SERVER_GETBLENDINGINTERFACE) GetProcAddress (hLibrary, "Server_GetBlendingInterface");

    // have we NOT found it ?
    if (!other_Server_GetBlendingInterface)
    {
        missing = TRUE; // then mark it as missing, no use to look for it again in the future
        return (FALSE); // and give up
    }

    // else call the function that provides the blending interface on request
    return ((other_Server_GetBlendingInterface) (version, ppinterface, pstudio, rotationmatrix, bonetransform));
}
