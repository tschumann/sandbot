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

		TEST_METHOD(TestIsValidEntity_NULL)
		{
			Assert::IsFalse( IsValidEntity( nullptr ) );
			Assert::IsFalse( IsValidEntity( NULL ) );
		}

		TEST_METHOD(TestIsValidEntity_Free)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();
			pPlayer->free = true;

			Assert::IsFalse( IsValidEntity( pPlayer ));
		}

		TEST_METHOD(TestIsValidEntity_Kill)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();
			pPlayer->v.flags &= FL_KILLME;

			Assert::IsFalse( IsValidEntity( pPlayer ) );
		}

		TEST_METHOD(Test_IsBuilt)
		{
			edict_t* pEdict = foolsgoldsource::gEngine.edicts[1].get();
			pEdict->v.iuser4 |= MASK_BUILDABLE;

			Assert::IsFalse( UTIL_IsBuilt( pEdict ) );

			pEdict->v.iuser4 &= ~MASK_BUILDABLE;

			Assert::IsTrue( UTIL_IsBuilt( pEdict ) );
		}
	};
}
