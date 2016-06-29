#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

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