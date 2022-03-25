//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// http://www.teamsandpit.com/
//
// Notes:
//
//=============================================================================

#include "CppUnitTest.h"

#include "h_export.h"
#include "bot.h"
#include "foolsgoldsource.h"
#include "test.h"

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

			Assert::AreEqual( GetModId(), VALVE_DLL );
		}

		TEST_METHOD(TestGiveFnptrsToDllOpfor)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory( "gearbox" );

			GiveFnptrsToDll(&engineFunctions, &globalVariables);

			Assert::AreEqual( GetModId(), GEARBOX_DLL );
		}
	};
}
