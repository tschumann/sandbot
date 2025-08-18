//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Gunman Chronicles specific bot code
//
//=============================================================================

#ifndef __BOT_GUNMAN_H__
#define __BOT_GUNMAN_H__

#include "bot.h"
#include "game.h"

class GunmanBot : public bot_t
{
public:
	GunmanBot();

	virtual void OnSpawn();
	virtual void Think();

	virtual bool ShouldReload() const;

	virtual bool IsSniping() const;

	virtual int GetPistolMode();

	virtual void UseGaussPistolPulse() const;
	virtual void UseGaussPistolCharge() const;
	virtual void UseGaussPistolRapid() const;
	virtual void UseGaussPistolSniper() const;

	virtual bool CanUseFists(bool really) const;
	virtual bool CanUseGaussPistolPulse(bool really) const;
	virtual bool CanUseGaussPistolCharge(bool really) const;
	virtual bool CanUseGaussPistolRapid(bool really) const;
	virtual bool CanUseShotgun(bool really) const;
	virtual bool CanUseMinigun(bool really) const;
	virtual bool CanUseBeamgun(bool really) const;
	virtual bool CanUseChemgun(bool really) const;
	virtual bool CanUseDML(bool really) const;
	virtual bool CanUseDMLGrenade(bool really) const;
	virtual bool CanUseAICore(bool really) const;

	const static int PISTOL_PULSE = 1;
	const static int PISTOL_CHARGE = 2;
	const static int PISTOL_RAPID = 3;
	const static int PISTOL_SNIPER = 4;
private:
	int iPistolMode;
};

#endif // __BOT_GUNMAN_H__