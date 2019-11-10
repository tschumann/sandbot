#include "CppUnitTest.h"

#include "h_export.h"
#include "bot.h"
#include "foolsgoldsource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	TEST_CLASS(bot_test)
	{
	public:

		TEST_METHOD(TestGetBotCountNoBots)
		{
			enginefuncs_t engineFunctions = foolsgoldsource::gEngine.GetServerEngineFunctions();
			globalvars_t globalVariables = foolsgoldsource::gEngine.GetServerGlobalVariables();

			g_engfuncs = engineFunctions;
			gpGlobals = &globalVariables;

			Assert::AreEqual(GetBotCount(), 0);
		}
	};
}
