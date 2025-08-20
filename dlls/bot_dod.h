//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Day of Defeat specific bot code
//
//=============================================================================

#ifndef __BOT_DOD_H__
#define __BOT_DOD_H__

#include "bot.h"
#include "game.h"

class DODBot : public bot_t
{
public:
	DODBot();

	virtual void Join();

	virtual float GetSpeedToEnemy() const;

	virtual bool IsSniping() const;

	virtual float GetMaxSpeed() const;
	virtual float GetSpeed();

	virtual bool IsSniper();

	virtual bool ShouldLookForNewGoal();
	virtual int GetGoalType();

	virtual bool ShouldCapturePoint(edict_t* pControlPoint);

	const static int TEAM_ALLIES = 1;
	const static int TEAM_AXIS = 2;

	const static int PANZERJAGER_AXIS = 19;

	const static int RIFLEMAN_BRITISH = 21;
	const static int SERGEANT_MAJOR_BRITISH = 22;
	const static int PIAT_BRITISH = 25;
};

#endif // __BOT_DOD_H__