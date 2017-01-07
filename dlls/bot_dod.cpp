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
	this->iGoalIndex = 0;
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

		if (this->bot_class == -1)
		{
			// do this to control what classes bots will spawn as (not everything is supported yet)
			this->bot_class = RANDOM_LONG(1, 5);
		}

		switch( this->bot_class )
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
		case 5: // 8
			FakeClientCommand( this->pEdict, "cls_bazooka", NULL, NULL );
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

		if (this->bot_class == -1)
		{
			// do this to control what classes bots will spawn as (not everything is supported yet)
			this->bot_class = RANDOM_LONG(1, 5);
		}

		switch( this->bot_class )
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
		case 5: // 8
			FakeClientCommand( this->pEdict, "cls_pshreck", NULL, NULL );
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

void DODBot::Think()
{
	bot_t::PreThink();

	// TODO: possibly this will trigger before the bot is touching the capture point? shouldn't
	// though because bCapturing is only true when the bot is close enough to the waypoint
	// if the bot is capturing, and is at a capture point, and it's a point that should be captured
	if( this->bCapturing )
	{
		this->f_move_speed = 0.0;

		// TODO: this is very rough - probably something is set in pev if the bot is
		// on or near a dod_control_point - should check if it's a brush entity...
		if( DistanceToNearest(this->pEdict->v.origin, "dod_control_point") > 200 )
		{
			ALERT( at_console, "too far from capture point while capturing; resetting\n" );
			this->f_move_speed = this->pEdict->v.maxspeed;
			this->bCapturing = false;
		}
	}

	// TODO: waypoint goal changes once it's capturing?
	// if the current waypoint is a capture point and it is now captured
	if( this->bCapturing && ShouldSkip(this->pEdict, this->iGoalIndex) )
	{
		ALERT( at_console, "leaving waypoint\n" );
		this->f_move_speed = this->pEdict->v.maxspeed;
		this->bCapturing = false;
	}

	bot_t::PostThink();
}

float DODBot::GetSpeedToEnemy()
{
	if( !this->pBotEnemy )
	{
		ALERT( at_error, "Call to " __FUNCTION__ " when pBotEnemy is NULL!\n" );
	}

	float fDistanceToEnemy = this->GetDistanceToEnemy();
	float fSpeed = 0.0;

	// run if distance to enemy is far
	if (fDistanceToEnemy > 200.0)
	{
		fSpeed = this->GetMaxSpeed();
	}
	else
	{
		fSpeed = 0.0;
	}

	return fSpeed;
}

bool DODBot::IsSniper()
{
	return false;
}

bool DODBot::ShouldLookForNewGoal()
{
	return !this->bCapturing;
}

int DODBot::GetGoalType()
{
	return W_FL_DOD_CAP;
}

bool DODBot::ShouldCapturePoint( edict_t * pControlPoint )
{
	if( !strcmp(STRING(pControlPoint->v.model), "models/mapmodels/flags.mdl"))
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
	}
	else
	{
		ALERT( at_error, "Capture point has unsupported flag model: %s\n", STRING(pControlPoint->v.model) );
		return false;
	}

	return false;
}