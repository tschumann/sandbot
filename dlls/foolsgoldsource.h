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
		Engine()
		{
			this->globalVariables.maxClients = 32;

			// TODO: edict_t * 0 is worldspawn?
			for( int i = 0; i <= this->globalVariables.maxClients; i++ )
			{
				unique_ptr<edict_t> edict = std::make_unique<edict_t>();
				this->edicts.push_back(edict.get());
			}

			this->strGameDir = "valve";
			this->bIsDedicatedServer = false;

			iMaxEdicts = 1024;
		}

		enginefuncs_t GetServerEngineFunctions();
		globalvars_t GetServerGlobalVariables();

		string GetGameDirectory();
		void SetGameDirectory( string strGameDir );
		bool GetIsDedicatedServer();
		void SetIsDedicatedServer( bool bIsDedicatedServer );

		void SetMaxClients( int iMaxClients );

		// TODO: should be in some server struct?
		vector<edict_t*> edicts;
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