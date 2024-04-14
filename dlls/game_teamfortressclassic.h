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

#ifndef __GAME_TEAMFORTRESSCLASSIC_H__
#define __GAME_TEAMFORTRESSCLASSIC_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class TFCGame : public Game
{
public:
	TFCGame( GameId gameId );
	TFCGame(const TFCGame&) = delete;
	TFCGame(TFCGame&&) = delete;
	TFCGame& operator=(const TFCGame other) = delete;
	TFCGame& operator=(TFCGame&& other) = delete;

	virtual bool IsTeamPlay() const
	{
		return true;
	}

	virtual bool IsDeathmatch() const
	{
		return false;
	}

	virtual bool CanChoosePlayerModel() const
	{
		return false;
	}

	virtual int GetTeam( const edict_t *pEdict ) const;

	const static int MAX_BACKPACKS = 100;
};

#endif // __GAME_TEAMFORTRESSCLASSIC_H__