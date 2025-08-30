//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: engine hooking test code
//
//=============================================================================

#include "base_test.h"
#include "h_export.h"
#include "bot.h"
#include "game.h"

namespace tests
{		
	TEST_CLASS(h_export_test)
	{
	public:

		TEST_METHOD_INITIALIZE(SetUp)
		{
			Initialise();
		}

		TEST_METHOD(TestGetEngineFunctionsWithoutMetamod)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();

			int iInterfaceVersion = 140;

			int iResult = GetEngineFunctions( &engineFunctions, &iInterfaceVersion );

			Assert::IsTrue( iResult );
		}

		TEST_METHOD(TestGetEngineFunctionsWithMetamod)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();

			int iInterfaceVersion = 140;

			g_bIsMMPlugin = true;

			int iResult = GetEngineFunctions( &engineFunctions, &iInterfaceVersion );

			Assert::IsTrue( iResult );
		}

		TEST_METHOD(TestGiveFnptrsToDllValveWithoutMetaMod)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory( "valve" );

			GiveFnptrsToDll( &engineFunctions, &globalVariables );

			Assert::IsTrue(pGame->IsHalfLife());
		}

		TEST_METHOD(TestGiveFnptrsToDllValveWithMetaMod)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory("valve");

			g_bIsMMPlugin = true;

			GiveFnptrsToDll( &engineFunctions, &globalVariables );

			Assert::IsTrue(pGame->IsHalfLife());
		}

		TEST_METHOD(TestGiveFnptrsToDllOpforWithoutMetaMod)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory( "gearbox" );

			GiveFnptrsToDll( &engineFunctions, &globalVariables );

			Assert::IsTrue(pGame->IsOpposingForce());
		}
	};
}
