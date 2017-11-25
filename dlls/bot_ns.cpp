#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

// http://www.unknownworlds.com/oldwebsite/manuals/Natural_Selection_Manual.html
// http://www.unknownworlds.com/oldwebsite/manuals/comm_manual/basic/index.htm

NSBot::NSBot()
{
}

void NSBot::OnSpawn()
{
	bot_t::OnSpawn();

	this->bIsEvolving = false;
}

void NSBot::Join()
{
	this->Reset();

	switch( this->start_action )
	{
	case MSG_NS_JOIN_ALIEN:
		FakeClientCommand( this->pEdict, "jointeamone" );
		break;
	case MSG_NS_JOIN_MARINE:
		FakeClientCommand( this->pEdict, "jointeamtwo" );
		break;
	case MSG_NS_JOIN_AUTO:
	default:
		FakeClientCommand( this->pEdict, "autoassign" );
		break;
	}

	this->start_action = 0;
	this->not_started = 0;
}

void NSBot::Think()
{
	bot_t::PreThink();

	extern bool g_bInGame;

	if( g_bInGame && ((NSGame *)pGame)->IsClassic() )
	{
		this->ClassicUpgrade();
	}
	else if( g_bInGame && ((NSGame *)pGame)->IsCombat() && this->ShouldCombatUpgrade() )
	{
		this->CombatUpgrade();
	}

	bot_t::PostThink();
}

