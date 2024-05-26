//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: bot test
//
//=============================================================================

#include "CppUnitTest.h"

#include "h_export.h"
#include "bot.h"
#include "dll.h"
#include "game.h"
#include "foolsgoldsource/foolsgoldsource.h"
#include "foolsgoldsource/vscu_test.h"

namespace tests
{
	TEST_CLASS(bot_test)
	{
	public:

		TEST_METHOD_INITIALIZE(SetUp)
		{
			foolsgoldsource::gEngine.Reset();

			CleanupGameAndBots();

			pGame = std::make_unique<Game>(GameId::GAME_VALVE);
			other_gFunctionTable = *foolsgoldsource::gEngine.GetDLLFunctions();
			ServerActivate( nullptr, 2048, 32 );
			// TODO: should be set by calling GiveFnptrsToDll
			pBotData = g_valveBots;

			Assert::IsNotNull( pGame.get() );
			Assert::IsNotNull( pBots );
		}

		TEST_METHOD_CLEANUP(TearDown)
		{
			CleanupGameAndBots();
		}

		TEST_METHOD(TestGetBotCount_NoBots)
		{
			Assert::AreEqual( 0, GetBotCount() );
		}

		TEST_METHOD(TestGetBotCount_OneBot)
		{
			edict_t* pPlayer = foolsgoldsource::gEngine.edicts[1].get();

			pPlayer->v.netname = ALLOC_STRING("test");

			Assert::AreEqual( 1, GetBotCount() );
		}

		TEST_METHOD(TestKickBot_NoBots)
		{
			CleanupGameAndBots();

			KickBot( 1 );
		}

		TEST_METHOD(TestKickBot)
		{
			// TODO: should be set by calling BotCreate
			pBots[0]->iBotDataIndex = 0;
			pBots[0]->is_used = true;
			strncpy( pBots[0]->name, "BotName", bot_t::BOT_NAME_MAX_LENGTH );

			Assert::AreEqual( true, pBots[0]->is_used );
			Assert::AreEqual( false, pBotData[pBots[0]->iBotDataIndex].bIsUsed );

			KickBot( 0 );

			Assert::AreEqual( false, pBots[0]->is_used );
			Assert::AreEqual( false, pBotData[pBots[0]->iBotDataIndex].bIsUsed );
			Assert::AreEqual( (size_t)1, foolsgoldsource::gEngine.executedServerCommands.size() );
			Assert::AreEqual( "kick \"BotName\"\n", foolsgoldsource::gEngine.executedServerCommands.back().c_str() );
		}
	};
}
