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

#include <memory>

#include "bot_weapons.h"

enum class GameId
{
	GAME_UNKNOWN = 0,
	GAME_VALVE,
	GAME_GEARBOX,
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

// derive this class to add support for other games
// TODO: make more of these methods abstract - this should really be a mostly abstract class with generic functionality
class Game
{
public:
	Game( GameId gameId );
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(const Game other) = delete;
	Game& operator=(Game&& other) = delete;
	virtual ~Game();

	virtual const char* GetGameDir() const = 0;

	virtual void Cleanup();
	virtual int GetMaxPlayers() const;
	virtual bool CanAddBots() const;
	virtual bool IsTeamPlay() const;
	virtual bool IsDeathmatch() const;
	virtual bool IsCTF() const;
	virtual bool IsCapturePoint() const;
	virtual unsigned int BotsOnTeam( const int team ) const;
	virtual bool IsValidEdict( const edict_t *pEdict ) const;
	virtual bool CanChoosePlayerModel() const = 0;
	virtual int GetTeam( const edict_t *pEdict ) const;
	virtual bool UseToOpenDoor() const;
	// virtual bot_weapon_select_t GetWeapons() const;
	// virtual bot_player_t GetBotProfiles() const;
	// virtual int GetInitialBotCount() const;
	virtual bool HasWeaponCustomisation() const;
	virtual void GetSaveGameComment( char *pBuffer, int iMaxLength ) const;

	// TODO: get rid of these - all logic should come from the game class not if statements throughout the code
	virtual bool IsHalfLife() const;
	virtual bool IsOpposingForce() const;
	virtual bool IsDayOfDefeat() const;
	virtual bool IsTeamFortressClassic() const;
	virtual bool IsDeathmatchClassic() const;
	virtual bool IsGunmanChronicles() const;
	virtual bool IsSvenCoop() const;
	virtual bool IsNaturalSelection() const;
	virtual bool IsTheShip() const;
	virtual bool IsTheyHunger() const;

	const static int MAX_PLAYERS = 32;

protected:
	const GameId gameId;

private:
	Game() : gameId(GameId::GAME_UNKNOWN) {};
};

extern std::unique_ptr<Game> pGame;

#endif // _GAME_H_
