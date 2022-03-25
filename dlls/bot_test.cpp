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
	TEST_CLASS(bot_test)
	{
	public:

		TEST_METHOD(TestGetBotCountNoBots)
		{
			Assert::AreEqual( GetBotCount(), 0 );
		}

		TEST_METHOD(TestGetBotCountOneBot)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();

			pPlayer->v.netname = ALLOC_STRING("test");

			Assert::AreEqual( GetBotCount(), 1 );
		}
	};
}
