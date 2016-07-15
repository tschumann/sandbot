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
			this->SetDesiredClass( NSBot::NS_CLASS_SKULK );
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			this->SetDesiredClass( NSBot::NS_CLASS_GORGE );
			break;
		case 6:
			this->SetDesiredClass( NSBot::NS_CLASS_LERK );
			break;
		case 7:
		case 8:
		case 9:
			this->SetDesiredClass( NSBot::NS_CLASS_FADE );
			break;
		case 10:
			this->SetDesiredClass( NSBot::NS_CLASS_ONOS );
			break;
		}
	}
	else
	{
		switch( iClass )
		{
		case 0:
			this->SetDesiredClass( NSBot::NS_CLASS_SKULK );
			break;
		case 1:
			this->SetDesiredClass( NSBot::NS_CLASS_GORGE );
			break;
		case 2:
			this->SetDesiredClass( NSBot::NS_CLASS_LERK );
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			this->SetDesiredClass( NSBot::NS_CLASS_FADE );
			break;
		case 7:
		case 8:
		case 9:
		case 10:
			this->SetDesiredClass( NSBot::NS_CLASS_ONOS );
			break;
		}
	}
}

void NSBot::CombatUpgrade()
{
	if( this->HasEnemy() )
	{
		return;
	}

	// find out what impulses mean what (check AvHMessage.h)
	if( this->IsAlien() && this->IsNearHive() )
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


bool NSBot::IsMarine()
{
	return this->pEdict->v.team == NS_TEAM_MARINE;
}

bool NSBot::HasWeaponDamage1()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_1;
}

void NSBot::UpgradeToWeaponDamage1()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_WEAPON_DAMAGE_1;
}

bool NSBot::HasArmor1()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_4;
}

void NSBot::UpgradeToArmor1()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_ARMOR_1;
}

bool NSBot::HasShotgun()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_SHOTGUN));
}

void NSBot::UpgradeToShotgun()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_SHOTGUN;
}

bool NSBot::HasHMG()
{
	return (this->pEdict->v.weapons & (1<<NS_WEAPON_HEAVYMACHINEGUN));
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
	return this->pEdict->v.iuser4 & MASK_UPGRADE_1;
}

void NSBot::UpgradeToCarapace()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_CARAPACE;
}

bool NSBot::HasRegeneration()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_2;
}

void NSBot::UpgradeToRegeneration()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_REGENERATION;
}

bool NSBot::HasCelerity()
{
	return this->pEdict->v.iuser4 & MASK_UPGRADE_4;
}

void NSBot::UpgradeToCelerity()
{
	this->pEdict->v.impulse = NSBot::COMBAT_UPGRADE_CELERITY;
}

void NSBot::EvolveToGorge()
{
	if( ((NSGame *)pGame)->IsCombat() )
	{
		this->pEdict->v.impulse = NSBot::EVOLVE_TO_GORGE;
	}
	else
	{
		// TODO: check that it's the same in Classic
	}
}

bool NSBot::ShouldBecomeGorge()
{
	return this->GetDesiredClass() == NSBot::NS_CLASS_GORGE;
}

bool NSBot::IsGorge()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER2;
}

void NSBot::EvolveToLerk()
{
	if( ((NSGame *)pGame)->IsCombat() )
	{
		this->pEdict->v.impulse = NSBot::EVOLVE_TO_LERK;
	}
	else
	{
		// TODO: check that it's the same in Classic
	}
}

bool NSBot::ShouldBecomeLerk()
{
	return this->GetDesiredClass() == NSBot::NS_CLASS_LERK;
}

bool NSBot::IsLerk()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER3;
}

void NSBot::EvolveToFade()
{
	if( ((NSGame *)pGame)->IsCombat() )
	{
		this->pEdict->v.impulse = NSBot::EVOLVE_TO_FADE;
	}
	else
	{
		// TODO: check that it's the same in Classic
	}
}

bool NSBot::ShouldBecomeFade()
{
	return this->GetDesiredClass() == NSBot::NS_CLASS_FADE;
}

bool NSBot::IsFade()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER4;
}

void NSBot::EvolveToOnos()
{
	if( ((NSGame *)pGame)->IsCombat() )
	{
		this->pEdict->v.impulse = NSBot::EVOLVE_TO_ONOS;
	}
	else
	{
		// TODO: check that it's the same in Classic
	}
}

bool NSBot::ShouldBecomeOnos()
{
	return this->GetDesiredClass() == NSBot::NS_CLASS_ONOS;
}

bool NSBot::IsOnos()
{
	return this->pEdict->v.iuser3 == AVH_USER3_ALIEN_PLAYER5;
}