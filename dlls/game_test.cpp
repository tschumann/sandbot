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

#include "extdll.h"
#include "bot.h"
#include "game.h"
#include "foolsgoldsource.h"
#include "test.h"

namespace tests
{
	TEST_CLASS(game_test)
	{
	public:

		TEST_METHOD(TestIsValidEdict)
		{
			std::unique_ptr<Game> pGame = std::make_unique<Game>();
			Assert::AreEqual( false, pGame->IsValidEdict( nullptr ) );
		}
	};
}
