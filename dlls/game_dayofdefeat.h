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

#ifndef __GAME_DAYOFDEFEAT_H__
#define __GAME_DAYOFDEFEAT_H__

#include "extdll.h"

#include "bot.h"
#include "game.h"

class DODGame : public Game
{
public:
	DODGame( GameId gameId );
	DODGame(const DODGame&) = delete;
	DODGame(DODGame&&) = delete;
	DODGame& operator=(const DODGame other) = delete;
	DODGame& operator=(DODGame&& other) = delete;

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

	virtual bool AreAlliesBritish() const
	{
		extern int g_iAlliesCountry;

		return g_iAlliesCountry == DODGame::ALLIES_COUNTRY_BRITISH;
	}

	virtual int GetTeam( const edict_t *pEdict ) const;

	// in info_doddetect entity's detect_allies_country property
	const static int ALLIES_COUNTRY_UNITED_STATES = 0;
	const static int ALLIES_COUNTRY_BRITISH = 1;
};

#endif // __GAME_DAYOFDEFEAT_H__