bool NSBot::CanUseItem( edict_t *pItem )
{
	Vector vecStart = pEdict->v.origin + pEdict->v.view_ofs;
	Vector vecEnd = pItem->v.origin;

	// check if entity is ammo
	if (this->IsMarine() && !strcmp("item_ammopack", STRING(pItem->v.classname)))
	{
		// check if the item is not visible (i.e. has not respawned)
		if (pItem->v.effects & EF_NODRAW)
		{
			return false;
		}

		return true;
	}
	// check if entity is a catalyst
	else if (this->IsMarine() && !strcmp("item_catalyst", STRING(pItem->v.classname)))
	{
		// check if the item is not visible (i.e. has not respawned)
		if (pItem->v.effects & EF_NODRAW)
		{
			return false;
		}

		return true;
	}
	// check if entity is ammo
	else if (this->IsMarine() && !strcmp("item_genericammo", STRING(pItem->v.classname)))
	{
		// check if the item is not visible (i.e. has not respawned)
		if (pItem->v.effects & EF_NODRAW)
		{
			return false;
		}

		return true;
	}

	// check if entity is a healthkit
	else if (this->IsMarine() && !strcmp("item_health", STRING(pItem->v.classname)))
	{
		// check if the item is not visible (i.e. has not respawned)
		if (pItem->v.effects & EF_NODRAW)
		{
			return false;
		}

		// check if the bot can make use of this item
		if (pEdict->v.health < pEdict->v.max_health)
		{
			return true;
		}
	}
	// check if entity is an alien resource tower
	else if (this->IsAlien() && UTIL_GetClass(pEdict) == AVH_USER3_ALIEN_PLAYER2 && !strcmp("alienresourcetower", STRING(pItem->v.classname)))
	{
		float fDistance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((fDistance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to resource tower...
				if (fDistance < 100.0)
				{
					// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is a resource tower
	else if (this->IsMarine() && !strcmp("resourcetower", STRING(pItem->v.classname)))
	{
		float fDistance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((fDistance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to resource tower...
				if (fDistance < 100.0)
				{
					// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is an arms lab
	else if (this->IsMarine() && !strcmp("team_armslab", STRING(pItem->v.classname)))
	{
		float fDistance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((fDistance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (fDistance < 100.0)
				{
					// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is an advanced armory
	else if (this->IsMarine() && !strcmp("team_advarmory", STRING(pItem->v.classname)))
	{
		float fDistance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((fDistance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (fDistance < 100.0)
				{
				// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is an armory
	else if (this->IsMarine() && !strcmp("team_armory", STRING(pItem->v.classname)))
	{
		float fDistance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((fDistance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (fDistance < 100.0)
				{
				// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is a hive
	else if (this->IsAlien() && UTIL_GetClass(pEdict) == AVH_USER3_ALIEN_PLAYER2 && !strcmp("team_hive", STRING(pItem->v.classname)))
	{
		float fDistance = (vecEnd - vecStart).Length();

		if (!this->bBuildHive)
		{
			// check if close enough and facing it directly...
			if ((fDistance < bot_t::PLAYER_SEARCH_RADIUS * 12.0) && this->GetResources() >= kHiveCost)
			{
				this->bBuildHive = true;
			}

			// if close to hive...
			if (fDistance < 100.0)
			{
				// don't avoid walls for a while
				this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
			}

			return true;
		}
	}
	// check if entity is an infantry portal...
	else if (this->IsMarine() && !strcmp("team_infportal", STRING(pItem->v.classname)))
	{
		float distance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((distance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (distance < 100.0)
				{
					// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is an observatory...
	else if (this->IsMarine() && !strcmp("team_observatory", STRING(pItem->v.classname)))
	{
		float distance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((distance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (distance < 100.0)
				{
					// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is a prototype lab...
	else if (this->IsMarine() && !strcmp("team_prototypelab", STRING(pItem->v.classname)))
	{
		float distance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((distance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (distance < 100.0)
				{
				// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is a turret factory...
	else if (this->IsMarine() && !strcmp("team_turretfactory", STRING(pItem->v.classname)))
	{
		float distance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((distance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (distance < 100.0)
				{
					// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is a siege turret...
	else if (this->IsMarine() && !strcmp("siegeturret", STRING(pItem->v.classname)))
	{
		float distance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((distance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (distance < 100.0)
				{
				// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}
	// check if entity is a turret...
	else if (this->IsMarine() && !strcmp("turret", STRING(pItem->v.classname)))
	{
		float distance = (vecEnd - vecStart).Length();

		if (!UTIL_IsBuilt(pItem))
		{
			if (!this->bBuild)
			{
				// check if close enough and facing it directly...
				if ((distance < bot_t::PLAYER_SEARCH_RADIUS * 2.0) /*&& ( angle_to_entity <= 10 )*/)
				{
					this->bBuild = true;
					this->fBuildTime = gpGlobals->time;
				}

				// if close to armory...
				if (distance < 100.0)
				{
				// don't avoid walls for a while
					this->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
				}

				return true;
			}
		}
		else
		{
			this->bBuild = false;
		}
	}

	return false;
}

float NSBot::GetSpeedToEnemy()
{
	if( !this->pBotEnemy )
	{
		ALERT( at_error, "Call to __FUNCTION__ when pBotEnemy is NULL!\n" );
	}

	float fDistanceToEnemy = this->GetDistanceToEnemy();
	float fSpeed = 0.0;

	// run if distance to enemy is far
	if( fDistanceToEnemy > 200.0 )
	{
		fSpeed = this->GetMaxSpeed();
	}
	// walk if distance is closer
	else if( fDistanceToEnemy > 20.0 )
	{
		// TODO: this check should more generally look at whether the bot has a melee weapon or not
		if (this->IsAlien())
		{
			// alien's should charge ahead due to the strength of their close-range attacks
			fSpeed = this->GetMaxSpeed();
		}
		else
		{
			fSpeed = this->GetMaxSpeed() / 2.0;
		}
	}
	// don't move if close enough
	else
	{
		fSpeed = 0.0;
	}

	return fSpeed;
}

void NSBot::Reset()
{
	// this->SetDesiredClass( NSBot::CLASS_SKULK );
	this->bIsEvolving = false;
}

float NSBot::GetAimSpread()
{
	float fSpread = bot_t::GetAimSpread();

	// until light level can be properly detected, make marines slightly less accurate
	if (this->IsMarine())
	{
		fSpread += 0.1;
	}

	return fSpread;
}

void NSBot::Reload()
{
	bot_t::Reload();

	// only marines should be reloading, but check anyway
	if( this->IsMarine() )
	{
		UTIL_HostSay(this->pEdict, TRUE, "reloading");
	}
}

bool NSBot::ShouldReload()
{
	if( this->IsAlien() )
	{
		return false;
	}
	else
	{
		// TODO: look at current weapon's clip
		return true;
	}
}

int NSBot::GetGoalType()
{
	if( this->IsMarine() )
	{
		// TODO: or W_FL_NS_RESNODE if in Classic and some per-bot flag is set
		return W_FL_NS_HIVE;
	}
	else
	{
		// TODO: or W_FL_NS_RESNODE if in Classic and a Gorge
		return W_FL_NS_COMMAND_CHAIR;
	}
}

bool NSBot::IsNearHive()
{
	edict_t *pent = NULL;

	// TODO: cache the results of this in NSGame
	while( pent = UTIL_FindEntityByClassname( pent, "team_hive" ) )
	{
		float distance = (pent->v.origin - this->GetOrigin()).Length();

		if( distance <= 600.0 )
		{
			return true;
		}
	}

	return false;
}

bool NSBot::IsInReadyRoom()
{
	return this->pEdict->v.playerclass == PLAYMODE_READYROOM;
}

int NSBot::GetDesiredClass()
{
	return this->iDesiredClass;
}

void NSBot::SetDesiredClass( int iDesiredClass )
{
	this->iDesiredClass = iDesiredClass;
}

void NSBot::ChooseDesiredClass()
{
	int iClass = RANDOM_LONG( 0, 10 );

	if( ((NSGame *)pGame)->IsClassic() )
	{
		switch( iClass )
		{
		case 0:
		case 1:
			this->SetDesiredClass( NSBot::CLASS_SKULK );
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			this->SetDesiredClass( NSBot::CLASS_GORGE );
			break;
		case 6:
			this->SetDesiredClass( NSBot::CLASS_LERK );
			break;
		case 7:
		case 8:
		case 9:
			this->SetDesiredClass( NSBot::CLASS_FADE );
			break;
		case 10:
			this->SetDesiredClass( NSBot::CLASS_ONOS );
			break;
		}
	}
	else
	{
		switch( iClass )
		{
		case 0:
			this->SetDesiredClass( NSBot::CLASS_SKULK );
			break;
		case 1:
			this->SetDesiredClass( NSBot::CLASS_GORGE );
			break;
		case 2:
			this->SetDesiredClass( NSBot::CLASS_LERK );
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			this->SetDesiredClass( NSBot::CLASS_FADE );
			break;
		case 7:
		case 8:
		case 9:
		case 10:
			this->SetDesiredClass( NSBot::CLASS_ONOS );
			break;
		}
	}
}

void NSBot::ClassicUpgrade()
{
	if( this->IsAlien() )
	{
		if( this->IsEvolved() )
		{
			this->FinishEvolving();
		}

		// start evolving
		if( this->CanEvolve() )
		{
			if( this->ShouldBecomeGorge() && this->GetResources() > (float)kGorgeCost )
			{
				this->EvolveToGorge();
			}
			else if( this->ShouldBecomeLerk() && this->GetResources() > (float)kLerkCost )
			{
				this->EvolveToLerk();
			}
			else if( this->ShouldBecomeFade() && this->GetResources() > (float)kFadeCost )
			{
				this->EvolveToFade();
			}
			else if( this->ShouldBecomeOnos() && this->GetResources() > (float)kOnosCost )
			{
				this->EvolveToOnos();
			}
		}
	}
}

bool NSBot::ShouldCombatUpgrade()
{
	if( this->HasEnemy() )
	{
		return false;
	}

	if( this->IsAlien() && !this->IsNearHive() )
	{
		return false;
	}

	return true;
}

void NSBot::CombatUpgrade()
{
	// find out what impulses mean what (check AvHMessage.h)
	if( this->IsAlien() )
	{
		if( !this->HasCarapace() )
		{
			this->UpgradeToCarapace();					
		}
		else if( !this->HasRegeneration() )
		{
			this->UpgradeToRegeneration();					
		}
		else if( !this->HasCelerity() )
		{
			this->UpgradeToCelerity();					
		}
		else if( !this->IsGorge() && this->ShouldBecomeGorge() )
		{
			this->EvolveToGorge();
		}
		else if( !this->IsLerk() && this->ShouldBecomeLerk() )
		{
			this->EvolveToLerk();
		}
		else if( !this->IsFade() && this->ShouldBecomeFade() )
		{
			this->EvolveToFade();
		}
		else if( !this->IsOnos() && this->ShouldBecomeOnos() )
		{
			this->EvolveToOnos();
		}
	}
	else if( this->IsMarine() )
	{
		if( !this->HasWeaponDamage1() )
		{
			this->UpgradeToWeaponDamage1();
		}
		else if( !this->HasShotgun() )
		{
			this->UpgradeToShotgun();
		}
		else if( !this->HasArmor1() )
		{
			this->UpgradeToArmor1();
		}
		else if( !this->HasHMG() )
		{
			this->UpgradeToHMG();
		}
	}
}

float NSBot::GetResources()
{
	return pEdict->v.vuser4.z / 100.0;
}


bool NSBot::IsMarine()
{
	return this->pEdict->v.team == NSBot::TEAM_MARINE;
}

bool NSBot::HasWeaponDamage1()
{
	return (this->pEdict->v.iuser4 & MASK_UPGRADE_1) != 0;
}

void NSBot::UpgradeToWeaponDamage1()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_WEAPON_DAMAGE_1;
}

bool NSBot::HasArmor1()
{
	return (this->pEdict->v.iuser4 & MASK_UPGRADE_4) != 0;
}

void NSBot::UpgradeToArmor1()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_ARMOR_1;
}

bool NSBot::HasShotgun()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_SHOTGUN)) != 0;
}

void NSBot::UpgradeToShotgun()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_SHOTGUN;
}

bool NSBot::HasHMG()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_HEAVYMACHINEGUN)) != 0;
}

void NSBot::UpgradeToHMG()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_HMG;
}

bool NSBot::HasWelder()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_WELDER)) != 0;
}

bool NSBot::ShouldAttackHive( edict_t *pHive )
{
	if( ((NSGame *)pGame)->IsClassic() )
	{
		// TODO: check if pev->effects & EF_NODRAW and return false too?
		if( pHive->v.solid == SOLID_NOT )
		{
			return false;
		}

		return true;
	}
	else
	{
		// in Combat there's only one hive, so always go for it
		return true;
	}
}


bool NSBot::IsAlien()
{
	return this->pEdict->v.team == NSBot::TEAM_ALIEN;
}

bool NSBot::HasCarapace()
{
	return (this->pEdict->v.iuser4 & MASK_UPGRADE_1) != 0;
}

void NSBot::UpgradeToCarapace()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_CARAPACE;
}

bool NSBot::HasRegeneration()
{
	return (this->pEdict->v.iuser4 & MASK_UPGRADE_2) != 0;
}

void NSBot::UpgradeToRegeneration()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_REGENERATION;
}

bool NSBot::HasCelerity()
{
	return (this->pEdict->v.iuser4 & MASK_UPGRADE_4) != 0;
}

void NSBot::UpgradeToCelerity()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_CELERITY;
}

bool NSBot::CanEvolve()
{
	if( this->HasEnemy() )
	{
		return false;
	}

	if( this->bIsEvolving )
	{
		return false;
	}

	if( this->GetDesiredClass() == NSBot::CLASS_SKULK )
	{
		return false;
	}

	return true;
}

bool NSBot::IsEvolved()
{
	return this->IsGorge() || this->IsLerk() || this->IsFade() || this->IsOnos();
}

void NSBot::StartEvolving()
{
	this->SetSpeed( 0.0 );
	this->bIsEvolving = true;
}

void NSBot::FinishEvolving()
{
	this->SetSpeed( this->GetMaxSpeed() );
	this->bIsEvolving = false;
}

void NSBot::EvolveToGorge()
{
	this->StartEvolving();
	this->pEdict->v.impulse = NSBot::EVOLVE_TO_GORGE;
}

bool NSBot::ShouldBecomeGorge()
{
	return this->GetDesiredClass() == NSBot::CLASS_GORGE;
}

bool NSBot::IsGorge()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER2;
}

void NSBot::EvolveToLerk()
{
	this->StartEvolving();
	this->pEdict->v.impulse = NSBot::EVOLVE_TO_LERK;
}

bool NSBot::ShouldBecomeLerk()
{
	return this->GetDesiredClass() == NSBot::CLASS_LERK;
}

bool NSBot::IsLerk()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER3;
}

void NSBot::EvolveToFade()
{
	this->StartEvolving();
	this->pEdict->v.impulse = NSBot::EVOLVE_TO_FADE;
}

bool NSBot::ShouldBecomeFade()
{
	return this->GetDesiredClass() == NSBot::CLASS_FADE;
}

bool NSBot::IsFade()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER4;
}

void NSBot::EvolveToOnos()
{
	this->StartEvolving();
	this->pEdict->v.impulse = NSBot::EVOLVE_TO_ONOS;
}

bool NSBot::ShouldBecomeOnos()
{
	return this->GetDesiredClass() == NSBot::CLASS_ONOS;
}

bool NSBot::IsOnos()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER5;
}

edict_t* NSBot::FindEnemy()
{
	Vector vecEnd;
	edict_t *pent = NULL;
	edict_t *pNewEnemy = NULL;
	float nearestdistance = 1000.0;

	if( this->pEdict->v.team == NSBot::TEAM_MARINE )
	{
		// TODO: possibly not needed, but safe to have - when at a bot waypoint it should
		// see the hive and acquire it as an enemy, but possibly it won't reqacquire it
		// as an enemy if it gets distracted?
		while( (pent = UTIL_FindEntityByClassname( pent, "team_hive" )) != NULL )
		{
			vecEnd = pent->v.origin + pent->v.view_ofs;

			// is this hive visible?
			if (FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) && pent->v.solid != SOLID_NOT)
			{
				float distance = (pent->v.origin - this->GetOrigin()).Length();
				UTIL_LogDPrintf( "found a hive\n" );

				// is this the closest hive?
				if (distance < nearestdistance)
				{
					nearestdistance = distance;
					pNewEnemy = pent;
				}
			}
		}

		// if there's no hive, look for some other enemies
		if( !this->HasEnemy() )
		{
			while( pent = UTIL_FindEntityInSphere( pent, this->GetOrigin(), 500.0 ) )
			{
				// ignore unbuilt structures
				if( pent->v.solid != SOLID_NOT )
				{
					continue;
				}

				if( !strcmp( "alienresourcetower", STRING(pent->v.classname) ) ||
					!strcmp( "defensechamber", STRING(pent->v.classname) ) ||
					!strcmp( "movementchamber", STRING(pent->v.classname) ) ||
					!strcmp( "offensechamber", STRING(pent->v.classname) ) ||
					!strcmp( "sensorychamber", STRING(pent->v.classname) ))
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - this->GetOrigin()).Length();

						// is this the closest?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							this->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
				}
			}
		}
	}
	else if( this->pEdict->v.team == NSBot::TEAM_ALIEN )
	{
		while( (pent = UTIL_FindEntityByClassname( pent, "team_command" )) != NULL )
		{
			vecEnd = pent->v.origin + pent->v.view_ofs;

			// is this command chair visible?
			if (/*FInViewCone( &vecEnd, pEdict ) && */FVisible( vecEnd, pEdict ))
			{
				float distance = (pent->v.origin - this->GetOrigin()).Length();
				UTIL_LogDPrintf( "found a command chair\n" );

				// is this the closest command chair?
				if (distance < nearestdistance)
				{
					nearestdistance = distance;
					pNewEnemy = pent;
				}
			}
		}

		// if there's no command chair, look for some other enemies
		if( !this->HasEnemy() )
		{
			while( pent = UTIL_FindEntityInSphere( pent, this->GetOrigin(), 500.0 ) )
			{
				if( !strcmp( "team_advarmoy", STRING(pent->v.classname) ) ||
					!strcmp( "team_armory", STRING(pent->v.classname) ) ||
					!strcmp( "team_armslab", STRING(pent->v.classname) ) ||
					!strcmp( "team_infportal", STRING(pent->v.classname) ) ||
					!strcmp( "team_observatory", STRING(pent->v.classname) ) ||
					!strcmp( "team_prototypelab", STRING(pent->v.classname) ) ||
					!strcmp( "team_turretfactory", STRING(pent->v.classname) ) || 
					!strcmp( "turret", STRING(pent->v.classname) ) ||
					!strcmp( "siegeturret", STRING(pent->v.classname) ) )
				{
					vecEnd = pent->v.origin + pent->v.view_ofs;

					// is this visible?
					if( FInViewCone( &vecEnd, pEdict ) && FVisible( vecEnd, pEdict ) )
					{
						float fDistance = (pent->v.origin - this->GetOrigin()).Length();

						// is this the closest sentry gun?
						if( fDistance < nearestdistance )
						{
							nearestdistance = fDistance;
							pNewEnemy = pent;

							this->pBotUser = NULL;  // don't follow user when enemy found
						}
					}
				}
			}
		}
	}

	return pNewEnemy;
}
