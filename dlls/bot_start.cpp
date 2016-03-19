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

extern int g_iMod;

void BotStartGame( bot_t *pBot )
{
	// don't need to do anything to start game...
	pBot->not_started = 0;
}

