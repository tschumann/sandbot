//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Team Fortress Classic specific bot code
//
//=============================================================================

#ifndef __BOT_TFC_H__
#define __BOT_TFC_H__

#include "bot.h"
#include "game.h"

class TFCBot : public bot_t
{
public:
	TFCBot();

	virtual void Join();

	virtual bool CanHeal() const;

	virtual int GetHealingWeapon() const;

	virtual bool IsValidEnemy(const edict_t* pEnemy);

	virtual bool IsSniping() const;

	virtual int GetGoalType();

	virtual bool IsEngineer();
	virtual bool IsSniper();

	virtual bool HasFlag() const;
	virtual bool FindFlag();

	virtual bool ShouldBuild();
	virtual void Build();
	virtual bool CanBuildDispenser();
	virtual bool CanBuildSentryGun();

	virtual edict_t* FindEnemy();

	const static int CLASS_CIVILIAN = 0;
	const static int CLASS_SCOUT = 1;
	const static int CLASS_SNIPER = 2;
	const static int CLASS_SOLDIER = 3;
	const static int CLASS_DEMOMAN = 4;
	const static int CLASS_MEDIC = 5;
	const static int CLASS_HWGUY = 6;
	const static int CLASS_PYRO = 7;
	const static int CLASS_SPY = 8;
	const static int CLASS_ENGINEER = 9;

	const static int TEAM_BLUE = 0xA096;
	const static int TEAM_RED = 0x04FA;
	const static int TEAM_YELLOW = 0x372D;
	const static int TEAM_GREEN = 0x6E64;
};

#endif // __BOT_TFC_H__