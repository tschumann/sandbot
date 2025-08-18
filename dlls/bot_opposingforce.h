//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Opposing Force specific bot code
//
//=============================================================================

#ifndef __BOT_OPPOSINGFORCE_H__
#define __BOT_OPPOSINGFORCE_H__

#include "bot_halflife.h"
#include "game.h"

class OpposingForceBot : public HalfLifeBot
{
public:
	OpposingForceBot();

	virtual void Join();
	virtual void PreThink();

	virtual bool ShouldSeekEnemy() const;

	virtual int GetPistol();

	virtual int GetGoalType();

	virtual bool HasFlag() const;
	virtual bool FindFlag();

	virtual bool ShouldCapturePoint(edict_t* pControlPoint);

	const static int TEAM_BLACK_MESA = 0;
	const static int TEAM_OPPOSING_FORCE = 1;

	const static int BLACK_MESA_FLAG_SKIN = 1;
	const static int OPPOSING_FORCE_FLAG_SKIN = 2;
};

#endif // __BOT_OPPOSINGFORCE_H__