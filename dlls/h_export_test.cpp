#include "CppUnitTest.h"

#include "h_export.h"
#include "bot.h"
#include "foolsgoldsource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(h_export_test)
	{
	public:
		
		TEST_METHOD(TestValveGiveFnptrsToDll)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory( "valve" );

			GiveFnptrsToDll( &engineFunctions, &globalVariables );

			Assert::AreEqual( GetModId(), VALVE_DLL );
		}

		TEST_METHOD(TestOpforGiveFnptrsToDll)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			foolsgoldsource::gEngine.SetGameDirectory( "gearbox" );

			GiveFnptrsToDll(&engineFunctions, &globalVariables);

			Assert::AreEqual( GetModId(), GEARBOX_DLL );
		}
	};
}
