//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_start.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_weapons.h"

extern int mod_id;

void BotStartGame( bot_t *pBot )
{
	if( mod_id == CSTRIKE_DLL || mod_id == DOD_DLL || mod_id == TFC_DLL || mod_id == NS_DLL )
	{
		pBot->Join();
		return;
	}
	else if( (mod_id == GEARBOX_DLL) && pGame->IsCTF() )
	{
		pBot->Join();
		return;
	}
	else
	{
		// otherwise, don't need to do anything to start game...
		pBot->not_started = 0;
	}
}

