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

#include "engine.h"
#include "dll.h"

HINSTANCE hLibrary = NULL;

enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;

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

void DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
{
	char szGamedir[256];

    // get the engine functions from the engine...
	memcpy( &g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t) );

    gpGlobals = pGlobals;

    // find the directory name of the currently running MOD...
    GET_GAME_DIR( szGamedir );

	hLibrary = LoadLibrary( "czeror/dlls/cz.dll" );

    if( !hLibrary )
    {
		// Directory error or Unsupported MOD!
		ALERT( at_error, "Error: mod .dll not found or mod not supported!" );
    }

    // give the engine functions to the other DLL...
    (*(GIVEFNPTRSTODLL)GetProcAddress( hLibrary, "GiveFnptrsToDll" ))( pengfuncsFromEngine, pGlobals );

    return; // finished, interfacing from gamedll to engine complete
}

extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
    static GETENTITYAPI other_GetEntityAPI = NULL;
    static bool missing = FALSE;

    // if the blending interface has been formerly reported as missing, give up
    if( missing )
	{
        return FALSE;
	}

    // do we NOT know if the blending interface is provided ? if so, look for its address
    if (other_GetEntityAPI == NULL)
        other_GetEntityAPI = (GETENTITYAPI) GetProcAddress (hLibrary, "GetEntityAPI");

    // have we NOT found it ?
    if (!other_GetEntityAPI)
    {
        missing = TRUE; // then mark it as missing, no use to look for it again in the future
        return (FALSE); // and give up
    }

    // else call the function that provides the blending interface on request
    return ((other_GetEntityAPI) (pFunctionTable, interfaceVersion));
}

extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
{
    static GETNEWDLLFUNCTIONS other_GetNewDLLFunctions = NULL;
    static bool missing = FALSE;

    // if the blending interface has been formerly reported as missing, give up
    if( missing )
	{
        return FALSE;
	}

    // do we NOT know if the blending interface is provided ? if so, look for its address
    if (other_GetNewDLLFunctions == NULL)
        other_GetNewDLLFunctions = (GETNEWDLLFUNCTIONS) GetProcAddress (hLibrary, "GetNewDLLFunctions");

    // have we NOT found it ?
    if (!other_GetNewDLLFunctions)
    {
        missing = TRUE; // then mark it as missing, no use to look for it again in the future
        return (FALSE); // and give up
    }

    // else call the function that provides the blending interface on request
    return ((other_GetNewDLLFunctions) (pFunctionTable, interfaceVersion));
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
