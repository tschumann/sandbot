#ifndef _STUB_ENGINE_H_
#define _STUB_ENGINE_H_

#include <cstring>
#include <cctype>
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
using std::vector;

namespace foolsgoldsource
{
	struct event_t
	{
		unsigned short iIndex;
		string strEventFileName;
		int iType;
	};

	class Engine
	{
	public:
		Engine();
		~Engine();

		const enginefuncs_t GetServerEngineFunctions();
		const globalvars_t GetServerGlobalVariables();
		const DLL_FUNCTIONS GetDLLFunctions();
		const NEW_DLL_FUNCTIONS GetNewDLLFunctions();

		const string GetGameDirectory();
		void SetGameDirectory( const string& strGameDir );
		bool GetIsDedicatedServer();
		void SetIsDedicatedServer( const bool bIsDedicatedServer );
		bool GetIsCareerMatch();
		void SetIsCareerMatch( const bool bIsCareerMatch);

		void SetMaxClients( const unsigned int iMaxClients );

		// below shouldn't be public because the game doesn't have access to them

		// TODO: below be in some server struct?
		vector<shared_ptr<edict_t>> edicts;
		vector<string> models;
		vector<string> sounds;
		vector<event_t> events;
		int iMaxEdicts;
		vector<string> serverCommands;

		// TODO: how does the engine track this?
		unsigned int iStringTableOffset;

		const static unsigned int iStringTableSize = 2048;
	private:
		enginefuncs_t engineFunctions;
		globalvars_t globalVariables;
		DLL_FUNCTIONS dllFunctions;
		NEW_DLL_FUNCTIONS newDllFunctions;

		string strGameDir;

		bool bIsDedicatedServer;
		bool bIsCareerMatch;
	};

	class Util
	{
	public:
		static string tolowercase( const string& str );
	};

	extern Engine gEngine;

	// enginefuncs_t
	int pfnPrecacheModel( char* s );
	int pfnPrecacheSound( char* s );
	void pfnSetModel( edict_t* e, const char* m );
	int pfnModelIndex( const char* m );
	int pfnModelFrames( int modelIndex );
	void pfnSetSize( edict_t* e, const float* rgflMin, const float* rgflMax );
	void pfnChangeLevel( char* s1, char* s2 );
	void pfnGetSpawnParms( edict_t* ent );
	void pfnSaveSpawnParms( edict_t* ent );
	float pfnVecToYaw( const float* rgflVector );
	void pfnVecToAngles( const float* rgflVectorIn, float* rgflVectorOut );
	void pfnMoveToOrigin( edict_t* ent, const float* pflGoal, float dist, int iMoveType );
	void pfnChangeYaw( edict_t* ent );
	void pfnChangePitch( edict_t* ent );
	edict_t* pfnFindEntityByString( edict_t* pEdictStartSearchAfter, const char* pszField, const char* pszValue );
	int pfnGetEntityIllum( edict_t* pEnt );
	edict_t* pfnFindEntityInSphere( edict_t* pEdictStartSearchAfter, const float* org, float rad );
	edict_t* pfnFindClientInPVS( edict_t* pEdict );
	edict_t* pfnEntitiesInPVS( edict_t* pplayer );

	void pfnSetOrigin( edict_t* e, const float* rgflOrigin );
	void pfnEmitSound( edict_t* entity, int channel, const char* sample, float volume, float attenuation, int fFlags, int pitch );
	void pfnEmitAmbientSound( edict_t* entity, float* pos, const char* samp, float vol, float attenuation, int fFlags, int pitch );

	void pfnTraceSphere( const float* v1, const float* v2, int fNoMonsters, float radius, edict_t* pentToSkip, TraceResult* ptr );

	void pfnServerCommand( char* str );

	void pfnParticleEffect( const float* org, const float* dir, float color, float count );
	void pfnLightStyle( int style, char* val );

	int32 pfnRandomLong( int32 lLow, int32 lHigh );

	void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... );

	int pfnAllocString( const char* szValue );

	edict_t* pfnPEntityOfEntOffset( int iEntOffset );

	edict_t* pfnPEntityOfEntIndex( int iEntIndex );

	void pfnServerPrint( const char* szMsg );

	void pfnGetGameDir( char *szGetGameDir );

	int pfnIsDedicatedServer( void );

	unsigned short pfnPrecacheEvent( int type, const char* psz );
	void pfnPlaybackEvent( int flags, const edict_t* pInvoker, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );

	int pfnIsCareerMatch( void );

	void pfnQueryClientCvarValue( const edict_t* player, const char* cvarName );
	void pfnQueryClientCvarValue2( const edict_t* player, const char* cvarName, int requestID );
	int pfnCheckParm( const char* pchCmdLineToken, char** ppnext );
	edict_t* pfnPEntityOfEntIndexAllEntities( int iEntIndex );

	// DLL_FUNCTIONS
	void ServerActivate( edict_t* pEdictList, int edictCount, int clientMax );
}

#endif // _STUB_ENGINE_H_