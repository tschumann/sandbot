#include "foolsgoldsource.h"

extern globalvars_t* gpGlobals;

namespace foolsgoldsource
{
	Engine gEngine;

	Engine::Engine()
	{
		// set up all the engine functions so they can be used
		this->engineFunctions.pfnPrecacheModel = pfnPrecacheModel;
		this->engineFunctions.pfnPrecacheSound = pfnPrecacheSound;
		this->engineFunctions.pfnSetModel = pfnSetModel;
		this->engineFunctions.pfnModelIndex = pfnModelIndex;
		this->engineFunctions.pfnSetSize = pfnSetSize;
		this->engineFunctions.pfnSetOrigin = pfnSetOrigin;
		this->engineFunctions.pfnTraceSphere = pfnTraceSphere;
		this->engineFunctions.pfnParticleEffect = pfnParticleEffect;
		this->engineFunctions.pfnLightStyle = pfnLightStyle;
		this->engineFunctions.pfnAlertMessage = pfnAlertMessage;
		this->engineFunctions.pfnAllocString = pfnAllocString;
		this->engineFunctions.pfnPEntityOfEntOffset = pfnPEntityOfEntOffset;
		this->engineFunctions.pfnPEntityOfEntIndex = pfnPEntityOfEntIndex;
		this->engineFunctions.pfnServerPrint = pfnServerPrint;
		this->engineFunctions.pfnGetGameDir = pfnGetGameDir;
		this->engineFunctions.pfnIsDedicatedServer = pfnIsDedicatedServer;
		this->engineFunctions.pfnIsCareerMatch = pfnIsCareerMatch;
		this->engineFunctions.pfnPEntityOfEntIndexAllEntities = pfnPEntityOfEntIndexAllEntities;

		// install the engine functions and global variables
		::g_engfuncs = this->engineFunctions;
		::gpGlobals = &this->globalVariables;

		this->globalVariables.maxClients = 32;
		this->globalVariables.pStringBase = new char[Engine::iStringTableSize];
		memset( (char *)this->globalVariables.pStringBase, 0, Engine::iStringTableSize);
		// start allocating at offset 1 so that checks against string_t with value 0 work
		// TODO: is this how the engine works?
		this->iStringTableOffset = 1;

		// TODO: edict_t * 0 is worldspawn?
		for( int i = 0; i <= this->globalVariables.maxClients; i++ )
		{
			// TODO: player spawning should happen later - and call one of the server-side callbacks?
			shared_ptr<edict_t> edict = std::make_shared<edict_t>();
			edict->free = 0;
			edict->pvPrivateData = new char[1]; // TODO: should be CBasePlayer's data
			edict->v.classname = ALLOC_STRING("player");
			edict->v.netname = 0;
			edict->v.flags = FL_CLIENT;
			this->edicts.push_back(edict);
		}

		this->strGameDir = "valve";
		this->bIsDedicatedServer = false;

		this->iMaxEdicts = 1024;
	}

	Engine::~Engine()
	{
		for( unsigned int i = 0; i < this->edicts.size(); i++ )
		{
			shared_ptr<edict_t> edict = this->edicts[i];

			if( edict->pvPrivateData )
			{
				delete[] edict->pvPrivateData;
				edict->pvPrivateData = nullptr;
			}
		}
		if( this->globalVariables.pStringBase )
		{
			delete[] this->globalVariables.pStringBase;
			this->globalVariables.pStringBase = nullptr;
		}
	}

	const enginefuncs_t Engine::GetServerEngineFunctions()
	{
		return this->engineFunctions;
	}

	const globalvars_t Engine::GetServerGlobalVariables()
	{
		return this->globalVariables;
	}

	const string Engine::GetGameDirectory()
	{
		return this->strGameDir;
	}

	void Engine::SetGameDirectory( const string& strGameDir )
	{
		this->strGameDir = strGameDir;
	}

	bool Engine::GetIsDedicatedServer()
	{
		return this->bIsDedicatedServer;
	}

	void Engine::SetIsDedicatedServer( const bool bIsDedicatedServer )
	{
		this->bIsDedicatedServer = bIsDedicatedServer;
	}

