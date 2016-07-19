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

void NSBot::Reset()
{
	// this->SetDesiredClass( NSBot::CLASS_SKULK );
	this->bIsEvolving = false;
}

bool NSBot::IsNearHive()
{
	edict_t *pent = NULL;

	// TODO: cache the results of this in NSGame
	while( pent = UTIL_FindEntityByClassname( pent, "team_hive" ) )
	{
		float distance = (pent->v.origin - this->pEdict->v.origin).Length();

		if( distance <= 600.0 )
		{
			return true;
		}
	}

	return false;
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
	return this->pEdict->v.team == NS_TEAM_MARINE;
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


bool NSBot::IsAlien()
{
	return this->pEdict->v.team == NS_TEAM_ALIEN;
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
	this->f_move_speed = 0.0;
	this->bEvolving = true;
}

void NSBot::FinishEvolving()
{
	this->f_move_speed = this->f_max_speed;
	this->bEvolving = false;
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