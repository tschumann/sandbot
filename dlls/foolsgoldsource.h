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
			this->bIsDedicatedServer = false;

			this->globalVariables.maxClients = 32;

			iMaxEdicts = 1024;
		}

		enginefuncs_t GetServerEngineFunctions();
		globalvars_t GetServerGlobalVariables();

		string GetGameDirectory();
		void SetGameDirectory( string strGameDir );
		bool GetIsDedicatedServer();
		void SetIsDedicatedServer( bool bIsDedicatedServer );

		void SetMaxClients( int iMaxClients );

		// TODO: should be in some server struct
		int iMaxEdicts;
	private:
		globalvars_t globalVariables;

		string strGameDir;

		bool bIsDedicatedServer;
	};

	extern Engine gEngine;

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... );

	edict_t* pfnPEntityOfEntOffset( int iEntOffset );

	edict_t* pfnPEntityOfEntIndex( int iEntIndex );

	void pfnGetGameDir( char *szGetGameDir );

	int pfnIsDedicatedServer( void );

	edict_t* pfnPEntityOfEntIndexAllEntities( int iEntIndex );
}

#endif // _STUB_ENGINE_H_