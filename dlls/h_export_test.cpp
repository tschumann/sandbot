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

#include "CppUnitTest.h"

#include "h_export.h"
#include "bot.h"
#include "game.h"
#include "foolsgoldsource/foolsgoldsource.h"
#include "foolsgoldsource/vscu_test.h"

namespace tests
{		
	TEST_CLASS(h_export_test)
	{
	public:
		
		TEST_METHOD(TestGiveFnptrsToDllValve)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory( "valve" );

			GiveFnptrsToDll( &engineFunctions, &globalVariables );

			Assert::IsTrue(pGame->IsHalfLife());
		}

		TEST_METHOD(TestGiveFnptrsToDllOpfor)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory( "gearbox" );

			GiveFnptrsToDll(&engineFunctions, &globalVariables);

			Assert::IsTrue(pGame->IsOpposingForce());
		}
	};
}
