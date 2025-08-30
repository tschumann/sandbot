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

#include "base_test.h"
#include "bot.h"
#include "game_halflife.h"

namespace tests
{
	TEST_CLASS(game_test)
	{
	public:

		TEST_METHOD_INITIALIZE(SetUp)
		{
			Initialise();

			pGame = std::make_unique<ValveGame>(GameId::GAME_VALVE);
		}

		TEST_METHOD(TestIsValidEdict)
		{
			Assert::AreEqual( false, pGame->IsValidEdict( nullptr ) );
		}
	};
}
