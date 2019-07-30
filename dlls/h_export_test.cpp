#include "CppUnitTest.h"

#include "h_export.h"
#include "foolsgoldsource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(h_export_test)
	{
	public:
		
		TEST_METHOD(TestGiveFnptrsToDll)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			GiveFnptrsToDll( &engineFunctions, &globalVariables );

			Assert::AreEqual( GetModId(), VALVE_DLL );
		}
	};
}
