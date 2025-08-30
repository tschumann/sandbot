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

#include "base_test.h"
#include "bot_gunman.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "game_gunmanchronicles.h"

namespace tests
{
	TEST_CLASS(bot_combat_test)
	{
	public:

		TEST_METHOD_INITIALIZE(SetUp)
		{
			Initialise();

			pGame = std::make_unique<RewolfGame>(GameId::GAME_REWOLF);
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
