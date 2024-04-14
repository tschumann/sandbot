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
	GAME_UNKNOWN = 0,
	GAME_VALVE,
	GAME_GEARBOX,
	GAME_DECAY,
	GAME_DOD,
	GAME_TFC,
	GAME_DMC,
	GAME_RICOCHET,
	GAME_REWOLF,
	GAME_CRYOFFEAR,
	GAME_SVENCOOP,
	GAME_NS,
	GAME_HUNGER,
	GAME_SHIP
};

class Game
{
public:
	Game( GameId gameId );
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(const Game other) = delete;
	Game& operator=(Game&& other) = delete;
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

private:
	Game() {};
};

extern int mod_id;

extern std::unique_ptr<Game> pGame;

#endif // _GAME_H_
