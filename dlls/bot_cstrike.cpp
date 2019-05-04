#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"
#include "wpt.h"
#include "waypoint.h"

CStrikeBot::CStrikeBot()
{
}

void CStrikeBot::Join()
{
	if (this->start_action == MSG_CS_TEAM_SELECT)
	{
		this->start_action = MSG_CS_IDLE;  // switch back to idle

		if ((this->bot_team != 1) && (this->bot_team != 2) && (this->bot_team != 5))
			this->bot_team = -1;

		if (this->bot_team == -1)
			this->bot_team = RANDOM_LONG(1, 2);

		// select the team the bot wishes to join...
		if( this->bot_team != 1 && this->bot_team != 2 )
			this->bot_team = 5;

		FakeClientCommand(pEdict, "menuselect %d", this->bot_team);

		return;
	}

	if (this->start_action == MSG_CS_CT_SELECT)  // counter terrorist
	{
		this->start_action = MSG_CS_IDLE;  // switch back to idle

		if ((this->bot_class < 1) || (this->bot_class > 4))
			this->bot_class = -1;  // use random if invalid

		if (this->bot_class == -1)
			this->bot_class = RANDOM_LONG(1, 4);

		FakeClientCommand(pEdict, "menuselect %d", this->bot_class);

		// bot has now joined the game (doesn't need to be started)
		this->not_started = 0;

		return;
	}

	if (this->start_action == MSG_CS_T_SELECT)  // terrorist select
	{
		this->start_action = MSG_CS_IDLE;  // switch back to idle

		if ((this->bot_class < 1) || (this->bot_class > 4))
			this->bot_class = -1;  // use random if invalid

		if (this->bot_class == -1)
			this->bot_class = RANDOM_LONG(1, 4);

		FakeClientCommand(pEdict, "menuselect %d", this->bot_class);

		// bot has now joined the game (doesn't need to be started)
		this->not_started = 0;

		return;
	}
}

float CStrikeBot::GetMaxSpeed()
{
	return pEdict->v.maxspeed;
}
