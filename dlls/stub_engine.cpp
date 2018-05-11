
#include "stub_engine.h"

namespace stub_engine
{
	Engine gEngine;

	enginefuncs_t Engine::GetServerEngineFunctions()
	{
		enginefuncs_t engineFunctions;

		engineFunctions.pfnGetGameDir = pfnGetGameDir;
		engineFunctions.pfnIsDedicatedServer = pfnIsDedicatedServer;

		return engineFunctions;
	}

	globalvars_t Engine::GetServerGlobalVariables()
	{
		globalvars_t globalVariables;

		return globalVariables;
	}


	void assertTrue( bool bCondition, string strMessage )
	{
		if( !bCondition )
		{
			cout << strMessage << endl;
		}
	}


	string Engine::GetGameDirectory()
	{
		return this->strGameDir;
	}

	void pfnGetGameDir( char *szGetGameDir )
	{
		strcpy( szGetGameDir, gEngine.GetGameDirectory().c_str() );
	}

	int pfnIsDedicatedServer( void )
	{
		return 0;
	}
}