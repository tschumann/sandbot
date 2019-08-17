#include "foolsgoldsource.h"

namespace foolsgoldsource
{
	Engine gEngine;

	enginefuncs_t Engine::GetServerEngineFunctions()
	{
		enginefuncs_t engineFunctions;

		engineFunctions.pfnAlertMessage = pfnAlertMessage;
		engineFunctions.pfnGetGameDir = pfnGetGameDir;
		engineFunctions.pfnIsDedicatedServer = pfnIsDedicatedServer;

		return engineFunctions;
	}

	globalvars_t Engine::GetServerGlobalVariables()
	{
		globalvars_t globalVariables;

		return globalVariables;
	}

	string Engine::GetGameDirectory()
	{
		return this->strGameDir;
	}

	void Engine::SetGameDirectory( string strGameDir )
	{
		this->strGameDir = strGameDir;
	}

	bool Engine::GetIsDedicatedServer()
	{
		return this->bIsDedicatedServer;
	}

	void Engine::SetIsDedicatedServer( bool bIsDedicatedServer )
	{
		this->bIsDedicatedServer = bIsDedicatedServer;
	}

	/////////////////////////////////
	// Stubbed enginefuncs_t below //
	/////////////////////////////////

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... )
	{
		printf( "%s", szFmt );
	}

	void pfnGetGameDir( char *szGetGameDir )
	{
		strcpy( szGetGameDir, gEngine.GetGameDirectory().c_str() );
	}

	int pfnIsDedicatedServer( void )
	{
		return gEngine.GetIsDedicatedServer();
	}
}