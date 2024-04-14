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

#ifndef _GAME_H_
#define _GAME_H_

enum class GameId
{
	UNKNOWN = 0,
	VALVE,
	BSHIFT,
	GEARBOX,
	DECAY,
	CSTRIKE,
	CZERO,
	CZEROR,
	DOD,
	TFC,
	REWOLF,
	SVENCOOP,
	NS,
	HUNGER,
	SHIP
};

class Game
{
public:
	Game( GameId gameId );
	virtual ~Game();

	virtual void Cleanup();
	virtual int GetMaxPlayers() const;
	virtual bool CanAddBots() const;
	virtual bool IsTeamPlay() const;
	virtual bool IsDeathmatch() const;
	virtual bool IsCTF() const;
	virtual bool IsCapturePoint() const;
	virtual unsigned int BotsOnTeam( const int team ) const;
	virtual bool IsValidEdict( const edict_t *pEdict ) const;
	virtual bool CanChoosePlayerModel() const;
	virtual int GetTeam( const edict_t *pEdict ) const;
	virtual bool UseToOpenDoor() const;
	virtual void GetSaveGameComment( char *pBuffer, int iMaxLength ) const;

	virtual bool IsGunmanChronicles() const;

	const static int MAX_PLAYERS = 32;

protected:
	GameId gameId;
};

class ValveGame : public Game
{
public:
	ValveGame( GameId gameId );

	virtual void GetSaveGameComment( char *pBuffer, int iMaxLength ) const;
};

class GearboxGame : public Game
{
public:
	GearboxGame( GameId gameId );

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

class DODGame : public Game
{
public:
	DODGame( GameId gameId );

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

class TFCGame : public Game
{
public:
	TFCGame( GameId gameId );

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

class NSGame : public Game
{
public:
	NSGame( GameId gameId );

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

class ShipGame : public Game
{
public:
	ShipGame( GameId gameId );

	virtual bool UseToOpenDoor() const
	{
		return true;
	}
};

extern int mod_id;

extern std::unique_ptr<Game> pGame;

#endif // _GAME_H_
