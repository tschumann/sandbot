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

extern int max_team_players[4];
extern int team_class_limits[4];
extern int max_teams;


void BotStartGame( bot_t *pBot )
{
   char c_team[32];
   char c_class[32];
   int index, count, retry_count;
   edict_t *pPlayer;
   int team;
   int class_not_allowed;

   edict_t *pEdict = pBot->pEdict;

   if (mod_id == TFC_DLL)
   {
      if ((pBot->start_action == MSG_TFC_IDLE) && (pBot->create_time + 3.0 <= gpGlobals->time))
      {
         pBot->start_action = MSG_TFC_TEAM_SELECT;  // force team selection
      }

      // handle Team Fortress Classic stuff here...

      if (pBot->start_action == MSG_TFC_TEAM_SELECT)
      {
         pBot->start_action = MSG_TFC_IDLE;  // switch back to idle
         pBot->create_time = gpGlobals->time;  // reset

         if ((pBot->bot_team != 1) && (pBot->bot_team != 2) && (pBot->bot_team != 3) && (pBot->bot_team != 4) && (pBot->bot_team != 5))
            pBot->bot_team = -1;

         if (pBot->bot_team == -1)
            pBot->bot_team = RANDOM_LONG(1, max_teams);

         retry_count = 0;

         while ((retry_count < 4) && (max_team_players[pBot->bot_team-1] > 0))  // not unlimited?
         {
            count = 0;

            // count number of players on this team...
            for (index = 1; index <= gpGlobals->maxClients; index++)
            {
               pPlayer = INDEXENT(index);

               if (pPlayer && !pPlayer->free)
               {
                  if (UTIL_GetTeam(pPlayer) == (pBot->bot_team - 1))
                     count++;
               }
            }

            if (count < max_team_players[pBot->bot_team-1])
               break;  // haven't reached limit yet, continue
            else
            {
               pBot->bot_team++;

               if (pBot->bot_team > max_teams)
                  pBot->bot_team = 1;

               retry_count++;
            }
         }

         // select the team the bot wishes to join...
         if (pBot->bot_team == 1)
            strcpy(c_team, "1");
         else if (pBot->bot_team == 2)
            strcpy(c_team, "2");
         else if (pBot->bot_team == 3)
            strcpy(c_team, "3");
         else if (pBot->bot_team == 4)
            strcpy(c_team, "4");
         else
            strcpy(c_team, "5");

         FakeClientCommand(pEdict, "jointeam", c_team, NULL);

         return;
      }

      if (pBot->start_action == MSG_TFC_CLASS_SELECT)
      {
         pBot->start_action = MSG_TFC_IDLE;  // switch back to idle
         pBot->create_time = gpGlobals->time;  // reset

         if ((pBot->bot_class < 0) || (pBot->bot_class > 10))
            pBot->bot_class = -1;

         if (pBot->bot_class == -1)
            pBot->bot_class = RANDOM_LONG(1, 10);

         team = UTIL_GetTeam(pEdict);

         if (team_class_limits[team] == -1)  // civilian only?
         {
            pBot->bot_class = 0;  // civilian
         }
         else
         {
            if (pBot->bot_class == 10)
               class_not_allowed = team_class_limits[team] & (1<<7);
            else if (pBot->bot_class <= 7)
               class_not_allowed = team_class_limits[team] & (1<<(pBot->bot_class-1));
            else
               class_not_allowed = team_class_limits[team] & (1<<(pBot->bot_class));

            while (class_not_allowed)
            {
               pBot->bot_class = RANDOM_LONG(1, 10);

               if (pBot->bot_class == 10)
                  class_not_allowed = team_class_limits[team] & (1<<7);
               else if (pBot->bot_class <= 7)
                  class_not_allowed = team_class_limits[team] & (1<<(pBot->bot_class-1));
               else
                  class_not_allowed = team_class_limits[team] & (1<<(pBot->bot_class));
            }
         }

         // select the class the bot wishes to use...
         if (pBot->bot_class == 0)
            strcpy(c_class, "civilian");
         else if (pBot->bot_class == 1)
            strcpy(c_class, "scout");
         else if (pBot->bot_class == 2)
            strcpy(c_class, "sniper");
         else if (pBot->bot_class == 3)
            strcpy(c_class, "soldier");
         else if (pBot->bot_class == 4)
            strcpy(c_class, "demoman");
         else if (pBot->bot_class == 5)
            strcpy(c_class, "medic");
         else if (pBot->bot_class == 6)
            strcpy(c_class, "hwguy");
         else if (pBot->bot_class == 7)
            strcpy(c_class, "pyro");
         else if (pBot->bot_class == 8)
            strcpy(c_class, "spy");
         else if (pBot->bot_class == 9)
            strcpy(c_class, "engineer");
         else
            strcpy(c_class, "randompc");

         FakeClientCommand(pEdict, c_class, NULL, NULL);

         // bot has now joined the game (doesn't need to be started)
         pBot->not_started = 0;

         return;
      }
   }
   else if (mod_id == CSTRIKE_DLL)
   {
      // handle Counter-Strike stuff here...

      if (pBot->start_action == MSG_CS_TEAM_SELECT)
      {
         pBot->start_action = MSG_CS_IDLE;  // switch back to idle

         if ((pBot->bot_team != 1) && (pBot->bot_team != 2) && (pBot->bot_team != 5))
            pBot->bot_team = -1;

         if (pBot->bot_team == -1)
            pBot->bot_team = RANDOM_LONG(1, 2);

         // select the team the bot wishes to join...
         if (pBot->bot_team == 1)
            strcpy(c_team, "1");
         else if (pBot->bot_team == 2)
            strcpy(c_team, "2");
         else
            strcpy(c_team, "5");

         FakeClientCommand(pEdict, "menuselect", c_team, NULL);

         return;
      }

      if (pBot->start_action == MSG_CS_CT_SELECT)  // counter terrorist
      {
         pBot->start_action = MSG_CS_IDLE;  // switch back to idle

         if ((pBot->bot_class < 1) || (pBot->bot_class > 4))
            pBot->bot_class = -1;  // use random if invalid

         if (pBot->bot_class == -1)
            pBot->bot_class = RANDOM_LONG(1, 4);

         // select the class the bot wishes to use...
         if (pBot->bot_class == 1)
            strcpy(c_class, "1");
         else if (pBot->bot_class == 2)
            strcpy(c_class, "2");
         else if (pBot->bot_class == 3)
            strcpy(c_class, "3");
         else if (pBot->bot_class == 4)
            strcpy(c_class, "4");
         else
            strcpy(c_class, "5");  // random

         FakeClientCommand(pEdict, "menuselect", c_class, NULL);

         // bot has now joined the game (doesn't need to be started)
         pBot->not_started = 0;

         return;
      }

      if (pBot->start_action == MSG_CS_T_SELECT)  // terrorist select
      {
         pBot->start_action = MSG_CS_IDLE;  // switch back to idle

         if ((pBot->bot_class < 1) || (pBot->bot_class > 4))
            pBot->bot_class = -1;  // use random if invalid

         if (pBot->bot_class == -1)
            pBot->bot_class = RANDOM_LONG(1, 4);

         // select the class the bot wishes to use...
         if (pBot->bot_class == 1)
            strcpy(c_class, "1");
         else if (pBot->bot_class == 2)
            strcpy(c_class, "2");
         else if (pBot->bot_class == 3)
            strcpy(c_class, "3");
         else if (pBot->bot_class == 4)
            strcpy(c_class, "4");
         else
            strcpy(c_class, "5");  // random

         FakeClientCommand(pEdict, "menuselect", c_class, NULL);

         // bot has now joined the game (doesn't need to be started)
         pBot->not_started = 0;

         return;
      }
   }
   else if ((mod_id == GEARBOX_DLL) && ((GearboxGame *)pGame)->IsCTF())
   {
      // handle Opposing Force CTF stuff here...

      if (pBot->start_action == MSG_OPFOR_TEAM_SELECT)
      {
         pBot->start_action = MSG_OPFOR_IDLE;  // switch back to idle

         if ((pBot->bot_team != 1) && (pBot->bot_team != 2) &&
             (pBot->bot_team != 3))
            pBot->bot_team = -1;

         if (pBot->bot_team == -1)
            pBot->bot_team = RANDOM_LONG(1, 2);

         // select the team the bot wishes to join...
         if (pBot->bot_team == 1)
            strcpy(c_team, "1");
         else if (pBot->bot_team == 2)
            strcpy(c_team, "2");
         else
            strcpy(c_team, "3");

         FakeClientCommand(pEdict, "jointeam", c_team, NULL);

         return;
      }

      if (pBot->start_action == MSG_OPFOR_CLASS_SELECT)
      {
         pBot->start_action = MSG_OPFOR_IDLE;  // switch back to idle

         if ((pBot->bot_class < 0) || (pBot->bot_class > 10))
            pBot->bot_class = -1;

         if (pBot->bot_class == -1)
            pBot->bot_class = RANDOM_LONG(1, 10);

         // select the class the bot wishes to use...
         if (pBot->bot_class == 1)
            strcpy(c_class, "1");
         else if (pBot->bot_class == 2)
            strcpy(c_class, "2");
         else if (pBot->bot_class == 3)
            strcpy(c_class, "3");
         else if (pBot->bot_class == 4)
            strcpy(c_class, "4");
         else if (pBot->bot_class == 5)
            strcpy(c_class, "5");
         else if (pBot->bot_class == 6)
            strcpy(c_class, "6");
         else
            strcpy(c_class, "7");

         FakeClientCommand(pEdict, "selectchar", c_class, NULL);

         // bot has now joined the game (doesn't need to be started)
         pBot->not_started = 0;

         return;
      }
   }
	else if( mod_id == DOD_DLL || mod_id == NS_DLL)
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

