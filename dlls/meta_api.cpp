//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// meta_api.cpp
//

#include <extdll.h>
#include <meta_api.h>

#include "bot.h"
#include "cbase.h"
#include "game.h"
#include "linkfunc.h"
#include "metamod.h"

// Loaded as metamod plugin?
bool g_bIsMMPlugin = false;

// Must provide at least one of these..
static META_FUNCTIONS gMetaFunctionTable = {
	GetEntityAPI,		// pfnGetEntityAPI				HL SDK; called before game DLL
	GetEntityAPI_Post,	// pfnGetEntityAPI_Post			META; called after game DLL
	GetEntityAPI2,	    // pfnGetEntityAPI2				HL SDK2; called before game DLL
	GetEntityAPI2_Post, // pfnGetEntityAPI2_Post		META; called after game DLL
	GetNewDLLFunctions,	// pfnGetNewDLLFunctions		HL SDK2; called before game DLL
	NULL,				// pfnGetNewDLLFunctions_Post	META; called after game DLL
	GetEngineFunctions,	// pfnGetEngineFunctions	META; called before HL engine
	NULL,				// pfnGetEngineFunctions_Post	META; called after HL engine
};

// Description of plugin
plugin_info_t Plugin_info = {
	META_INTERFACE_VERSION,	// ifvers
	"Sandbot",		// name
	"0.5",		// version
	"2025/08/18",	// date
	"Team Sandpit",	// author
	"https://www.teamsandpit.com/",	// url
	"SANDBOT",	// logtag, all caps please
	PT_STARTUP,	// (when) loadable
	PT_NEVER,	// (when) unloadable
};

int DispatchSpawn_Post( edict_t * pent )
{
	// solves the bots unable to see through certain types of glass bug.
	// MAPPERS: NEVER EVER ALLOW A TRANSPARENT ENTITY TO WEAR THE FL_WORLDBRUSH FLAG !!!

	if( pent->v.rendermode == kRenderTransTexture )
		pent->v.flags &= ~FL_WORLDBRUSH;  // clear the FL_WORLDBRUSH flag out of transparent ents

	return METAMOD_RETURN_VALUE( MRES_IGNORED, 0 );
}

extern "C" EXPORT int GetEntityAPI_Post( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
	memset( pFunctionTable, 0, sizeof(DLL_FUNCTIONS) );

	pFunctionTable->pfnSpawn = DispatchSpawn_Post;

	return 1;
}

extern "C" EXPORT int GetEntityAPI2_Post( DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
{
	memset( pFunctionTable, 0, sizeof(DLL_FUNCTIONS) );

	pFunctionTable->pfnSpawn = DispatchSpawn_Post;

	return 1;
}

extern DLL_FUNCTIONS other_gFunctionTable;

// Global vars from metamod:
meta_globals_t *gpMetaGlobals;		// metamod globals
gamedll_funcs_t *gpGamedllFuncs;	// gameDLL function tables
mutil_funcs_t *gpMetaUtilFuncs;		// metamod utility functions

									// Metamod requesting info about this plugin:
									//  ifvers			(given) interface_version metamod is using
									//  pPlugInfo		(requested) struct with info about plugin
									//  pMetaUtilFuncs	(given) table of utility functions provided by metamod
extern "C" EXPORT int Meta_Query( char *ifvers, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs )
{
	// Give metamod our plugin_info struct
	*pPlugInfo = &Plugin_info;
	// Get metamod utility function table.
	gpMetaUtilFuncs = pMetaUtilFuncs;

	// Check for interface version compatibility
	if( strcmp( ifvers, Plugin_info.ifvers ) != 0 )
	{
		int mmajor = 0, mminor = 0, pmajor = 0, pminor = 0;

		ALERT( at_error, "%s: meta-interface version mismatch (metamod: %s, %s: %s)", Plugin_info.name, ifvers, Plugin_info.name, Plugin_info.ifvers );

		// if plugin has later interface version, it's incompatible (update metamod)
		sscanf( ifvers, "%d:%d", &mmajor, &mminor );
		sscanf( META_INTERFACE_VERSION, "%d:%d", &pmajor, &pminor );

		if( pmajor > mmajor || (pmajor == mmajor && pminor > mminor) )
		{
			ALERT( at_error, "metamod version is too old for this plugin; update metamod\n" );

			return FALSE;
		}
		// if plugin has older major interface version, it's incompatible (update plugin)
		else if( pmajor < mmajor )
		{
			ALERT( at_error, "metamod version is incompatible with this plugin; please find a newer version of this plugin\n" );

			return FALSE;
		}
	}

	return TRUE; // tell metamod this plugin looks safe
}

// Metamod attaching plugin to the server.
//  now				(given) current phase, ie during map, during changelevel, or at startup
//  pFunctionTable	(requested) table of function tables this plugin catches
//  pMGlobals		(given) global vars from metamod
//  pGamedllFuncs	(given) copy of function tables from game dll
extern "C" EXPORT int Meta_Attach( PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs )
{
	// Are we allowed to load this plugin now?
	if( now > Plugin_info.loadable )
	{
		ALERT( at_error, "%s: plugin NOT attaching (can't load plugin right now)\n", Plugin_info.name );

		return FALSE; // returning FALSE prevents metamod from attaching this plugin
	}

	if( !pMGlobals )
	{
		ALERT( at_error, "Meta_Attach called with null pMGlobals\n" );
		return FALSE;
	}
	gpMetaGlobals = pMGlobals;

	if( !pFunctionTable )
	{
		ALERT( at_error, "Meta_Attach called with null pFunctionTable\n" );
		return FALSE;
	}
	memcpy( pFunctionTable, &gMetaFunctionTable, sizeof( META_FUNCTIONS ) );
	gpGamedllFuncs = pGamedllFuncs;

	memcpy( &other_gFunctionTable, pGamedllFuncs->dllapi_table, sizeof( DLL_FUNCTIONS ) );

	extern std::unique_ptr<Game> pGame;

	InitBots();

	LoadExtraExports();

	return TRUE;
}

// Metamod detaching plugin from the server.
// now		(given) current phase, ie during map, etc
// reason	(given) why detaching (refresh, console unload, forced unload, etc)
extern "C" EXPORT int Meta_Detach( PLUG_LOADTIME now, PL_UNLOAD_REASON reason )
{
	// This function is called when metamod unloads the plugin. 
	// A basic check is made in order to prevent unloading the plugin 
	// if its processing should not be interrupted.

	// Is metamod allowed to unload the plugin?
	if( now > Plugin_info.unloadable && reason != PNL_CMD_FORCED )
	{
		ALERT( at_error, "%s: plugin NOT detaching (can't unload plugin right now)\n", Plugin_info.name );

		return FALSE; // returning FALSE prevents metamod from unloading this plugin
	}

	extern void KickAllBots();
	KickAllBots();

	extern void CleanupGameAndBots();
	CleanupGameAndBots();

	return TRUE; // returning TRUE enables metamod to unload this plugin
}

extern "C" EXPORT void Meta_Init( void )
{
	g_bIsMMPlugin = true;
}
