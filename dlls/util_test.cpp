//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: utility test code
//
//=============================================================================

#include "base_test.h"
#include "bot.h"
#include "game_halflife.h"

namespace tests
{
	TEST_CLASS(util_test)
	{
	public:

		TEST_METHOD_INITIALIZE(SetUp)
		{
			Initialise();

			pGame = std::make_unique<ValveGame>(GameId::GAME_VALVE);
		}

		TEST_METHOD(TestUTIL_GetBotIndex_NullBot)
		{
			int iIndex = UTIL_GetBotIndex(nullptr);

			Assert::AreEqual( -1, iIndex );
		}

		TEST_METHOD(TestUTIL_UTIL_GetBotPointer_NullBot)
		{
			bot_t *pBot = UTIL_GetBotPointer(nullptr);

			Assert::IsNull( pBot );
		}

		TEST_METHOD(TestUTIL_ToLower)
		{
			char szTest[16] = "TEST";
			char *pTest = szTest;

			pTest = UTIL_ToLower(pTest);

			Assert::AreEqual( "test", pTest );
		}

		TEST_METHOD(TestUTIL_BuildFileName)
		{
			char szPath[256] = "";

			UTIL_BuildFileName(szPath, "maps", "map.bsp");

			Assert::AreEqual( "valve/maps/map.bsp", szPath );

			UTIL_BuildFileName(szPath, "maps", nullptr);

			Assert::AreEqual( "valve/maps", szPath );

			UTIL_BuildFileName(szPath, nullptr, nullptr);

			Assert::AreEqual( "valve/", szPath );
		}

		TEST_METHOD(TestIsAlive_Dead)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();
			pPlayer->v.deadflag = DEAD_DYING;
			pPlayer->v.health = 0;
			pPlayer->v.flags = 0;

			Assert::AreEqual( false, IsAlive( pPlayer ) );
		}

		TEST_METHOD(TestIsAlive_Alive)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();
			pPlayer->v.deadflag = DEAD_NO;
			pPlayer->v.health = 100;
			pPlayer->v.flags = 0;

			Assert::AreEqual( true, IsAlive( pPlayer ) );
		}

		TEST_METHOD(TestIsAlive_NoTarget)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();
			pPlayer->v.deadflag = DEAD_NO;
			pPlayer->v.health = 100;
			pPlayer->v.flags = FL_NOTARGET;

			Assert::AreEqual( false, IsAlive( pPlayer ) );
		}

		TEST_METHOD(TestIsValidEntity_NULL)
		{
			Assert::AreEqual( false, IsValidEntity( nullptr ) );
			Assert::AreEqual( false, IsValidEntity( NULL ) );
		}

		TEST_METHOD(TestIsValidEntity_Free)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();
			pPlayer->free = true;

			Assert::AreEqual( false, IsValidEntity( pPlayer ));
		}

		TEST_METHOD(TestIsValidEntity_Kill)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();
			pPlayer->v.flags |= FL_KILLME;

			Assert::AreEqual( false, IsValidEntity( pPlayer ) );
		}

		TEST_METHOD(Test_IsBuilt)
		{
			edict_t* pEdict = foolsgoldsource::gEngine.edicts[1].get();
			pEdict->v.iuser4 |= MASK_BUILDABLE;

			Assert::AreEqual( false, UTIL_IsBuilt( pEdict ) );

			pEdict->v.iuser4 &= ~MASK_BUILDABLE;

			Assert::AreEqual( true, UTIL_IsBuilt( pEdict ) );
		}
	};
}
