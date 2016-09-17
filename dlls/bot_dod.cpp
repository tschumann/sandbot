#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

void DODBot::OnSpawn()
{
	this->bCapturing = false;
}

void DODBot::Join()
{
	if( this->start_action == MSG_DOD_TEAM_SELECT )
	{
		// switch back to idle
		this->start_action = MSG_DOD_IDLE;

		// TODO: British?
		if ((this->bot_team != DODBot::TEAM_ALLIES) && (this->bot_team != DODBot::TEAM_AXIS))
		{
			this->bot_team = -1;
		}

		// TODO: count how many each team has because even teams seems to be off by default
		if (this->bot_team == -1)
		{
			this->bot_team = RANDOM_LONG(1, 2);
		}

		// select the team the bot wishes to join...
		if (this->bot_team == DODBot::TEAM_ALLIES)
		{
			FakeClientCommand( this->pEdict, "jointeam", "1", NULL );
		}
		else if (this->bot_team == DODBot::TEAM_AXIS)
		{
			FakeClientCommand( this->pEdict, "jointeam", "2", NULL );
		}

		return;
	}
	else if( this->start_action == MSG_DOD_ALLIED_SELECT )
	{
		this->start_action = MSG_DOD_IDLE;  // switch back to idle

		// do this to control what classes bots will spawn as (not everything is supported yet)
		int playerClass = RANDOM_LONG(1, 4);

		switch( playerClass )
		{
		case 1:
			FakeClientCommand( this->pEdict, "cls_garand", NULL, NULL );
			break;
		case 2:
			FakeClientCommand( this->pEdict, "cls_carbine", NULL, NULL );
			break;
		case 3:
			FakeClientCommand( this->pEdict, "cls_tommy", NULL, NULL );
			break;
		case 4:
			FakeClientCommand( this->pEdict, "cls_grease", NULL, NULL );
			break;
		default:
			FakeClientCommand( this->pEdict, "cls_random", NULL, NULL );
			break;
		}

		// bot has now joined the game (doesn't need to be started)
		this->not_started = 0;

		return;
	}
	else if( this->start_action == MSG_DOD_AXIS_SELECT )
	{
		this->start_action = MSG_DOD_IDLE;  // switch back to idle

		int playerClass = RANDOM_LONG(1, 4);

		switch( playerClass )
		{
		case 1:
			FakeClientCommand( this->pEdict, "cls_k98", NULL, NULL );
			break;
		case 2:
			FakeClientCommand( this->pEdict, "cls_k43", NULL, NULL );
			break;
		case 3:
			FakeClientCommand( this->pEdict, "cls_mp40", NULL, NULL );
			break;
		case 4:
			FakeClientCommand( this->pEdict, "cls_mp44", NULL, NULL );
			break;
		default:
			FakeClientCommand( this->pEdict, "cls_random", NULL, NULL );
			break;
		}

		// bot has now joined the game (doesn't need to be started)
		this->not_started = 0;

		return;
	}
}

bool DODBot::ShouldCapturePoint( edict_t * pControlPoint )
{
	// if it's currently captured by the Axis and the player is Allied
	if( pControlPoint->v.body == 0 && this->GetTeam() == DODBot::TEAM_ALLIES )
	{
		return true;
	}

	// if it's currently captured by the Allies and the player is Axis
	if( pControlPoint->v.body == 1 && this->GetTeam() == DODBot::TEAM_AXIS )
	{
		return true;
	}

	// if it's uncaptured
	if( pControlPoint->v.body == 3 )
	{
		return true;
	}

	return false;
}