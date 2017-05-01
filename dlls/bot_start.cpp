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

extern int mod_id;

void BotStartGame( bot_t *pBot )
{
	pBot->Join();
}

