#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "wpt.h"
#include "waypoint.h"

ShipBot::ShipBot()
{
}

bool ShipBot::IsValidEnemy( edict_t *pEnemy )
{
	bool bBaseIsValid = bot_t::IsValidEnemy( pEnemy );
	bool bShipIsValid = bBaseIsValid;

	// if the bot has a quarry but this isn't the
	// quarry, it shouldn't be targetted as an enemy
	if( this->HasQuarry() && this->GetQuarry() != this->pEdict )
	{
		bShipIsValid = false;
	}

	return bBaseIsValid && bShipIsValid;
}

void ShipBot::PickUpItem()
{
	FakeClientCommand( this->pEdict, "pickup" );
}

bool ShipBot::ShouldJumpAfterDeath()
{
	return false;
}

void ShipBot::SetQuarry( int iEntIndex )
{
	// TODO: check that no quarry is -1
	if( iEntIndex == -1 )
	{
		this->pQuarry = NULL;
	}
	else
	{
		this->pQuarry = INDEXENT( iEntIndex );
	}
}

edict_t* ShipBot::GetQuarry()
{
	return this->pQuarry;
}

bool ShipBot::HasQuarry()
{
	return this->pQuarry != NULL;
}
