//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Gunman Chronicles bot test code
//
//=============================================================================

#include "base_test.h"
#include "bot_gunman.h"
#include "bot_func.h"
#include "game.h"

namespace tests
{
	TEST_CLASS(bot_gunman_test)
	{
	public:

		TEST_METHOD_INITIALIZE(SetUp)
		{
			Initialise();
		}

		TEST_METHOD(TestUseGaussPistolSniper_NotAllowed)
		{
			unsigned int iCurrentCallsToClientCommand = foolsgoldsource::gEngine.iCallsToClientCommand;
			std::unique_ptr<GunmanBot> pBot = std::make_unique<GunmanBot>();
			pBot->pEdict = foolsgoldsource::gEngine.edicts[1].get();
			BotSpawnInit(pBot.get());
			bot_use_sniper.value = 0;
			pBot->UseGaussPistolSniper();
			// TODO: test something better, like what command was sent
			Assert::AreEqual(iCurrentCallsToClientCommand, foolsgoldsource::gEngine.iCallsToClientCommand);
		}

		TEST_METHOD(TestUseGaussPistolSniper_Allowed)
		{
			unsigned int iCurrentCallsToClientCommand = foolsgoldsource::gEngine.iCallsToClientCommand;
			std::unique_ptr<GunmanBot> pBot = std::make_unique<GunmanBot>();
			pBot->pEdict = foolsgoldsource::gEngine.edicts[1].get();
			BotSpawnInit(pBot.get());
			bot_use_sniper.value = 1;
			pBot->UseGaussPistolSniper();
			// TODO: test something better, like what command was sent
			Assert::AreEqual(iCurrentCallsToClientCommand + 1, foolsgoldsource::gEngine.iCallsToClientCommand);
		}
	};
}
