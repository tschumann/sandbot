//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: game rules code
//
//=============================================================================

#ifndef __GAME_OPPOSINGFORCE_H__
#define __GAME_OPPOSINGFORCE_H__

#include "extdll.h"

#include "bot_opposingforce.h"
#include "game.h"

class GearboxGame : public Game
{
public:
	GearboxGame( GameId gameId );
	GearboxGame(const GearboxGame&) = delete;
	GearboxGame(GearboxGame&&) = delete;
	GearboxGame& operator=(const GearboxGame other) = delete;
	GearboxGame& operator=(GearboxGame&& other) = delete;

	virtual const char* GetGameDir() const;

	virtual void Cleanup()
	{
		extern vector<CapturePoint> capturePoints;
		capturePoints.clear();
	}

	virtual bool IsTeamPlay() const
	{
		return this->IsCTF() || this->IsCapturePoint() || Game::IsTeamPlay();
	}

	virtual bool IsDeathmatch() const
	{
		return !this->IsCTF() && !this->IsCapturePoint();
	}

	virtual bool IsCTF() const
	{
		extern edict_t *pent_info_ctfdetect;
		extern bool bIsCapturePoint;

		// info_ctfdetect will be present in op4cp_ maps too
		return pent_info_ctfdetect != nullptr && !bIsCapturePoint;
	}

	virtual bool IsCapturePoint() const
	{
		extern bool bIsCapturePoint;

		return bIsCapturePoint;
	}

	virtual bool CanChoosePlayerModel() const
	{
		return !this->IsCTF() && !this->IsCapturePoint();
	}

	virtual int GetTeam( const edict_t *pEdict ) const;
};

#endif // __GAME_OPPOSINGFORCE_H__