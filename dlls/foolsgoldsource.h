#ifndef _STUB_ENGINE_H_
#define _STUB_ENGINE_H_

#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "extdll.h"

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;
using std::vector;

namespace foolsgoldsource
{
	class Engine
	{
	public:
		Engine();

		enginefuncs_t GetServerEngineFunctions();
		globalvars_t GetServerGlobalVariables();

		string GetGameDirectory();
		void SetGameDirectory( string strGameDir );
		bool GetIsDedicatedServer();
		void SetIsDedicatedServer( bool bIsDedicatedServer );

		void SetMaxClients( int iMaxClients );

		// below shouldn't be public because the game doesn't have access to them

		// TODO: below be in some server struct?
		vector<edict_t*> edicts;
		vector<string> models;
		vector<string> sounds;
		int iMaxEdicts;
	private:
		globalvars_t globalVariables;

		string strGameDir;

		bool bIsDedicatedServer;
	};

	extern Engine gEngine;

	int pfnPrecacheModel( char* s );
	int pfnPrecacheSound( char* s );

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... );

	edict_t* pfnPEntityOfEntOffset( int iEntOffset );

	edict_t* pfnPEntityOfEntIndex( int iEntIndex );

	void pfnGetGameDir( char *szGetGameDir );

	int pfnIsDedicatedServer( void );

	edict_t* pfnPEntityOfEntIndexAllEntities( int iEntIndex );
}

#endif // _STUB_ENGINE_H_