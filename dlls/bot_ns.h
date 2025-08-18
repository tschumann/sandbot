//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Natural Selection specific bot code
//
//=============================================================================

#ifndef __BOT_NS_H__
#define __BOT_NS_H__

#include "bot.h"
#include "game.h"

class NSBot : public bot_t
{
public:
	NSBot();

	virtual void OnSpawn();
	virtual void Join();
	virtual void Think();

	virtual bool CanHeal() const;

	virtual bool CanUseItem(const edict_t* pItem);

	virtual float GetSpeedToEnemy() const;

	virtual void Reset();
	virtual float GetAimSpread() const;
	virtual void Reload();
	virtual bool ShouldReload() const;

	virtual int GetGoalType();

	virtual bool IsNearHive();
	virtual bool IsInReadyRoom() const;
	virtual int GetDesiredClass() const;
	virtual void SetDesiredClass(int iDesiredClass);
	virtual void ChooseDesiredClass();
	virtual void ClassicUpgrade();
	virtual bool ShouldCombatUpgrade();
	virtual void CombatUpgrade();
	virtual float GetResources();

	virtual bool IsMarine() const;
	virtual bool HasWeaponDamage1();
	virtual void UpgradeToWeaponDamage1();
	virtual bool HasArmor1();
	virtual void UpgradeToArmor1();
	virtual bool HasShotgun();
	virtual void UpgradeToShotgun();
	virtual bool HasHMG();
	virtual void UpgradeToHMG();
	virtual bool HasWelder();

	virtual bool ShouldAttackHive(edict_t* pHive);

	virtual bool IsAlien() const;
	virtual bool HasCarapace();
	virtual void UpgradeToCarapace();
	virtual bool HasRegeneration();
	virtual void UpgradeToRegeneration();
	virtual bool HasCelerity();
	virtual void UpgradeToCelerity();
	virtual bool CanEvolve();
	virtual bool IsEvolved();
	virtual void StartEvolving();
	virtual void FinishEvolving();
	virtual void EvolveToGorge();
	virtual void EvolveToLerk();
	virtual void EvolveToFade();
	virtual void EvolveToOnos();

	virtual bool ShouldBecomeCommander() const;
	virtual bool IsCommander() const;

	virtual bool ShouldBecomeGorge() const;
	virtual bool IsGorge() const;
	virtual bool ShouldBecomeLerk() const;
	virtual bool IsLerk() const;
	virtual bool ShouldBecomeFade() const;
	virtual bool IsFade() const;
	virtual bool ShouldBecomeOnos() const;
	virtual bool IsOnos() const;

	virtual bool ShouldBuildResourceTower() const;

	virtual edict_t* FindEnemy();

	const static int TEAM_NONE = 0;
	const static int TEAM_MARINE = 1;
	const static int TEAM_ALIEN = 2;

	const static int CLASS_SKULK = AVH_USER3_ALIEN_PLAYER1;
	const static int CLASS_GORGE = AVH_USER3_ALIEN_PLAYER2;
	const static int CLASS_LERK = AVH_USER3_ALIEN_PLAYER3;
	const static int CLASS_FADE = AVH_USER3_ALIEN_PLAYER4;
	const static int CLASS_ONOS = AVH_USER3_ALIEN_PLAYER5;

	// from AvHMessage.h
	const static int RESEARCH_ARMOR_ONE = 20;
	const static int RESEARCH_ARMOR_TWO = 21;
	const static int RESEARCH_ARMOR_THREE = 22;
	const static int ALIEN_BUILD_RESOURCES = 90;
	const static int ALIEN_BUILD_OFFENSE_CHAMBER = 91;
	const static int ALIEN_BUILD_DEFENSE_CHAMBER = 92;
	const static int ALIEN_BUILD_SENSORY_CHAMBER = 93;
	const static int ALIEN_BUILD_MOVEMENT_CHAMBER = 94;
	const static int ALIEN_BUILD_HIVE = 95;
	const static int ALIEN_EVOLUTION_ONE = 101;
	const static int ALIEN_EVOLUTION_TWO = 102;
	const static int ALIEN_EVOLUTION_SEVEN = 107;
	const static int ALIEN_LIFEFORM_ONE = 113;
	const static int ALIEN_LIFEFORM_TWO = 114;
	const static int ALIEN_LIFEFORM_THREE = 115;
	const static int ALIEN_LIFEFORM_FOUR = 116;
	const static int ALIEN_LIFEFORM_FIVE = 117;

	// TODO: possibly make these equal to the exact names of things in the Natural
	// Selection code so they can be traced back if necessary (ideally use the exact
	// names used in Natural Selection but they can be vague at times)
	const static int CLASSIC_BUILD_RESOURCE_TOWER = ALIEN_BUILD_RESOURCES;
	const static int CLASSIC_BUILD_HIVE = ALIEN_BUILD_HIVE;

	const static int COMBAT_UPGRADE_WEAPON_DAMAGE_1 = 23;
	const static int COMBAT_UPGRADE_ARMOR_1 = RESEARCH_ARMOR_ONE;
	const static int COMBAT_UPGRADE_SHOTGUN = 64;
	const static int COMBAT_UPGRADE_HMG = 65;
	const static int COMBAT_UPGRADE_CARAPACE = ALIEN_EVOLUTION_ONE;
	const static int COMBAT_UPGRADE_REGENERATION = ALIEN_EVOLUTION_TWO;
	const static int COMBAT_UPGRADE_CELERITY = ALIEN_EVOLUTION_SEVEN;

	const static int EVOLVE_TO_GORGE = ALIEN_LIFEFORM_TWO;
	const static int EVOLVE_TO_LERK = ALIEN_LIFEFORM_THREE;
	const static int EVOLVE_TO_FADE = ALIEN_LIFEFORM_FOUR;
	const static int EVOLVE_TO_ONOS = ALIEN_LIFEFORM_FIVE;

protected:
	int iDesiredClass;

	bool bIsEvolving;
	bool bShouldBuildResourceNode;
};

#endif // __BOT_NS_H__