	bool Engine::GetIsCareerMatch()
	{
		return this->bIsCareerMatch;
	}

	void Engine::SetIsCareerMatch( const bool bIsCareerMatch )
	{
		this->bIsCareerMatch = bIsCareerMatch;
	}

	void Engine::SetMaxClients( const int iMaxClients )
	{
		this->globalVariables.maxClients = iMaxClients;
	}

	string Util::tolowercase( const string& str )
	{
		string lowerCased = str;

		for( unsigned int i = 0; i < str.length(); i++ )
		{
			// not ideal but this is how the engine would be doing it
			lowerCased[i] = tolower( str[i] );
		}

		return lowerCased;
	}

	/////////////////////////////////
	// Stubbed enginefuncs_t below //
	/////////////////////////////////

	int pfnPrecacheModel( char* s )
	{
		printf( "Precache %s\n", s );

		gEngine.models.push_back( string( s ) );

		return gEngine.models.size() - 1;
	}

	int pfnPrecacheSound( char* s )
	{
		printf( "Precache %s\n", s );

		gEngine.sounds.push_back( string( s ) );

		return gEngine.sounds.size() - 1;
	}

	void pfnSetModel( edict_t* e, const char* m )
	{
		// TODO: excessive calls will overflow the string table - is this what the engine does?
		e->v.model = ALLOC_STRING(m);
	}

	void pfnSetOrigin( edict_t* e, const float* rgflOrigin )
	{
		e->v.origin[0] = rgflOrigin[0];
		e->v.origin[1] = rgflOrigin[1];
		e->v.origin[2] = rgflOrigin[2];
	}

	int pfnModelIndex( const char* m )
	{
		for( unsigned int i = 0; i < gEngine.models.size(); i++ )
		{
			if( gEngine.models[i] == Util::tolowercase( string(m) ) )
			{
				return i;
			}
		}

		// TODO: not right - crash instead?
		return -1;
	}

	void pfnSetSize( edict_t* e, const float* rgflMin, const float* rgflMax )
	{
		// TODO: check this
		e->v.mins.x = rgflMin[0];
		e->v.mins.y = rgflMin[1];
		e->v.mins.z = rgflMin[2];
		e->v.maxs.x = rgflMax[0];
		e->v.maxs.y = rgflMax[1];
		e->v.maxs.z = rgflMax[2];
	}

	void pfnTraceSphere( const float* v1, const float* v2, int fNoMonsters, float radius, edict_t* pentToSkip, TraceResult* ptr )
	{
		fprintf( stderr, "Not implemented\n" );
	}

	void pfnParticleEffect( const float* org, const float* dir, float color, float count )
	{
		// TODO: do nothing?
	}

	void pfnLightStyle( int style, char* val )
	{
		// TODO: do nothing?
	}

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... )
	{
		printf( "%s", szFmt );
	}

	int pfnAllocString(const char* szValue)
	{
		globalvars_t globalVars = gEngine.GetServerGlobalVariables();
		// get the next unassigned part of the string table
		const char* pCurrentOffset = globalVars.pStringBase + gEngine.iStringTableOffset;
		// copy the new string to the next unassigned part of the string table
		strcpy((char*)pCurrentOffset, szValue);

		// get the newly assigned string's location
		int iCurrentOffset = gEngine.iStringTableOffset;
		// update the location of the next unassigned part of the string table
		gEngine.iStringTableOffset += strlen(szValue);
		// return the newly assigned string's location
		return iCurrentOffset;
	}

	edict_t* pfnPEntityOfEntOffset( int iEntOffset )
	{
		if( (unsigned int)iEntOffset >= gEngine.edicts.size() )
		{
			return nullptr;
		}
		else
		{
			return gEngine.edicts[iEntOffset].get();
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

	void pfnServerPrint(const char* szMsg)
	{
		printf( "%s", szMsg );
	}

	void pfnGetGameDir( char *szGetGameDir )
	{
		strcpy( szGetGameDir, gEngine.GetGameDirectory().c_str() );
	}

	int pfnIsDedicatedServer( void )
	{
		return gEngine.GetIsDedicatedServer();
	}

	int pfnIsCareerMatch( void )
	{
		return gEngine.GetIsCareerMatch();
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