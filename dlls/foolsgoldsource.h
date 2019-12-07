#ifndef _STUB_ENGINE_H_
#define _STUB_ENGINE_H_

#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "extdll.h"
#include "enginecallback.h"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace foolsgoldsource
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		const enginefuncs_t GetServerEngineFunctions();
		const globalvars_t GetServerGlobalVariables();

		string GetGameDirectory();
		void SetGameDirectory( string strGameDir );
		bool GetIsDedicatedServer();
		void SetIsDedicatedServer( bool bIsDedicatedServer );

		void SetMaxClients( int iMaxClients );

		// below shouldn't be public because the game doesn't have access to them

		// TODO: below be in some server struct?
		vector<shared_ptr<edict_t>> edicts;
		vector<string> models;
		vector<string> sounds;
		int iMaxEdicts;

		// TODO: how does the engine track this?
		unsigned int iStringTableOffset;
	private:
		enginefuncs_t engineFunctions;
		globalvars_t globalVariables;

		string strGameDir;

		bool bIsDedicatedServer;
	};

	extern Engine gEngine;

	int pfnPrecacheModel( char* s );
	int pfnPrecacheSound( char* s );

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... );

	int pfnAllocString( const char* szValue );

	edict_t* pfnPEntityOfEntOffset( int iEntOffset );

	edict_t* pfnPEntityOfEntIndex( int iEntIndex );

	void pfnGetGameDir( char *szGetGameDir );

	int pfnIsDedicatedServer( void );

	edict_t* pfnPEntityOfEntIndexAllEntities( int iEntIndex );
}

#endif // _STUB_ENGINE_H_