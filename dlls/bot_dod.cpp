#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "waypoint.h"

DODBot::DODBot()
{
}

void DODBot::OnSpawn()
{
	bot_t::OnSpawn();

	this->bCapturing = false;
	this->iGoalIndex = 0;
}

void DODBot::Join()
{
	if( this->start_action == MSG_DOD_TEAM_SELECT )
	{
		// switch back to idle
		this->start_action = MSG_DOD_IDLE;

		if( this->bot_team != DODBot::TEAM_ALLIES && this->bot_team != DODBot::TEAM_AXIS )
		{
			this->bot_team = TEAM_UNKNOWN;
		}

		// TODO: count how many each team has because even teams seems to be off by default
		if( this->bot_team == TEAM_UNKNOWN )
		{
			this->bot_team = RANDOM_LONG(DODBot::TEAM_ALLIES, DODBot::TEAM_AXIS);
		}

		// select the team the bot wishes to join...
		FakeClientCommand( this->pEdict, "jointeam %d", this->bot_team );

		return;
	}
	else if( this->start_action == MSG_DOD_ALLIED_SELECT )
	{
		this->start_action = MSG_DOD_IDLE;  // switch back to idle

		if (this->bot_class == -1)
		{
			if( ((DODGame *)pGame)->AreAlliesBritish() )
			{
				this->bot_class = RANDOM_LONG(DODBot::RIFLEMAN_BRITISH, DODBot::SERGEANT_MAJOR_BRITISH);
			}
			else
			{
				// do this to control what classes bots will spawn as (not everything is supported yet)
				this->bot_class = RANDOM_LONG(1, 4);
			}
		}

		switch( this->bot_class )
		{
		case 1:
			FakeClientCommand( this->pEdict, "cls_garand");
			break;
		case 2:
			FakeClientCommand( this->pEdict, "cls_carbine" );
			break;
		case 3:
			FakeClientCommand( this->pEdict, "cls_tommy" );
			break;
		case 4:
			FakeClientCommand( this->pEdict, "cls_grease" );
			break;
		case 5: // 8
			FakeClientCommand( this->pEdict, "cls_bazooka" );
			break;
		default:
			FakeClientCommand( this->pEdict, "cls_random" );
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
			this->bot_class = RANDOM_LONG(1, 4);
		}

		switch( this->bot_class )
		{
		case 1:
			FakeClientCommand( this->pEdict, "cls_k98" );
			break;
		case 2:
			FakeClientCommand( this->pEdict, "cls_k43" );
			break;
		case 3:
			FakeClientCommand( this->pEdict, "cls_mp40" );
			break;
		case 4:
			FakeClientCommand( this->pEdict, "cls_mp44" );
			break;
		case 5: // 8
			FakeClientCommand( this->pEdict, "cls_pshreck" );
			break;
		default:
			FakeClientCommand( this->pEdict, "cls_random" );
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
		this->SetSpeed( 0.0 );

		// TODO: this is very rough - probably something is set in pev if the bot is
		// on or near a dod_control_point - should check if it's a brush entity...
		if( DistanceToNearest(this->pEdict->v.origin, "dod_control_point") > 200 )
		{
			UTIL_LogDPrintf( "too far from capture point while capturing; resetting\n" );
			this->bCapturing = false;
			this->SetSpeed( this->GetMaxSpeed() );
		}
	}

	// TODO: waypoint goal changes once it's capturing?
	// if the current waypoint is a capture point and it is now captured
	if( this->bCapturing && ShouldSkip(this->pEdict, this->iGoalIndex) )
	{
		UTIL_LogDPrintf( "leaving waypoint\n" );
		this->bCapturing = false;
		this->SetSpeed( this->GetMaxSpeed() );
	}

	bot_t::PostThink();
}

float DODBot::GetSpeedToEnemy()
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
	else
	{
		fSpeed = 0.0;
	}

	return fSpeed;
}

void DODBot::Reload()
{
	bot_t::Reload();

	UTIL_HostSay( this->pEdict, TRUE, "reloading" );
}

bool DODBot::IsSniping()
{
	// TODO: work out the weapon ids for the sniper rifles
	return false;
}

float DODBot::GetMaxSpeed()
{
	return pEdict->v.maxspeed;
}

float DODBot::GetSpeed()
{
	// TODO: maxspeed for Day of Defeat is probably sprint speed - find out regular run speed
	return 300;
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
		if( pControlPoint->v.body == 0 && pGame->GetTeam( this->pEdict ) == DODBot::TEAM_ALLIES )
		{
			return true;
		}

		// if it's currently captured by the Allies and the player is Axis
		if( pControlPoint->v.body == 1 && pGame->GetTeam( this->pEdict ) == DODBot::TEAM_AXIS )
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
