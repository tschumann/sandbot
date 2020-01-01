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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	TEST_CLASS(util_test)
	{
	public:

		TEST_METHOD(TestUTIL_ToLower)
		{
			char szTest[16] = "TEST";
			char *pTest = szTest;

			pTest = UTIL_ToLower(pTest);

			Assert::AreEqual( pTest, "test" );
		}
	};
}
