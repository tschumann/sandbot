#ifndef _STUB_ENGINE_H_
#define _STUB_ENGINE_H_

#include <cstring>
#include <iostream>
#include <string>

#include "extdll.h"

using std::cout;
using std::endl;
using std::string;

namespace foolsgoldsource
{
	class Engine
	{
	public:
		Engine()
		{
			this->strGameDir = "valve";
		}

		enginefuncs_t GetServerEngineFunctions();
		globalvars_t GetServerGlobalVariables();

		string GetGameDirectory();
	private:
		string strGameDir;
	};

	extern Engine gEngine;

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... );

	void pfnGetGameDir( char *szGetGameDir );

	int pfnIsDedicatedServer( void );
}

#endif // _STUB_ENGINE_H_