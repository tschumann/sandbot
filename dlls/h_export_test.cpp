#include "stub_engine.h"

extern int mod_id;
extern bool g_bIsMMPlugin;

void reset()
{
	mod_id = 0;
	g_bIsMMPlugin = false;
}

// now it's happy without the "C": sense - this makes none
extern void WINAPI GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals );

void testGiveFnptrsToDll()
{
	enginefuncs_t engineFunctions = stub_engine::gEngine.GetServerEngineFunctions();
	globalvars_t globalVariables = stub_engine::gEngine.GetServerGlobalVariables();

	GiveFnptrsToDll( &engineFunctions, &globalVariables );

	stub_engine::assertTrue( mod_id == VALVE_DLL, "mod_id is set correctly" );
}