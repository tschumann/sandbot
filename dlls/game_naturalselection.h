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

#ifndef __GAME_NATURALSELECTION_H__
#define __GAME_NATURALSELECTION_H__

#include "extdll.h"

#include "bot_ns.h"
#include "game.h"

class NSGame : public Game
{
public:
	NSGame( GameId gameId );
	NSGame(const NSGame&) = delete;
	NSGame(NSGame&&) = delete;
	NSGame& operator=(const NSGame other) = delete;
	NSGame& operator=(NSGame&& other) = delete;

	virtual const char* GetGameDir() const;

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

	bool IsClassic() const
	{
		return !this->IsCombat();
	}

	bool IsCombat() const
	{
		const char *szMap = STRING(gpGlobals->mapname);

		return (szMap[0] == 'c') && (szMap[1] == 'o');
	}

	edict_t **GetHives() const
	{
		return this->pHives;
	}

	void StartRound() const
	{
		for( int i = 0; i < Game::MAX_PLAYERS; i++ )
		{
			pBots[i]->not_started = true;

			// TODO: check that getting the team at this point works
			if( ((NSBot *)pBots[i])->IsAlien() )
			{
				pBots[i]->start_action = MSG_NS_JOIN_ALIEN;
			}
			else if( ((NSBot *)pBots[i])->IsMarine() )
			{
				pBots[i]->start_action = MSG_NS_JOIN_MARINE;
			}
		}
	}
private:
	edict_t *pCommandChair;
	edict_t **pHives;
};

#endif // __GAME_NATURALSELECTION_H__