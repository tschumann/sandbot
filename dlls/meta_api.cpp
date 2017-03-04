//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// meta_api.cpp
//

#include <extdll.h>
#include <meta_api.h>
#include "sdk_util.h"
#include "studio.h"

#include "bot.h"

// Loaded as metamod plugin?
bool g_bIsMMPlugin = false;

extern bot_player_t *pBotData;

// Must provide at least one of these..
static META_FUNCTIONS gMetaFunctionTable = {
	GetEntityAPI,		// pfnGetEntityAPI				HL SDK; called before game DLL
	GetEntityAPI_Post,	// pfnGetEntityAPI_Post			META; called after game DLL
	NULL,				// pfnGetEntityAPI2				HL SDK2; called before game DLL
	NULL,				// pfnGetEntityAPI2_Post		META; called after game DLL
	GetNewDLLFunctions,	// pfnGetNewDLLFunctions		HL SDK2; called before game DLL
	NULL,				// pfnGetNewDLLFunctions_Post	META; called after game DLL
	GetEngineFunctions,	// pfnGetEngineFunctions	META; called before HL engine
	NULL,				// pfnGetEngineFunctions_Post	META; called after HL engine
};

// Description of plugin
plugin_info_t Plugin_info = {
	META_INTERFACE_VERSION,	// ifvers
	"Sandbot",		// name
	"0.4.0",		// version
	"2017/03/04",	// date
	"Team Sandpit",	// author
	"http://www.teamsandpit.com/",	// url
	"SANDBOT",	// logtag, all caps please
	PT_STARTUP,	// (when) loadable
	PT_NEVER,	// (when) unloadable
};

extern DLL_FUNCTIONS other_gFunctionTable;

// Global vars from metamod:
meta_globals_t *gpMetaGlobals;		// metamod globals
gamedll_funcs_t *gpGamedllFuncs;	// gameDLL function tables
mutil_funcs_t *gpMetaUtilFuncs;		// metamod utility functions

									// Metamod requesting info about this plugin:
									//  ifvers			(given) interface_version metamod is using
									//  pPlugInfo		(requested) struct with info about plugin
									//  pMetaUtilFuncs	(given) table of utility functions provided by metamod
C_DLLEXPORT int Meta_Query( char *ifvers, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs )
{
	// Give metamod our plugin_info struct
	*pPlugInfo = &Plugin_info;
	// Get metamod utility function table.
	gpMetaUtilFuncs = pMetaUtilFuncs;

	// Check for interface version compatibility
	if( strcmp( ifvers, Plugin_info.ifvers ) != 0 )
	{
		int mmajor = 0, mminor = 0, pmajor = 0, pminor = 0;

		LOG_CONSOLE( PLID, "%s: meta-interface version mismatch (metamod: %s, %s: %s)",
			Plugin_info.name, ifvers, Plugin_info.name, Plugin_info.ifvers );
		LOG_MESSAGE( PLID, "%s: meta-interface version mismatch (metamod: %s, %s: %s)",
			Plugin_info.name, ifvers, Plugin_info.name, Plugin_info.ifvers );

		// if plugin has later interface version, it's incompatible (update metamod)
		sscanf( ifvers, "%d:%d", &mmajor, &mminor );
		sscanf( META_INTERFACE_VERSION, "%d:%d", &pmajor, &pminor );

		if( pmajor > mmajor || (pmajor == mmajor && pminor > mminor) )
		{
			LOG_CONSOLE( PLID, "metamod version is too old for this plugin; update metamod" );
			LOG_ERROR( PLID, "metamod version is too old for this plugin; update metamod" );

			return FALSE;
		}
		// if plugin has older major interface version, it's incompatible (update plugin)
		else if( pmajor < mmajor )
		{
			LOG_CONSOLE( PLID, "metamod version is incompatible with this plugin; please find a newer version of this plugin" );
			LOG_ERROR( PLID, "metamod version is incompatible with this plugin; please find a newer version of this plugin" );

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
C_DLLEXPORT int Meta_Attach( PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs )
{
	// Are we allowed to load this plugin now?
	if( now > Plugin_info.loadable )
	{
		LOG_CONSOLE( PLID, "%s: plugin NOT attaching (can't load plugin right now)",
			Plugin_info.name );
		LOG_ERROR( PLID, "%s: plugin NOT attaching (can't load plugin right now)",
			Plugin_info.name );

		return FALSE; // returning FALSE prevents metamod from attaching this plugin
	}

	if( !pMGlobals )
	{
		LOG_ERROR( PLID, "Meta_Attach called with null pMGlobals" );
		return(FALSE);
	}
	gpMetaGlobals = pMGlobals;

	if( !pFunctionTable )
	{
		LOG_ERROR( PLID, "Meta_Attach called with null pFunctionTable" );
		return(FALSE);
	}
	memcpy( pFunctionTable, &gMetaFunctionTable, sizeof( META_FUNCTIONS ) );
	gpGamedllFuncs = pGamedllFuncs;

	memcpy( &other_gFunctionTable, pGamedllFuncs->dllapi_table, sizeof( DLL_FUNCTIONS ) );

	return TRUE;
}

// Metamod detaching plugin from the server.
// now		(given) current phase, ie during map, etc
// reason	(given) why detaching (refresh, console unload, forced unload, etc)
C_DLLEXPORT int Meta_Detach( PLUG_LOADTIME now, PL_UNLOAD_REASON reason )
{
	// This function is called when metamod unloads the plugin. 
	// A basic check is made in order to prevent unloading the plugin 
	// if its processing should not be interrupted.

	// Is metamod allowed to unload the plugin?
	if( now > Plugin_info.unloadable && reason != PNL_CMD_FORCED )
	{
		LOG_CONSOLE( PLID, "%s: plugin NOT detaching (can't unload plugin right now)",
			Plugin_info.name );
		LOG_ERROR( PLID, "%s: plugin NOT detaching (can't unload plugin right now)",
			Plugin_info.name );

		return FALSE; // returning FALSE prevents metamod from unloading this plugin
	}

	// Kick all bots off this server
	// TODO: move this stuff into a function
	// apparently this can be called more than once, so check before deleting
	// kick any bot off of the server after time/frag limit...
	for( int index = 0; index < MAX_PLAYERS; index++ )
	{
		// TODO:
		if( pBots && pBots[index]->is_used )	// is this slot used?
		{
			char cmd[40];

			sprintf( cmd, "kick \"%s\"\n", pBots[index]->name );

			pBots[index]->respawn_state = RESPAWN_IDLE;

			SERVER_COMMAND( cmd );	// kick the bot using (kick "name")
		}
	}

	if( pGame )
	{
		delete pGame;
		pGame = NULL;
	}
	if( pBotData )
	{
		for( int i = 0; i < MAX_PLAYERS; i++ )
		{
			pBotData[i].bIsUsed = false;
		}
	}
	if( pBots )
	{
		for( int i = 0; i < MAX_PLAYERS; i++ )
		{
			delete pBots[i];
			pBots[i] = NULL;
		}
		pBots = NULL;
	}

	return TRUE; // returning TRUE enables metamod to unload this plugin
}

C_DLLEXPORT void Meta_Init( void )
{
	g_bIsMMPlugin = true;
}
