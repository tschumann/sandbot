//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// http://www.teamsandpit.com/
//
// Notes:
//
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

class Game
{
public:
	virtual ~Game();

	virtual void Cleanup();
	virtual int GetMaxPlayers();
	virtual bool CanAddBots();
	virtual bool IsTeamPlay();
	virtual bool IsDeathmatch();
	virtual bool IsCTF();
	virtual bool IsCapturePoint();
	virtual unsigned int BotsOnTeam( int team );
	virtual bool IsValidEdict( edict_t *pEdict );
	virtual bool CanChoosePlayerModel();
	virtual int GetTeam( edict_t *pEdict );
	virtual bool UseToOpenDoor();
	virtual void GetSaveGameComment( char *pBuffer, int iMaxLength );

	virtual bool IsGunmanChronicles();
};

class ValveGame : public Game
{
public:
	virtual void GetSaveGameComment( char *pBuffer, int iMaxLength );
};

class GearboxGame : public Game
{
public:
	virtual void Cleanup()
	{
		for( int i = 0; i < OpposingForceBot::MAX_CAPTURE_POINTS; i++ )
		{
			extern CapturePoint capturePoints[OpposingForceBot::MAX_CAPTURE_POINTS];
			capturePoints[i].iTeam = 0;
			capturePoints[i].szName = nullptr;
			capturePoints[i].szTarget = nullptr;
			capturePoints[i].pEdict = nullptr;
		}
	}

	virtual bool IsTeamPlay()
	{
		return this->IsCTF() || this->IsCapturePoint() || Game::IsTeamPlay();
	}

	virtual bool IsDeathmatch()
	{
		return !this->IsCTF() && !this->IsCapturePoint();
	}

	virtual bool IsCTF()
	{
		extern edict_t *pent_info_ctfdetect;
		extern bool bIsCapturePoint;

		// info_ctfdetect will be present in op4cp_ maps too
		return pent_info_ctfdetect != nullptr && !bIsCapturePoint;
	}

	virtual bool IsCapturePoint()
	{
		extern bool bIsCapturePoint;

		return bIsCapturePoint;
	}

	virtual bool CanChoosePlayerModel()
	{
		return !this->IsCTF() && !this->IsCapturePoint();
	}

	virtual int GetTeam( edict_t *pEdict );
};

class DecayGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}

	virtual bool IsDeathmatch()
	{
		return false;
	}

	virtual bool CanChoosePlayerModel()
	{
		return false;
	}
};

class CStrikeGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}

	virtual bool IsDeathmatch()
	{
		return false;
	}

	virtual bool CanChoosePlayerModel()
	{
		return false;
	}

	virtual int GetTeam( edict_t *pEdict );
};

class DODGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}

	virtual bool IsDeathmatch()
	{
		return false;
	}

	virtual bool CanChoosePlayerModel()
	{
		return false;
	}

	virtual bool AreAlliesBritish()
	{
		extern int g_iAlliesCountry;

		return g_iAlliesCountry == DODGame::ALLIES_COUNTRY_BRITISH;
	}

	virtual int GetTeam( edict_t *pEdict );

	// in info_doddetect entity's detect_allies_country property
	const static int ALLIES_COUNTRY_UNITED_STATES = 0;
	const static int ALLIES_COUNTRY_BRITISH = 1;
};

class TFCGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}

	virtual bool IsDeathmatch()
	{
		return false;
	}

	virtual bool CanChoosePlayerModel()
	{
		return false;
	}

	virtual int GetTeam( edict_t *pEdict );
};

class NSGame : public Game
{
public:
	virtual bool IsTeamPlay()
	{
		return true;
	}

	virtual bool IsDeathmatch()
	{
		return false;
	}

	virtual bool CanChoosePlayerModel()
	{
		return false;
	}

	virtual int GetTeam( edict_t *pEdict );

	bool IsClassic()
	{
		return !this->IsCombat();
	}

	bool IsCombat()
	{
		const char *szMap = STRING(gpGlobals->mapname);

		return (szMap[0] == 'c') && (szMap[1] == 'o');
	}

	edict_t **GetHives()
	{
		return this->pHives;
	}

	void StartRound()
	{
		for( int i = 0; i < MAX_PLAYERS; i++ )
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
	virtual bool UseToOpenDoor()
	{
		return true;
	}
};

extern std::unique_ptr<Game> pGame;

#endif // _GAME_H_
