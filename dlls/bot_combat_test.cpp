//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: bot combat test
//
//=============================================================================

#include "CppUnitTest.h"

#include "extdll.h"
#include "bot_gunman.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "dll.h"
#include "game_gunmanchronicles.h"
#include "h_export.h"
#include "foolsgoldsource/foolsgoldsource.h"
#include "foolsgoldsource/vscu_test.h"

namespace tests
{
	TEST_CLASS(bot_combat_test)
	{
	public:

		TEST_METHOD_INITIALIZE(SetUp)
		{
			foolsgoldsource::gEngine.Reset();

			CleanUpBots();

			g_bIsMMPlugin = false;

			const DLL_FUNCTIONS* dllFunctions = foolsgoldsource::gEngine.GetDLLFunctions();
			gGameDLLFunc.dllapi_table = const_cast<DLL_FUNCTIONS*>(dllFunctions);
			gpGamedllFuncs = &gGameDLLFunc;

			pGame = std::make_unique<RewolfGame>(GameId::GAME_REWOLF);

			bot_use_melee.value = 1;
			bot_use_pistol.value = 1;
			bot_use_rangedpistol.value = 1;
			bot_use_shotgun.value = 1;
			bot_use_machinegun.value = 1;
			bot_use_rifle.value = 1;
			bot_use_sniper.value = 1;
			bot_use_rocketlauncher.value = 1;
			bot_use_energy.value = 1;
			bot_use_organic.value = 1;
			bot_use_grenade.value = 1;
			bot_use_chemical.value = 1;
		}

		TEST_METHOD(TestBotFireWeapon_GaussPistol)
		{
			unsigned int iCurrentCallsToClientCommand = foolsgoldsource::gEngine.iCallsToClientCommand;
			std::unique_ptr<GunmanBot> pBot = std::make_unique<GunmanBot>();
			pBot->pEdict = foolsgoldsource::gEngine.edicts[1].get();
			BotSpawnInit(pBot.get());
			pBot->pEdict->v.weapons |= (1 << GUNMAN_WEAPON_GAUSSPISTOL);
			pBot->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_GAUSSPISTOL].iAmmo1] = 50;

			BotFireWeapon(Vector(0, 0, 0), pBot.get(), GUNMAN_WEAPON_GAUSSPISTOL);
			// TODO: test something better, like what command was sent
			Assert::AreEqual(iCurrentCallsToClientCommand + 2, foolsgoldsource::gEngine.iCallsToClientCommand);
		}

		TEST_METHOD(TestBotFireWeapon_Shotgun)
		{
			unsigned int iCurrentCallsToClientCommand = foolsgoldsource::gEngine.iCallsToClientCommand;
			std::unique_ptr<GunmanBot> pBot = std::make_unique<GunmanBot>();
			pBot->pEdict = foolsgoldsource::gEngine.edicts[1].get();
			BotSpawnInit(pBot.get());
			pBot->pEdict->v.weapons |= (1 << GUNMAN_WEAPON_SHOTGUN);
			pBot->m_rgAmmo[weapon_defs[GUNMAN_WEAPON_SHOTGUN].iAmmo1] = 50;

			BotFireWeapon(Vector(0, 0, 0), pBot.get(), GUNMAN_WEAPON_SHOTGUN);
			// TODO: test something better, like what command was sent
			Assert::AreEqual(iCurrentCallsToClientCommand + 1, foolsgoldsource::gEngine.iCallsToClientCommand);
		}
	};
}
