//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: common test code
//
//=============================================================================

#include "base_test.h"
#include "bot.h"
#include "dll.h"
#include "h_export.h"

namespace tests
{
	/// <summary>
	/// Shared Initialise method to set all of the global variables to known good states.
	/// Ideally this would be a class that the various tests inherit, but Microsoft's TEST_CLASS doesn't seem designed to support inheritance.
	/// </summary>
	void Initialise()
	{
		foolsgoldsource::gEngine.Initialise();

		CleanUpBots();

		const DLL_FUNCTIONS* dllFunctions = foolsgoldsource::gEngine.GetDLLFunctions();
		gGameDLLFunc.dllapi_table = const_cast<DLL_FUNCTIONS*>(dllFunctions);
		gpGamedllFuncs = &gGameDLLFunc;

		other_gFunctionTable = *dllFunctions;

		g_bIsMMPlugin = false;

		pGame.release();

		// set cvars to sane defaults
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
}