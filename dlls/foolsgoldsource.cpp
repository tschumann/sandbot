#include "foolsgoldsource.h"

namespace foolsgoldsource
{
	Engine gEngine;

	enginefuncs_t Engine::GetServerEngineFunctions()
	{
		enginefuncs_t engineFunctions;

		engineFunctions.pfnAlertMessage = pfnAlertMessage;
		engineFunctions.pfnPEntityOfEntOffset = pfnPEntityOfEntOffset;
		engineFunctions.pfnPEntityOfEntIndex = pfnPEntityOfEntIndex;
		engineFunctions.pfnGetGameDir = pfnGetGameDir;
		engineFunctions.pfnIsDedicatedServer = pfnIsDedicatedServer;
		engineFunctions.pfnPEntityOfEntIndexAllEntities = pfnPEntityOfEntIndexAllEntities;

		return engineFunctions;
	}

	globalvars_t Engine::GetServerGlobalVariables()
	{
		return this->globalVariables;
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

	void Engine::SetMaxClients( int iMaxClients )
	{
		this->globalVariables.maxClients = iMaxClients;
	}

	/////////////////////////////////
	// Stubbed enginefuncs_t below //
	/////////////////////////////////

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... )
	{
		printf( "%s", szFmt );
	}

	edict_t* pfnPEntityOfEntOffset( int iEntOffset )
	{
		vector<edict_t*> edicts = gEngine.edicts;

		if( iEntOffset >= edicts.size() )
		{
			return nullptr;
		}
		else
		{
			return edicts[iEntOffset];
		}
	}

	edict_t* pfnPEntityOfEntIndex( int iEntIndex )
	{
		edict_t* result;

		// TODO: is pfnPEntityOfEntOffset the same as EDICT_NUM?
		if( iEntIndex < 0 ||
			iEntIndex >= gEngine.iMaxEdicts ||
			( (result = pfnPEntityOfEntOffset( iEntIndex )) == nullptr || result->free || !result->pvPrivateData ) &&
			( iEntIndex >= gEngine.GetServerGlobalVariables().maxClients || result->free ) ) // this check on result fails if there are no edicts - never happens in GoldSource?
		{
			result = nullptr;
		}

		return result;
	}

	void pfnGetGameDir( char *szGetGameDir )
	{
		strcpy( szGetGameDir, gEngine.GetGameDirectory().c_str() );
	}

	int pfnIsDedicatedServer( void )
	{
		return gEngine.GetIsDedicatedServer();
	}

	edict_t* pfnPEntityOfEntIndexAllEntities( int iEntIndex )
	{
		edict_t* result;

		// TODO: is pfnPEntityOfEntOffset the same as EDICT_NUM?
		if( iEntIndex < 0 ||
			iEntIndex >= gEngine.iMaxEdicts ||
			( (result = pfnPEntityOfEntOffset( iEntIndex )) == nullptr || result->free || !result->pvPrivateData ) &&
			( iEntIndex > gEngine.GetServerGlobalVariables().maxClients || result->free ) ) // this check on result fails if there are no edicts - never happens in GoldSource?
		{
			result = nullptr;
		}

		return result;
	}
}