//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: game rules test code
//
//=============================================================================

#include "CppUnitTest.h"

#include "extdll.h"
#include "bot.h"
#include "game.h"
#include "foolsgoldsource/foolsgoldsource.h"
#include "foolsgoldsource/vscu_test.h"

namespace tests
{
	TEST_CLASS(game_test)
	{
	public:

		TEST_METHOD(TestIsValidEdict)
		{
			std::unique_ptr<Game> pGame = std::make_unique<Game>(GameId::VALVE);
			Assert::AreEqual( false, pGame->IsValidEdict( nullptr ) );
		}
	};
}
