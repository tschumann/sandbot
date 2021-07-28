//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot.cpp
//

#include "extdll.h"
#include "meta_api.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "game.h"
#include "wpt.h"
#include "waypoint.h"
#include "wpt.h"
#include "bot_weapons.h"
#include "h_export.h"

bot_player_t *pBotData = nullptr;
bot_t **pBots = nullptr; // [Game::MAX_PLAYERS];

bool b_observer_mode = false;

bot_player_t g_valveBots[Game::MAX_PLAYERS] =
{
	{"Kelly", "barney", false},
	{"Ted", "gina", false},
	{"Julie", "gman", false},
	{"Yahn", "gordon", false},
	{"Ken", "helmet", false},
	{"Steve", "hgrunt", false},
	{"Dario", "recon", false},
	{"Greg", "robo", false},
	{"Andrew", "scientist", false},
	{"Wes", "zombie", false},
	{"Matt", "barney", false},
	{"John", "gina", false},
	{"Mona Lisa", "gman", false},
	{"Mike", "gordon", false},
	{"Monica", "helmet", false},
	{"Brett", "hgrunt", false},
	{"Erik", "recon", false},
	{"Chuck", "robo", false},
	{"Phil", "scientist", false},
	{"Marc", "zombie", false},
	{"Karen", "barney", false},
	{"Dave", "gina", false},
	{"Miene", "gman", false},
	{"Randy", "gordon", false},
	{"Gabe", "helmet", false},
	{"Yatsze", "hgrunt", false},
	{"Lisa", "recon", false},
	{"Cade", "robo", false},
	{"Kate", "scientist", false},
	{"Torsten", "zombie", false},
	{"Jay", "barney", false},
	{"Harry", "gina", false}
};

bot_player_t g_gearboxBots[Game::MAX_PLAYERS] =
{
	{"Randy", "barney", false},
	{"Brian", "beret", false},
	{"John", "cl_suit", false},
	{"Rob", "drill", false},
	{"StephenB", "fassn", false},
	{"Landon", "gina", false},
	{"DavidM", "gman", false},
	{"MikeW", "gordon", false},
	{"Patrick", "grunt", false},
	{"Steve", "helmet", false},
	{"Sean", "hgrunt", false},
	{"StephenP", "massn", false},
	{"Kristy", "otis", false},
	{"Matt", "recon", false},
	{"Sean", "recruit", false},
	{"Ben",	"robo", false},				// Sierra
	{"Chris", "scientist", false},
	{"Harry", "shephard", false},		// voices
	{"MikeS", "tower", false},
	{"Jon", "zombie", false},
	{"JohnC", "barney", false},			// Valve continued
	{"Aaron", "beret", false},
	{"Robin", "cl_suit", false},
	{"Bill", "drill", false},
	{"Douglas", "fassn", false},
	{"Jeffrey",	"gina", false},			// botman
	{"Eric", "gordon", false},			// Allstars
	{"DavidK", "grunt", false},
	{"Tom", "helmet", false},
	{"Marin", "hgrunt", false},
	{"RichardG", "massn", false},
	{"RichardC", "otis", false}
};

bot_player_t g_cstrikeBots[Game::MAX_PLAYERS] =
{
	{"Minh", NULL, false},
	{"Jesse", NULL, false}
};

bot_player_t g_dodBots[Game::MAX_PLAYERS] =
{
	{"Matt", NULL, false},		// mugsy
	{"John", NULL, false},		// pickitup
	{"Kelly", NULL, false},		// thunder weenie
	{"Tim", NULL, false},		// Waldo
	{"Jake", NULL, false},		// molotov_billy
	{"Travis", NULL, false},	// agent-0
	{"Magnus", NULL, false},	// Insta
	{"Davide", NULL, false},	// Chow_Yun_Fat
	{"Iikka", NULL, false},		// Fingers
	{"Joel", NULL, false},		// c0w
	{"Chris", NULL, false},		// Narby
	{"Svante", NULL, false},	// xerent
	{"Arjan", NULL, false},		// IR
	{"Wes", NULL, false},		// FuzzDad
	{"Brian", NULL, false},		// Arcturus
	{"ChrisS", NULL, false},	// Unreal
	{"Arttu", NULL, false},		// skdr
	{"Jeremy", NULL, false},	// Izuno
	{"Patrick", NULL, false},	// Mojo
	{"Mike", NULL, false},		// MikeZilla
	{"David", NULL, false},		// Zaphod
	{"Jeff", NULL, false},		// ViciouS
	{"Michael", NULL, false},	// Dim Reaper
	{"MichaelG", NULL, false},	// Sector-Effector
	{"Sean", NULL, false},
	{"Unknown1", NULL, false},	// Panzergrenadier
	{"Unknown3", NULL, false},	// Kommie
	{"Unknown4", NULL, false},	// H&K - Sean?
	{"Unknown5", NULL, false},	// lovemeister
	{"Unknown6", NULL, false},	// Masako
	{"Unknown7", NULL, false},	// DasJuden
	{"Unknown8", NULL, false},	// Kamikazi
};

bot_player_t g_gunmanBots[Game::MAX_PLAYERS] =
{
	{"Herb", "bandit", false},		// BoneWolf
	{"Steven", "general", false},	// Wipeoot
	{"Rodney", "scientist", false},	// Hostage
	{"Tomaz", "soldier", false},	// ToM
	{"Reid", "bandit", false},		// BrushBoy
	{"Dale", "general", false},		// Mutley
	{"Renier", "scientist", false},	// Lando
	{"Adrian", "soldier", false},	// PUG
	{"BrianL", "bandit", false},	// Cannelbrae
	{"BrianB", "general", false},	// Sake
	{"ChrisM", "scientist", false},	// Ilian
	{"Mary", "soldier", false},		// Spur
	{"Miriam", "bandit", false},
	{"Christopher", "general", false},	// CyberAcid
	{"Steve", "scientist", false},	// Blevo
	{"Peter", "soldier", false},	// SoulSounds
	{"ChrisL", "bandit", false},
	{"Anthony", "general", false},	// Eyeque
	{"ChrisJ", "scientist", false},
	{"Aaron", "soldier", false},
	{"David", "bandit", false},
	{"Bill", "scientist", false},
	{"Cynthia", "soldier", false},
	{"Mary Kae", "bandit", false},
	{"Eric", "general", false},
	{"Dex", "scientist", false},
	{"Jim", "soldier", false},
	{"Ilian", "bandit", false},
	{"Joe", "general", false},
	{"Jeff", "scientist", false},
	{"Yahn", "bandit", false},
	{"Robin", "general", false}
};

bot_player_t g_nsBots[Game::MAX_PLAYERS] =
{
	{"Charlie", NULL, false},
	{"Jon", NULL, false},
	{"Joel", NULL, false},
	{"David", NULL, false},
	{"Harry", NULL, false},
	{"Petter", NULL, false},
	{"Jim", NULL, false},
	{"Jason", NULL, false},
	{"Cory", NULL, false},
	{"Josh", NULL, false},
	{"Mike", NULL, false},
	{"Jeff", NULL, false},
	{"Alex", NULL, false},
	{"Phil", NULL, false},
	{"Florian", NULL, false},
	{"Karl", NULL, false},
	{"Joe", NULL, false},
	{"Max", NULL, false},
	{"Kevin", NULL, false},
	{"Ned", NULL, false},
	{"Guy", NULL, false},
	{"Spencer", NULL, false},
	{"Tom", NULL, false},
	{"Joseph", NULL, false},
	{"Ozgur", NULL, false},
	{"Jeremy", NULL, false},
	{"Hugo", NULL, false},
	{"Lani", NULL, false},
	{"Oliver", NULL, false},
	{"Emmanuel", NULL, false},
	{"Matt", NULL, false},
	{"Nick", NULL, false}
};

bot_player_t g_hungerBots[Game::MAX_PLAYERS] =
{
	{"Bill", "civie", false},
	{"Dave", "dave", false},
	{"Einar", "einar", false},
	{"BDog", "einarhev", false},
	{"Eric", "franklin", false},
	{"Sebastian", "gangster", false},
	{"Jack", "jack", false},
	{"Magnus", "magnus", false},
	{"Neil", "neil", false},
	{"Aaron", "nohead", false},
	{"Ted", "nypdcop", false},
	{"Per", "orderly", false},
	{"Petter", "patient", false},
	{"Lo", "paul", false},
	{"Richard", "sheriff", false},
	{"Minh", "worker", false},
	{"Fabian", "zork", false},
	{"Dik", "civie", false},
	{"Bennet", "dave", false},
	{"David", "einar", false},
	{"Pete", "franklin", false},
	{"Nick", "gangster", false},
	{"Margaret", "jack", false},
	{"Jerry", "magnus", false},
	{"Andrew", "nohead", false},
	{"Don", "nypdcop", false},
	{"John", "orderly", false},
	{"Morgan", "patient", false},
	{"Jiang", "paul", false},
	{"Dennis", "sheriff", false},
	{"Brian", "worker", false},
	{"Ben", "zork", false}
};

bot_player_t g_shipBots[Game::MAX_PLAYERS] =
{
	{"Chris", NULL, false},
	{"Ailsa", NULL, false},
	{"Ed", NULL, false},
	{"Simon", NULL, false},
	{"Jason", NULL, false},
	{"Neil", NULL, false},
	{"Andy", NULL, false},
	{"David", NULL, false},
	{"Jock", NULL, false},
	{"Gayle", NULL, false},
	{"Debbie", NULL, false},
	{"Mike", NULL, false},
	{"Nick", NULL, false},
	{"Mark", NULL, false},
	{"Dorota", NULL, false},
	{"Jonathon", NULL, false},
	{"Julian", NULL, false},
	{"Richard", NULL, false},
	{"Duncan", NULL, false},
	{"Rebecca", NULL, false},
	{"Andrea", NULL, false},
	{"Julie", NULL, false},
	{"Glen", NULL, false},
	{"Ellis", NULL, false},
	{"Edward", NULL, false},
	{"Alek", NULL, false},
	{"Casey", NULL, false},
	{"Suzie", NULL, false},
	{"Caro", NULL, false},
	{"Reanne", NULL, false},
	{"Lucy", NULL, false},
	{"Toby", NULL, false}
};

// how often (out of 1000 times) the bot will pause, based on bot skill
float pause_frequency[5] = {4, 7, 10, 15, 20};

float pause_time[5][2] = {{0.2f, 0.5f}, {0.5f, 1.0f}, {0.7f, 1.3f}, {1.0f, 1.7f}, {1.2f, 2.0f}};

// TheFatal's method for calculating the msecval
extern float msecval;

int GetBotCount()
{
	int count = 0;

	for( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		edict_t *pPlayer = INDEXENT(i);

		if( pPlayer && !pPlayer->free )
		{
			if( FBitSet(pPlayer->v.flags, FL_CLIENT) && IsValidEntity( pPlayer ) )
			{
				count++;
			}
		}
	}

	return count;
}

void KickBot( const int iIndex )
{
	// if the slot is in use
	if( pBots && pBots[iIndex]->is_used )
	{
		char szCmd[64];

		sprintf( szCmd, "kick \"%s\"\n", pBots[iIndex]->name );

		pBots[iIndex]->is_used = false;
		pBotData[pBots[iIndex]->iBotDataIndex].bIsUsed = false;

		// kick the bot using (kick "name")
		SERVER_COMMAND(szCmd);
	}
	else
	{
		ALERT( at_warning, "Bot at index %d is not in use; not kicking\n", iIndex );
	}
}

void KickAllBots()
{
	for( unsigned int index = 0; index < Game::MAX_PLAYERS; index++ )
	{
		KickBot( index );
	}
}

void CleanupGameAndBots()
{
	if( pBotData )
	{
		for( int i = 0; i < Game::MAX_PLAYERS; i++ )
		{
			pBotData[i].bIsUsed = false;
		}
	}
	if( pBots )
	{
		for( int i = 0; i < Game::MAX_PLAYERS; i++ )
		{
			delete pBots[i];
			pBots[i] = nullptr;
		}
		pBots = nullptr;
	}
}

// this is the LINK_ENTITY_TO_CLASS function that creates a player (bot)
void player( entvars_t *pev )
{
	static LINK_ENTITY_FUNC pClassName = nullptr;

	if( h_Library != nullptr && pClassName == nullptr )
	{
		pClassName = (LINK_ENTITY_FUNC)GetProcAddress(h_Library, "player");
	}
	if( pClassName != nullptr )
	{
		(*pClassName)(pev);
	}
}

void BotSpawnInit( bot_t *pBot )
{
	pBot->OnSpawn();

	pBot->v_prev_origin = Vector(9999.0, 9999.0, 9999.0);
	pBot->prev_time = gpGlobals->time;

	pBot->waypoint_origin = Vector(0, 0, 0);
	pBot->f_waypoint_time = 0.0;
	pBot->curr_waypoint_index = -1;
	pBot->prev_waypoint_index[0] = -1;
	pBot->prev_waypoint_index[1] = -1;
	pBot->prev_waypoint_index[2] = -1;
	pBot->prev_waypoint_index[3] = -1;
	pBot->prev_waypoint_index[4] = -1;

	pBot->f_random_waypoint_time = gpGlobals->time;
	pBot->waypoint_goal = -1;
	pBot->f_waypoint_goal_time = 0.0;
	pBot->waypoint_near_flag = FALSE;
	pBot->waypoint_flag_origin = Vector(0, 0, 0);
	pBot->prev_waypoint_distance = 0.0;

	pBot->blinded_time = 0.0;

	pBot->prev_speed = 0.0;  // fake "paused" since bot is NOT stuck

	pBot->f_find_item = 0.0;

	pBot->ladder_dir = LADDER_UNKNOWN;
	pBot->f_start_use_ladder_time = 0.0;
	pBot->f_end_use_ladder_time = 0.0;
	pBot->waypoint_top_of_ladder = FALSE;

	pBot->f_wall_check_time = 0.0;
	pBot->f_wall_on_right = 0.0;
	pBot->f_wall_on_left = 0.0;
	pBot->f_dont_avoid_wall_time = 0.0;
	pBot->f_look_for_waypoint_time = 0.0;
	pBot->f_jump_time = 0.0;
	pBot->f_dont_check_stuck = 0.0;

	// pick a wander direction (50% of the time to the left, 50% to the right)
	if (RANDOM_LONG(1, 100) <= 50)
	{
		pBot->wander_dir = WANDER_LEFT;
	}
	else
	{
		pBot->wander_dir = WANDER_RIGHT;
	}

	pBot->f_exit_water_time = 0.0;

	pBot->pBotEnemy = NULL;
	pBot->f_bot_see_enemy_time = gpGlobals->time;
	pBot->f_bot_find_enemy_time = gpGlobals->time;
	pBot->pBotUser = NULL;
	pBot->f_bot_use_time = 0.0;
	pBot->b_bot_say_killed = FALSE;
	pBot->f_bot_say_killed = 0.0;
	pBot->f_sniper_aim_time = 0.0;

	pBot->f_shoot_time = gpGlobals->time;
	pBot->f_primary_charging = -1.0;
	pBot->f_secondary_charging = -1.0;
	pBot->charging_weapon_id = 0;

	pBot->f_pause_time = 0.0;
	pBot->f_sound_update_time = 0.0;
	pBot->bBotHasFlag = false;

	pBot->b_see_tripmine = FALSE;
	pBot->b_shoot_tripmine = FALSE;
	pBot->v_tripmine = Vector(0,0,0);

	pBot->b_use_health_station = FALSE;
	pBot->f_use_health_time = 0.0;
	pBot->b_use_HEV_station = FALSE;
	pBot->f_use_HEV_time = 0.0;

	pBot->b_use_button = FALSE;
	pBot->f_use_button_time = 0;
	pBot->b_lift_moving = FALSE;

	pBot->b_use_capture = FALSE;
	pBot->f_use_capture_time = 0.0;
	pBot->pCaptureEdict = NULL;

	// The Ship
	pBot->bUseDoor = false;
	pBot->fUseDoorTime = 0.0;

	pBot->current_weapon.iAmmo1 = 0;
	pBot->current_weapon.iAmmo2 = 0;
	pBot->current_weapon.iClip = 0;
	pBot->current_weapon.iId = 0;

	for( int i = 0; i < MAX_AMMO_SLOTS; i++ )
	{
		pBot->m_rgAmmo[i] = 0;
	}
}

void BotCreate( edict_t *pPlayer, const char *arg1, const char *arg2, const char *arg3, const char *arg4)
{
	bot_t *pBot;
	int start_action = 0;

	int iIndex = RANDOM_LONG( 0, 31 );

	// find an unused bot data slot
	while( pBotData[iIndex].bIsUsed == true )
	{
		iIndex++;

		// loop around
		if( iIndex == Game::MAX_PLAYERS )
		{
			iIndex = 0;
		}
	}

	// TODO: move this to a utility function: it may be useful
	// remove any illegal characters from name...
	/*
	for (i = 0; i < length; i++)
	{
		if ((c_name[i] <= ' ') || (c_name[i] > '~') || (c_name[i] == '"'))
		{
			for (j = i; j < length; j++)  // shuffle chars left (and null)
				c_name[j] = c_name[j+1];
			length--;
		}
	}
	*/

	edict_t *pBotEdict = CREATE_FAKE_CLIENT( pBotData[iIndex].szName );

	if( FNullEnt( pBotEdict ) )
	{
		if( pPlayer )
		{
			ClientPrint( pPlayer, HUD_PRINTNOTIFY, "Cannot create bot - player limit reached\n");
		}

		return;
	}
   else
   {
      char ptr[128];  // allocate space for message from ClientConnect
      char *infobuffer;
      int clientIndex;
      int index;

      if (IS_DEDICATED_SERVER())
         printf("Creating bot...\n");
      else if (pPlayer)
         ClientPrint( pPlayer, HUD_PRINTNOTIFY, "Creating bot...\n");

	  // Make sure that when creating new bot, the previous one private data is freed
	  if (pBotEdict->pvPrivateData != NULL)
	  {
		  FREE_PRIVATE(pBotEdict);
	  }
	  pBotEdict->pvPrivateData = NULL;
	  pBotEdict->v.frags = 0;

      index = 0;
	  while ((index < Game::MAX_PLAYERS) && (pBots[index]->is_used))
	  {
		  index++;
	  }

      if (index == Game::MAX_PLAYERS)
      {
         ClientPrint( pPlayer, HUD_PRINTNOTIFY, "Can't create bot!\n");
         return;
      }

      // create the player entity by calling MOD's player function
      // (from LINK_ENTITY_TO_CLASS for player object)
	  if( g_bIsMMPlugin )
		  CALL_GAME_ENTITY( PLID, "player", VARS(pBotEdict) );
	  else
		  player( VARS(pBotEdict) );

      infobuffer = GET_INFOBUFFER( pBotEdict );
      clientIndex = ENTINDEX( pBotEdict );

	  ALERT( at_console, "Using bot data %d for client %d\n", iIndex, clientIndex );

      if (pGame->CanChoosePlayerModel())
	  {
		  char szColour[4];

		  SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "model", pBotData[iIndex].szModel );
		  // the engine supports this but not all modes have customisable player models - set it anyway
		  sprintf( szColour, "%d", RANDOM_LONG(0, 255) );
		  SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "topcolor", szColour );
		  sprintf( szColour, "%d", RANDOM_LONG(0, 255) );
		  SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "bottomcolor", szColour );
	  }
      else
	  {
			// TODO: is this even needed?
			SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "model", "" );
	  }

      if (mod_id == CSTRIKE_DLL)
      {
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "rate", "3500.000000");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "cl_updaterate", "20");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "cl_lw", "1");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "cl_lc", "1");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "tracker", "0");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "cl_dlmax", "128");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "lefthand", "1");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "friends", "0");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "dm", "0");
         SET_CLIENT_KEY_VALUE( clientIndex, infobuffer, "ah", "1");
      }

      MDLL_ClientConnect( pBotEdict, pBotData[iIndex].szName, "127.0.0.1", ptr );

      // Pieter van Dijk - use instead of DispatchSpawn() - Hip Hip Hurray!
	  MDLL_ClientPutInServer( pBotEdict );

      pBotEdict->v.flags |= FL_FAKECLIENT;

      // initialize all the variables for this bot...

		pBot = pBots[index];
		pBot->index = clientIndex;

		// mark the bot data slot as used
		pBotData[iIndex].bIsUsed = true;
		// tie this bot back to the bot data slot it's using
		pBot->iBotDataIndex = iIndex;
		pBot->is_used = TRUE;
		pBot->respawn_state = RESPAWN_IDLE;
		pBot->create_time = gpGlobals->time;
		pBot->name[0] = 0;  // name not set by server yet
		pBot->bot_money = 0;

		if( pBotData[iIndex].szModel )
		{
			strcpy( pBot->skin, pBotData[iIndex].szModel );
		}
		else
		{
			strcpy( pBot->skin, "" );
		}

		pBot->pEdict = pBotEdict;

		pBot->not_started = 1;  // hasn't joined game yet

		if (mod_id == TFC_DLL)
			pBot->start_action = MSG_TFC_IDLE;
		else if (mod_id == CSTRIKE_DLL)
			pBot->start_action = MSG_CS_IDLE;
		else if (mod_id == DOD_DLL)
			pBot->start_action = MSG_DOD_IDLE;
		else if ((mod_id == GEARBOX_DLL) && (pGame->IsCTF() || pGame->IsCapturePoint()))
			pBot->start_action = MSG_OPFOR_IDLE;
		else if (mod_id == NS_DLL && start_action != 0)
		{
			pBot->start_action = start_action;
		}
		else
			pBot->start_action = 0;  // not needed for non-team MODs


		BotSpawnInit(pBot);

		pBot->bShouldInit = FALSE;  // don't need to initialize yet

		pBotEdict->v.idealpitch = pBotEdict->v.v_angle.x;
		pBotEdict->v.ideal_yaw = pBotEdict->v.v_angle.y;
		pBotEdict->v.pitch_speed = BOT_PITCH_SPEED;
		pBotEdict->v.yaw_speed = BOT_YAW_SPEED;

		pBot->idle_angle = 0.0;
		pBot->idle_angle_time = 0.0;

		pBot->bot_team = -1;
		pBot->bot_class = -1;

		if( mod_id == NS_DLL )
		{
			// decide randomly which class to become
			((NSBot *)pBot)->ChooseDesiredClass();
		}

	  	if( arg1 && (*arg1 != 0) )
		{
			if( mod_id == DOD_DLL )
			{
				if( !strcmp(arg1, "allies") )
				{
					pBot->bot_team = DODBot::TEAM_ALLIES;
				}
				else if( !strcmp(arg1, "axis") )
				{
					pBot->bot_team = DODBot::TEAM_AXIS;
				}
				else
				{
					ALERT( at_error, "Unknown team name %s\n", arg1 );
				}
			}
			else if( mod_id == NS_DLL )
			{
				if( !strcmp(arg1, "alien") )
				{
					start_action = MSG_NS_JOIN_ALIEN;
				}
				else if( !strcmp(arg1, "marine") )
				{
					start_action = MSG_NS_JOIN_MARINE;
				}
				else
				{
					start_action = MSG_NS_JOIN_AUTO;
				}
			}
		}
		if( arg2 && (*arg2 != 0) )
		{
			// TODO: expand this
			if( mod_id == DOD_DLL )
			{
				if( !strcmp(arg2, "bazooka") )
				{
					pBot->bot_class = 5;
				}
				else if( !strcmp(arg2, "pshreck") )
				{
					pBot->bot_class = 5;
				}
				else
				{
					ALERT( at_error, "Unknown class name %s\n", arg1 );
				}
			}
		}

	  if ((mod_id == TFC_DLL) || (mod_id == CSTRIKE_DLL) || ((mod_id == GEARBOX_DLL) && (pGame->IsCTF() || pGame->IsCapturePoint())))
      {
         if ((arg1 != NULL) && (arg1[0] != 0))
         {
            pBot->bot_team = atoi(arg1);

            if ((arg2 != NULL) && (arg2[0] != 0))
            {
               pBot->bot_class = atoi(arg2);
            }
         }
      }
   }
}

void BotFindItem( bot_t *pBot )
{
   edict_t *pent = NULL;
   edict_t *pPickupEntity = NULL;
   Vector pickup_origin;
   Vector entity_origin;
   float radius = pBot->GetWaypointRadius();
   bool can_pickup;
   float min_distance;
   char item_name[40];
   TraceResult tr;
   Vector vecStart;
   Vector vecEnd;
   int angle_to_entity;
   edict_t *pEdict = pBot->pEdict;

   pBot->pBotPickupItem = NULL;

   min_distance = radius + 1.0;

   while ((pent = UTIL_FindEntityInSphere( pent, pEdict->v.origin, radius )) != NULL)
   {
      can_pickup = FALSE;  // assume can't use it until known otherwise

      strcpy(item_name, STRING(pent->v.classname));

      // see if this is a "func_" type of entity (func_button, etc.)...
      if (strncmp("func_", item_name, 5) == 0)
      {
         // BModels have 0,0,0 for origin so must use VecBModelOrigin...
         entity_origin = VecBModelOrigin(pent);

         vecStart = pEdict->v.origin + pEdict->v.view_ofs;
         vecEnd = entity_origin;

         angle_to_entity = pBot->GetAngleToPoint( vecEnd - vecStart );

         // check if entity is outside field of view (+/- 45 degrees)
         if (angle_to_entity > 45)
            continue;  // skip this item if bot can't "see" it

         // check if entity is a ladder (ladders are a special case)
         // DON'T search for ladders if there are waypoints in this level...
         if ((strcmp("func_ladder", item_name) == 0) && (num_waypoints == 0))
         {
            // force ladder origin to same z coordinate as bot since
            // the VecBModelOrigin is the center of the ladder.  For
            // LONG ladders, the center MAY be hundreds of units above
            // the bot.  Fake an origin at the same level as the bot...

            entity_origin.z = pEdict->v.origin.z;
            vecEnd = entity_origin;

            // trace a line from bot's eyes to func_ladder entity...
            UTIL_TraceLine( vecStart, vecEnd, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

            // check if traced all the way up to the entity (didn't hit wall)
            if (tr.flFraction >= 1.0)
            {
               // find distance to item for later use...
               float distance = (vecEnd - vecStart).Length( );

               // use the ladder about 100% of the time, if haven't
               // used a ladder in at least 5 seconds...
               if ((RANDOM_LONG(1, 100) <= 100) && ((pBot->f_end_use_ladder_time + 5.0) < gpGlobals->time))
               {
                  // if close to ladder...
                  if (distance < 100)
                  {
                     // don't avoid walls for a while
                     pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                  }

                  can_pickup = TRUE;
               }
            }
         }
         else
         {
            // trace a line from bot's eyes to func_ entity...
            UTIL_TraceLine( vecStart, vecEnd, dont_ignore_monsters, pEdict->v.pContainingEntity, &tr);

            // check if traced all the way up to the entity (didn't hit wall)
            if (strcmp(item_name, STRING(tr.pHit->v.classname)) == 0)
            {
               // find distance to item for later use...
               float distance = (vecEnd - vecStart).Length( );

               // check if entity is wall mounted health charger...
               if (strcmp("func_healthcharger", item_name) == 0)
               {
                  // check if the bot can use this item and
                  // check if the recharger is ready to use (has power left)...
                  if ((pEdict->v.health < 100) && (pent->v.frame == 0))
                  {
                     // check if flag not set...
                     if (!pBot->b_use_health_station)
                     {
                        // check if close enough and facing it directly...
                        if ((distance < bot_t::PLAYER_SEARCH_RADIUS) && (angle_to_entity <= 10))
                        {
                           pBot->b_use_health_station = TRUE;
                           pBot->f_use_health_time = gpGlobals->time;
                        }

                        // if close to health station...
                        if (distance < 100)
                        {
                           // don't avoid walls for a while
                           pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                        }

                        can_pickup = TRUE;
                     }
                  }
                  else
                  {
                     // don't need or can't use this item...
                     pBot->b_use_health_station = FALSE;
                  }
               }

               // check if entity is wall mounted HEV charger...
               else if (strcmp("func_recharge", item_name) == 0)
               {
                  // check if the bot can use this item and
                  // check if the recharger is ready to use (has power left)...
                  if ((pEdict->v.armorvalue < VALVE_MAX_NORMAL_BATTERY) && (pent->v.frame == 0))
                  {
                     // check if flag not set and facing it...
                     if (!pBot->b_use_HEV_station)
                     {
                        // check if close enough and facing it directly...
                        if ((distance < bot_t::PLAYER_SEARCH_RADIUS) && (angle_to_entity <= 10))
                        {
                           pBot->b_use_HEV_station = TRUE;
                           pBot->f_use_HEV_time = gpGlobals->time;
                        }

                        // if close to HEV recharger...
                        if (distance < 100)
                        {
                           // don't avoid walls for a while
                           pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                        }

                        can_pickup = TRUE;
                     }
                  }
                  else
                  {
                     // don't need or can't use this item...
                     pBot->b_use_HEV_station = FALSE;
                  }
               }

               // check if entity is a button...
               else if (strcmp("func_button", item_name) == 0)
               {
                  // use the button about 100% of the time, if haven't
                  // used a button in at least 5 seconds...
                  if ((RANDOM_LONG(1, 100) <= 100) && ((pBot->f_use_button_time + 5) < gpGlobals->time))
                  {
                     // check if flag not set and facing it...
                     if (!pBot->b_use_button)
                     {
                        // check if close enough and facing it directly...
                        if ((distance < bot_t::PLAYER_SEARCH_RADIUS) && (angle_to_entity <= 10))
                        {
                           pBot->b_use_button = TRUE;
                           pBot->b_lift_moving = FALSE;
                           pBot->f_use_button_time = gpGlobals->time;
                        }

                        // if close to button...
                        if (distance < 100)
                        {
                           // don't avoid walls for a while
                           pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                        }

                        can_pickup = TRUE;
                     }
                  }
                  else
                  {
                     // don't need or can't use this item...
                     pBot->b_use_button = FALSE;
                  }
               }
			   else if( pGame->UseToOpenDoor() && !strcmp( "func_door", item_name ) )
                {
                    // use the button about 100% of the time, if haven't
                    // used a button in at least 5 seconds...
                    if( ( RANDOM_LONG(1, 100) <= 100 ) && ( ( pBot->f_use_button_time + 5.0 ) < gpGlobals->time ) )
                    {
                        // check if flag not set and facing it...
                        if( !pBot->bUseDoor )
                        {
                            // check if close enough and facing it directly...
                            if( ( distance < bot_t::PLAYER_SEARCH_RADIUS ) && ( angle_to_entity <= 10 ) )
                            {
                                pBot->bUseDoor = TRUE;
                                pBot->fUseDoorTime = gpGlobals->time;
                            }

                            // if close to door...
                            if( distance < 100.0 )
                            {
                                // don't avoid walls for a while
                                pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
                            }

                            can_pickup = TRUE;
                        }
                    }
                    else
                    {
                        // don't need or can't use this item...
                        pBot->bUseDoor = FALSE;
                    }
                }
				else if( mod_id == NS_DLL )
				{
					// check if the entity is a resource node and the player is a gorge
					if( ((NSBot *)pBot)->IsAlien() && ((NSBot *)pBot)->IsGorge() && !strcmp( "func_resource", item_name ) )
					{
						// check if flag not set and facing it...
						if( !pBot->bBuildAlienResourceTower )
						{
							// check if close enough and facing it directly and has enough resource
							if( ( distance < bot_t::PLAYER_SEARCH_RADIUS * 2 ) && ( angle_to_entity <= 10 ) && ((NSBot *)pBot)->GetResources() >= kAlienResourceTowerCost )
							{
								pBot->bBuildAlienResourceTower = TRUE;
							}

							// if close
							if( distance < 100.0 )
							{
								// don't avoid walls for a while
								pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;
							}

							can_pickup = TRUE;
						}
						else
						{
							// don't need or can't use this item...
							pBot->bBuildAlienResourceTower = FALSE;
						}
					}
				}
            }
         }
      }
      else  // everything else...
      {
         entity_origin = pent->v.origin;

         vecStart = pEdict->v.origin + pEdict->v.view_ofs;
         vecEnd = entity_origin;

         // find angles from bot origin to entity...
         angle_to_entity = pBot->GetAngleToPoint( vecEnd - vecStart );

         // check if entity is outside field of view (+/- 45 degrees)
         if (angle_to_entity > 45)
            continue;  // skip this item if bot can't "see" it

         // check if line of sight to object is not blocked (i.e. visible)
         if (pBot->CanSeePoint( vecEnd ))
         {
            // check if entity is a weapon...
            if (strncmp("weapon_", item_name, 7) == 0)
            {
               if (pent->v.effects & EF_NODRAW)
               {
                  // someone owns this weapon or it hasn't respawned yet
                  continue;
               }

               can_pickup = TRUE;
            }

            // check if entity is ammo...
            else if (strncmp("ammo_", item_name, 5) == 0)
            {
               // check if the item is not visible (i.e. has not respawned)
               if (pent->v.effects & EF_NODRAW)
                  continue;

               can_pickup = TRUE;
            }

            // check if entity is a battery...
            else if (strcmp("item_battery", item_name) == 0)
            {
               // check if the item is not visible (i.e. has not respawned)
               if (pent->v.effects & EF_NODRAW)
                  continue;

               // check if the bot can use this item...
               if (pEdict->v.armorvalue < VALVE_MAX_NORMAL_BATTERY)
               {
                  can_pickup = TRUE;
               }
            }

            // check if entity is a healthkit...
            else if (strcmp("item_healthkit", item_name) == 0)
            {
               // check if the item is not visible (i.e. has not respawned)
               if (pent->v.effects & EF_NODRAW)
                  continue;

               // check if the bot can use this item...
               if (pEdict->v.health < 100)
               {
                  can_pickup = TRUE;
               }
            }

            // check if entity is a packed up weapons box...
            else if (strcmp("weaponbox", item_name) == 0)
            {
               can_pickup = TRUE;
            }

            // check if entity is the spot from RPG laser
            else if (strcmp("laser_spot", item_name) == 0)
            {
            }

            // check if entity is an armed tripmine
            else if (strcmp("monster_tripmine", item_name) == 0)
            {
               float distance = (pent->v.origin - pEdict->v.origin).Length( );

               if (pBot->b_see_tripmine)
               {
                  // see if this tripmine is closer to bot...
                  if (distance < (pBot->v_tripmine - pEdict->v.origin).Length())
                  {
                     pBot->v_tripmine = pent->v.origin;
                     pBot->b_shoot_tripmine = FALSE;

                     // see if bot is far enough to shoot the tripmine...
                     if (distance >= 375)
                     {
                        pBot->b_shoot_tripmine = TRUE;
                     }
                  }
               }
               else
               {
                  pBot->b_see_tripmine = TRUE;
                  pBot->v_tripmine = pent->v.origin;
                  pBot->b_shoot_tripmine = FALSE;

                  // see if bot is far enough to shoot the tripmine...
                  if (distance >= 375)  // 375 is damage radius
                  {
                     pBot->b_shoot_tripmine = TRUE;
                  }
               }
            }

            // check if entity is an armed satchel charge
            else if (strcmp("monster_satchel", item_name) == 0)
            {
            }

            // check if entity is a snark (squeak grenade)
            else if (strcmp("monster_snark", item_name) == 0)
            {
            }

			if (mod_id == NS_DLL)
			{
				can_pickup = pBot->CanUseItem( pent );
			}
         }  // end if object is visible
      }  // end else not "func_" entity

      if (can_pickup) // if the bot found something it can pickup...
      {
         float distance = (entity_origin - pEdict->v.origin).Length( );

         // see if it's the closest item so far...
         if (distance < min_distance)
         {
            min_distance = distance;        // update the minimum distance
            pPickupEntity = pent;        // remember this entity
            pickup_origin = entity_origin;  // remember location of entity
         }
      }
   }  // end while loop

   if (pPickupEntity != NULL)
   {
      // let's head off toward that item...
      Vector v_item = pickup_origin - pEdict->v.origin;

      Vector bot_angles = UTIL_VecToAngles( v_item );

      pEdict->v.ideal_yaw = bot_angles.y;

      BotFixIdealYaw(pEdict);

      pBot->pBotPickupItem = pPickupEntity;  // save the item bot is trying to get

	  pBot->PickUpItem();
   }
}


void BotThink( bot_t *pBot )
{
	int index = 0;
	Vector v_diff;             // vector from previous to current location
	float pitch_degrees;
	float yaw_degrees;
	float moved_distance;      // length of v_diff vector (distance bot moved)
	TraceResult tr;
	bool found_waypoint;
	bool is_idle;

	edict_t *pEdict = pBot->pEdict;

	pEdict->v.flags |= FL_FAKECLIENT;

	if (pBot->name[0] == 0)  // name filled in yet?
		strcpy(pBot->name, STRING(pBot->pEdict->v.netname));

	pBot->SetMaxSpeed( pBot->GetMaxSpeed() );
	pBot->SetSpeed( pBot->GetMaxSpeed() );

	pEdict->v.button = 0;
	pBot->SetSpeed( 0.0 );

   // if the bot hasn't selected stuff to start the game yet, go do that...
   if (pBot->not_started)
   {
		pBot->Join();
		pBot->FixIdealPitch();
		BotFixIdealYaw( pEdict );
		BotFixBodyAngles( pEdict );
		BotFixViewAngles( pEdict );

		g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, 0.0, 0, 0, pEdict->v.button, 0, msecval);

		return;
   }

   // this forces the bot to attempt to join a team each frame - it's easier than trying to
   // handle the conditional messages and work out how long until it's possible to rejoin
   if( mod_id == NS_DLL && ((NSBot *)pBot)->IsInReadyRoom() )
   {
	   pBot->not_started = true;
   }

   // if the bot is dead, randomly press fire to respawn...
   if (pBot->IsDead())
   {
		pBot->Respawn();

		return;
   }

   // set this for the next time the bot dies so it will initialize stuff
   if (pBot->bShouldInit == FALSE)
   {
      pBot->bShouldInit = TRUE;
      pBot->f_bot_spawn_time = gpGlobals->time;
   }

   is_idle = FALSE;

   if (pBot->blinded_time > gpGlobals->time)
   {
      is_idle = TRUE;  // don't do anything while blinded
   }

   if (is_idle)
   {
      if (pBot->idle_angle_time <= gpGlobals->time)
      {
         pBot->idle_angle_time = gpGlobals->time + RANDOM_FLOAT(0.5, 2.0);

         pEdict->v.ideal_yaw = pBot->idle_angle + RANDOM_FLOAT(0.0, 40.0) - 20.0;

         BotFixIdealYaw(pEdict);
      }

      // turn towards ideal_yaw by yaw_speed degrees (slower than normal)
      BotChangeYaw( pBot, pEdict->v.yaw_speed / 2 );

      pBot->FixIdealPitch();
      BotFixIdealYaw (pEdict);
      BotFixBodyAngles (pEdict);
      BotFixViewAngles (pEdict);

      g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, pBot->GetSpeed(), 0, 0, pEdict->v.button, 0, msecval);

      return;
   }
   else
   {
      pBot->idle_angle = pEdict->v.v_angle.y;
   }

   // check if time to check for player sounds (if don't already have enemy)
   if ((pBot->f_sound_update_time <= gpGlobals->time) && !pBot->HasEnemy())
   {
	   pBot->UpdateSounds();
   }

   // set to max speed
   pBot->SetSpeed( pBot->GetMaxSpeed() );

   if (pBot->prev_time <= gpGlobals->time)
   {
      // see how far bot has moved since the previous position...
      v_diff = pBot->v_prev_origin - pEdict->v.origin;
      moved_distance = v_diff.Length();

      // save current position as previous
      pBot->v_prev_origin = pEdict->v.origin;
      pBot->prev_time = gpGlobals->time + 0.2;
   }
   else
   {
      moved_distance = 2.0;
   }

   // if the bot is under water, adjust pitch by pitch_speed degrees
   if ((pEdict->v.waterlevel == 2) || pBot->IsUnderWater())
   {
      // turn towards ideal_pitch by pitch_speed degrees
      pitch_degrees = BotChangePitch( pBot, pEdict->v.pitch_speed );
   }
   else
      pitch_degrees = 0.0;

   // turn towards ideal_yaw by yaw_speed degrees
   yaw_degrees = BotChangeYaw( pBot, pEdict->v.yaw_speed );

   if ((pitch_degrees >= pEdict->v.pitch_speed) || (yaw_degrees >= pEdict->v.yaw_speed))
   {
		// don't move while turning a lot
		pBot->SetSpeed( 0.0 );
   }
   else if ((pitch_degrees >= 10) || (yaw_degrees >= 10))  // turning more than 10 degrees?
   {
      pBot->SetSpeed( pBot->GetSpeed() / 4.0 );  // slow down while turning
   }
   else  // else handle movement related actions...
   {
		if( pBot->CanShoot() && pBot->ShouldSeekEnemy() )
		{
			// if it's CTF, prioritise looking for the flag
			if( pGame->IsCTF() )
			{
				// is it time to check whether bot should look for enemies yet?
				if( pBot->f_bot_find_enemy_time <= gpGlobals->time )
				{
					pBot->f_bot_find_enemy_time = gpGlobals->time + 5.0;

					if( RANDOM_LONG(1, 100) <= 30 )
					{
						pBot->pBotEnemy = BotFindEnemy( pBot );
					}
				}
			}
			else
			{
				pBot->pBotEnemy = BotFindEnemy( pBot );
			}
		}
		else
		{
			// clear enemy pointer (no enemy for you!)
			pBot->pBotEnemy = NULL;
		}

      if (pBot->HasEnemy())  // does an enemy exist?
      {
		 BotShootAtEnemy( pBot );  // shoot at the enemy

         pBot->f_pause_time = 0;  // dont't pause if enemy exists
      }
      else if (pBot->f_pause_time > gpGlobals->time)  // is bot "paused"?
      {
         // you could make the bot look left then right, or look up
         // and down, to make it appear that the bot is hunting for
         // something (don't do anything right now)
      }
      // is bot being "used" and can still follow "user"?
      else if ((pBot->pBotUser != NULL) && BotFollowUser( pBot ))
      {
         // do nothing here!
         ;
      }
      else
      {
         // no enemy, let's just wander around...

		  // is bot NOT under water?
         if ((pEdict->v.waterlevel != 2) && !pBot->IsUnderWater())
         {
            // reset pitch to 0 (level horizontally)
            pEdict->v.idealpitch = 0;
            pEdict->v.v_angle.x = 0;
         }

         pEdict->v.v_angle.z = 0;  // reset roll to 0 (straight up and down)

         pEdict->v.angles.x = 0;
         pEdict->v.angles.y = pEdict->v.v_angle.y;
         pEdict->v.angles.z = 0;

         // check if bot should look for items now or not...
         if (pBot->f_find_item < gpGlobals->time)
         {
            BotFindItem( pBot );  // see if there are any visible items
         }

         // check if bot sees a tripmine...
         if (pBot->b_see_tripmine)
         {
            // check if bot can shoot the tripmine...
            if ((pBot->b_shoot_tripmine) && BotShootTripmine( pBot ))
            {
               // shot at tripmine, may or may not have hit it, clear
               // flags anyway, next BotFindItem will see it again if
               // it is still there...
               pBot->b_shoot_tripmine = FALSE;
               pBot->b_see_tripmine = FALSE;

               // pause for a while to allow tripmine to explode...
               pBot->f_pause_time = gpGlobals->time + 0.5;
            }
            else  // run away!!!
            {
               Vector tripmine_angles;

               tripmine_angles = UTIL_VecToAngles( pBot->v_tripmine - pEdict->v.origin );

               // face away from the tripmine
               pEdict->v.ideal_yaw += 180;  // rotate 180 degrees

               BotFixIdealYaw(pEdict);

               pBot->b_see_tripmine = FALSE;

               pBot->SetSpeed( 0.0 );  // don't run while turning
            }
         }

         // check if should use wall mounted health station...
         else if (pBot->b_use_health_station)
         {
            if ((pBot->f_use_health_time + 10.0) > gpGlobals->time)
            {
               pBot->SetSpeed( 0.0 );  // don't move while using health station

               pEdict->v.button = IN_USE;
            }
            else
            {
               // bot is stuck trying to "use" a health station...
               pBot->b_use_health_station = FALSE;

               // don't look for items for a while since the bot
               // could be stuck trying to get to an item
               pBot->f_find_item = gpGlobals->time + 0.5;
            }
         }

         // check if should use wall mounted HEV station...
         else if (pBot->b_use_HEV_station)
         {
            if ((pBot->f_use_HEV_time + 10.0) > gpGlobals->time)
            {
               pBot->SetSpeed( 0.0 );  // don't move while using HEV station

               pEdict->v.button = IN_USE;
            }
            else
            {
               // bot is stuck trying to "use" a HEV station...
               pBot->b_use_HEV_station = FALSE;

               // don't look for items for a while since the bot
               // could be stuck trying to get to an item
               pBot->f_find_item = gpGlobals->time + 0.5;
            }
         }

         // check if should capture a point by using it...
         else if (pBot->b_use_capture)
         {
            int team = pGame->GetTeam( pBot->pEdict );  // skin and team must match

            // still capturing and hasn't captured yet...
            if ((pBot->f_use_capture_time > gpGlobals->time) && (pBot->pCaptureEdict->v.skin == team))
            {
               pBot->SetSpeed( 0.0 );  // don't move while capturing

               pEdict->v.button = IN_USE;
            }
            else
            {
               // bot is stuck trying to "use" a capture point...
               pBot->b_use_capture = FALSE;

               // don't look for items for a while since the bot
               // could be stuck trying to get to an item
               pBot->f_find_item = gpGlobals->time + 0.5;
            }
         }

         else if (pBot->b_use_button)
         {
            pBot->SetSpeed( 0.0 );  // don't move while using elevator

            BotUseLift( pBot, moved_distance );
         }
		 // The Ship
		else if( pBot->bUseDoor )
		{
			BotUseDoor( pBot );
		}

         else
         {
			 if( mod_id == NS_DLL )
			 {
				 if( ((NSBot *)pBot)->IsMarine() )
				{
					// check if should build
					if( pBot->bBuild )
					{
						// TODO: do some buildings take longer to build than others?
						if( ( pBot->fBuildTime + 22.0 ) > gpGlobals->time )
						{
							// don't move
							pBot->SetSpeed( 0.0 );
							if (((NSBot *)pBot)->HasWelder())
							{
								FakeClientCommand(pBot->pEdict, "weapon_welder");
								pEdict->v.button = ( IN_ATTACK | IN_DUCK );
							}
							else
							{
								pEdict->v.button = ( IN_USE | IN_DUCK );
							}
						}
						else
						{
							pBot->bBuild = false;

							// don't look for items for a while since the bot
							// could be stuck trying to get to an item
							pBot->f_find_item = gpGlobals->time + 0.5;
						}
					}
					// check if should use an armory...
					else if( pBot->bUseArmory )
					{
						if( ( pBot->fUseArmoryTime + 4.0 ) > gpGlobals->time )
						{
							pBot->SetSpeed( 0.0 );  // don't move

							pEdict->v.button = IN_USE;
						}
						else
						{
							pBot->bUseArmory = false;

							// don't look for items for a while since the bot
							// could be stuck trying to get to an item
							pBot->f_find_item = gpGlobals->time + 0.5;
						}
					}
				}
				else
				{
					if( pBot->bBuildAlienResourceTower )
					{
						pEdict->v.impulse = NSBot::CLASSIC_BUILD_RESOURCE_TOWER;

						pBot->bBuildAlienResourceTower = false;
					}
					else if( pBot->bBuildHive )
					{
						pEdict->v.impulse = NSBot::CLASSIC_BUILD_HIVE;

						pBot->bBuildHive = false;
					}
				}
			}

            if (pBot->IsUnderWater())  // check if the bot is underwater...
            {
               BotUnderWater( pBot );
            }

            found_waypoint = FALSE;

            // if the bot is not trying to get to something AND
            // it is time to look for a waypoint AND
            // there are waypoints in this level...

            if ((pBot->pBotPickupItem == NULL) && (pBot->f_look_for_waypoint_time <= gpGlobals->time) && (num_waypoints != 0))
            {
               found_waypoint = BotHeadTowardWaypoint(pBot);
            }

            // check if the bot is on a ladder...
            if (pEdict->v.movetype == MOVETYPE_FLY)
            {
               // check if bot JUST got on the ladder...
               if ((pBot->f_end_use_ladder_time + 1.0) < gpGlobals->time)
                  pBot->f_start_use_ladder_time = gpGlobals->time;

               // go handle the ladder movement
               BotOnLadder( pBot, moved_distance );

               pBot->f_dont_avoid_wall_time = gpGlobals->time + 2.0;
               pBot->f_end_use_ladder_time = gpGlobals->time;
            }
            else
            {
               // check if the bot JUST got off the ladder...
               if ((pBot->f_end_use_ladder_time + 1.0) > gpGlobals->time)
               {
                  pBot->ladder_dir = LADDER_UNKNOWN;
               }
            }

            // if the bot isn't headed toward a waypoint...
            if (found_waypoint == FALSE)
            {
               TraceResult tr;

               // check if we should be avoiding walls
               if (pBot->f_dont_avoid_wall_time <= gpGlobals->time)
               {
                  // let's just randomly wander around
                  if (BotStuckInCorner( pBot ))
                  {
                     pEdict->v.ideal_yaw += 180;  // turn 180 degrees
            
                     BotFixIdealYaw(pEdict);
            
                     pBot->SetSpeed( 0.0 );  // don't move while turning
                     pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
            
                     moved_distance = 2.0;  // dont use bot stuck code
                  }
                  else
                  {
                     // check if there is a wall on the left...
                     if (!BotCheckWallOnLeft( pBot ))
                     {
                        // if there was a wall on the left over 1/2 a second ago then
                        // 20% of the time randomly turn between 45 and 60 degrees
                        if ((pBot->f_wall_on_left != 0) && (pBot->f_wall_on_left <= gpGlobals->time - 0.5) &&
                            (RANDOM_LONG(1, 100) <= 20))
                        {
                           pEdict->v.ideal_yaw += RANDOM_LONG(45, 60);
            
                           BotFixIdealYaw(pEdict);
            
                           pBot->SetSpeed( 0.0 );  // don't move while turning
                           pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
                        }
            
                        pBot->f_wall_on_left = 0;  // reset wall detect time
                     }
                     else if (!BotCheckWallOnRight( pBot ))
                     {
                        // if there was a wall on the right over 1/2 a second ago then
                        // 20% of the time randomly turn between 45 and 60 degrees
                        if ((pBot->f_wall_on_right != 0) && (pBot->f_wall_on_right <= gpGlobals->time - 0.5) && (RANDOM_LONG(1, 100) <= 20))
                        {
                           pEdict->v.ideal_yaw -= RANDOM_LONG(45, 60);
            
                           BotFixIdealYaw(pEdict);
            
                           pBot->SetSpeed( 0.0 );  // don't move while turning
                           pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
                        }
            
                        pBot->f_wall_on_right = 0;  // reset wall detect time
                     }
                  }
               }
            
               // check if bot is about to hit a wall.  TraceResult gets returned
               if ((pBot->f_dont_avoid_wall_time <= gpGlobals->time) && BotCantMoveForward( pBot, &tr ))
               {
                  // ADD LATER
                  // need to check if bot can jump up or duck under here...
                  // ADD LATER
            
                  BotTurnAtWall( pBot, &tr );
               }
            }

            // check if bot is on a ladder and has been on a ladder for
            // more than 5 seconds...
            if ((pEdict->v.movetype == MOVETYPE_FLY) && (pBot->f_start_use_ladder_time > 0.0) && ((pBot->f_start_use_ladder_time + 5.0) <= gpGlobals->time))
            {
               // bot is stuck on a ladder...

               BotRandomTurn(pBot);

               // don't look for items for 2 seconds
               pBot->f_find_item = gpGlobals->time + 2.0;

               pBot->f_start_use_ladder_time = 0.0;  // reset start ladder time
            }
            
            // check if the bot hasn't moved much since the last location
            // (and NOT on a ladder since ladder stuck handled elsewhere)
            // (don't check for stuck if f_dont_check_stuck in the future)
            if ((moved_distance <= 1.0) && (pBot->prev_speed >= 1.0) && (pEdict->v.movetype != MOVETYPE_FLY) && (pBot->f_dont_check_stuck < gpGlobals->time))
            {
               // the bot must be stuck!
               pBot->f_dont_avoid_wall_time = gpGlobals->time + 1.0;
               pBot->f_look_for_waypoint_time = gpGlobals->time + 1.0;
            
               if (BotCanJumpUp( pBot ))  // can the bot jump onto something?
               {
                  if ((pBot->f_jump_time + 2.0) <= gpGlobals->time)
                  {
                     pBot->f_jump_time = gpGlobals->time;
                     pEdict->v.button |= IN_JUMP;  // jump up and move forward
                  }
                  else
                  {
                     // bot already tried jumping less than two seconds ago, just turn
                     BotRandomTurn(pBot);
                  }
               }
               else if (BotCanDuckUnder( pBot ))  // can the bot duck under something?
               {
                  pEdict->v.button |= IN_DUCK;  // duck down and move forward
               }
               else
               {
                  BotRandomTurn(pBot);

                  // is the bot trying to get to an item?...
                  if (pBot->pBotPickupItem != NULL)
                  {
                     // don't look for items for a while since the bot
                     // could be stuck trying to get to an item
                     pBot->f_find_item = gpGlobals->time + 0.5;
                  }
               }
            }

            // should the bot pause for a while here?
            // (don't pause on ladders or while being "used"...
            if ((RANDOM_LONG(1, 1000) <= pause_frequency[pBot->GetSkill()]) && (pEdict->v.movetype != MOVETYPE_FLY) && (pBot->pBotUser == NULL) && mod_id != NS_DLL )
            {
               // set the time that the bot will stop "pausing"
               pBot->f_pause_time = gpGlobals->time + RANDOM_FLOAT(pause_time[pBot->GetSkill()][0], pause_time[pBot->GetSkill()][1]);
            }
         }
      }
   }

   if (pBot->curr_waypoint_index != -1)  // does the bot have a waypoint?
   {
      // check if the next waypoint is a door waypoint...
      if (waypoints[pBot->curr_waypoint_index].flags & W_FL_DOOR)
      {
         pBot->SetSpeed( pBot->GetMaxSpeed() / 3.0 );  // slow down for doors
      }

      // check if the next waypoint is a ladder waypoint...
      if (waypoints[pBot->curr_waypoint_index].flags & W_FL_LADDER)
      {
         // check if the waypoint is at the top of a ladder AND
         // the bot isn't currenly on a ladder...
         if ((pBot->waypoint_top_of_ladder) && (pEdict->v.movetype != MOVETYPE_FLY))
         {
            // is the bot on "ground" above the ladder?
            if (pEdict->v.flags & FL_ONGROUND)
            {
               float waypoint_distance = (pEdict->v.origin - pBot->waypoint_origin).Length();

               if (waypoint_distance <= 20.0)  // if VERY close...
                  pBot->SetSpeed( 20.0 );  // go VERY slow
               else if (waypoint_distance <= 100.0)  // if fairly close...
                  pBot->SetSpeed( 50.0 );  // go fairly slow

               pBot->ladder_dir = LADDER_DOWN;
               pBot->f_dont_check_stuck = gpGlobals->time + 1.0;
            }
            else  // bot must be in mid-air, go BACKWARDS to touch ladder...
            {
               pBot->SetSpeed( -pBot->GetMaxSpeed() );
            }
         }
         else
         {
            // don't avoid walls for a while
            pBot->f_dont_avoid_wall_time = gpGlobals->time + 5.0;

            pBot->waypoint_top_of_ladder = FALSE;
         }
      }

      // check if the next waypoint is a crouch waypoint...
      if (waypoints[pBot->curr_waypoint_index].flags & W_FL_CROUCH)
	  {
         pEdict->v.button |= IN_DUCK;  // duck down while moving forward
	  }

      // check if the waypoint is a sniper waypoint AND
      // bot isn't currently aiming at an ememy...
      if ((waypoints[pBot->curr_waypoint_index].flags & W_FL_SNIPER) && !pBot->HasEnemy())
      {
		  if (pBot->IsSniper())
         {
            // check if it's time to adjust aim yet...
            if (pBot->f_sniper_aim_time <= gpGlobals->time)
            {
               int aim_index;

               aim_index = WaypointFindNearestAiming(waypoints[pBot->curr_waypoint_index].origin);

               if (aim_index != -1)
               {
                  Vector v_aim = waypoints[aim_index].origin - waypoints[pBot->curr_waypoint_index].origin;

                  Vector aim_angles = UTIL_VecToAngles( v_aim );

                  aim_angles.y += RANDOM_LONG(0, 30) - 15;

                  pEdict->v.ideal_yaw = aim_angles.y;

                  BotFixIdealYaw(pEdict);
               }

               // don't adjust aim again until after a few seconds...
               pBot->f_sniper_aim_time = gpGlobals->time + RANDOM_FLOAT(3.0, 5.0);
            }
         }
      }
   }

	pBot->Think();

	g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, pBot->GetSpeed(), 0, 0, pEdict->v.button, 0, msecval);

	return;
}

bot_t::bot_t()
{
	// is_used gets set to true in ServerActivate
	this->is_used = false;
	this->iBotDataIndex = BOTDATA_INDEX_UNSET;
	this->index = -1;
	// TODO: needed? already set in BotThink - not BotCreate?
	this->name[0] = '\0';
	this->pEdict = nullptr;

	this->iGoalIndex = 0;
	this->bCapturing = false;
}

bot_t::~bot_t()
{
}

void bot_t::OnSpawn()
{
	this->iGoalIndex = 0;
	this->bCapturing = false;
}

void bot_t::Join()
{
	this->not_started = 0;
}

void bot_t::Respawn()
{
	if( this->bShouldInit )
	{
		BotSpawnInit(this);

		this->bShouldInit = FALSE;
	}

	if( RANDOM_LONG(1, 100) > 50 )
	{
		pEdict->v.button = IN_ATTACK;
	}

	// TODO: everything below seems to need to get run each frame - move it to one place
	this->FixIdealPitch();
	BotFixIdealYaw(pEdict);
	BotFixBodyAngles(pEdict);
	BotFixViewAngles(pEdict);

	g_engfuncs.pfnRunPlayerMove( pEdict, pEdict->v.v_angle, this->GetSpeed(), 0, 0, pEdict->v.button, 0, msecval );
}

void bot_t::PreThink()
{
	// TODO: put all generic bot thinking here
}

void bot_t::Think()
{
	// TODO: move all of BotThink into here eventually
	this->PreThink();

	// TODO: possibly this will trigger before the bot is touching the capture point? shouldn't
	// though because bCapturing is only true when the bot is close enough to the waypoint
	// if the bot is capturing, and is at a capture point, and it's a point that should be captured
	if( this->IsCapturing() )
	{
		this->SetSpeed( 0.0 );

		// TODO: this is very rough - probably something is set in pev if the bot is
		// on or near a dod_control_point/trigger_ctfgeneric - should check if it's a brush entity...
		if( DistanceToNearest( this->pEdict->v.origin, "dod_control_point" ) > 200.0 || DistanceToNearest( this->pEdict->v.origin, "trigger_ctfgeneric" ) > 200.0 )
		{
			UTIL_LogDPrintf( "too far from capture point while capturing; resetting\n" );
			this->SetIsCapturing( false );
			this->SetSpeed( this->GetMaxSpeed() );
		}
	}

	// TODO: waypoint goal changes once it's capturing?
	// if the current waypoint is a capture point and it is now captured
	if( this->IsCapturing() && ShouldSkip( this->pEdict, this->iGoalIndex ) )
	{
		UTIL_LogDPrintf( "leaving waypoint\n" );
		this->SetIsCapturing( false );
		this->SetSpeed( this->GetMaxSpeed() );
	}

	this->PostThink();
}

void bot_t::PostThink()
{
	if( this->f_pause_time > gpGlobals->time )  // is the bot "paused"?
	{
		this->SetSpeed( 0.0 );  // don't move while pausing
	}

	// make the body face the same way the bot is looking
	this->pEdict->v.angles.y = this->pEdict->v.v_angle.y;

	// save the previous speed (for checking if stuck)
	this->prev_speed = this->fSpeed;
	
	this->FixIdealPitch();
	BotFixIdealYaw( this->pEdict );
	BotFixBodyAngles( this->pEdict );
	BotFixViewAngles( this->pEdict );
}

int bot_t::GetSkill() const
{
	extern cvar_t bot_skill;

	int iSkill = CvarGetValue( &bot_skill );

	if( iSkill < 0 )
	{
		iSkill = 0;
	}
	if( iSkill > 5 )
	{
		iSkill = 5;
	}

	return iSkill;
}

int bot_t::GetHealth() const
{
	return this->pEdict->v.health;
}

bool bot_t::HasEnemy() const
{
	if( this->pBotEnemy && this->pBotEnemy->v.solid != SOLID_NOT )
	{
		return true;
	}

	return this->pBotEnemy != NULL;
}

bool bot_t::ShouldSeekEnemy() const
{
	return true;
}

bool bot_t::CanHeal() const
{
	return false;
}

edict_t *bot_t::FindEnemyToHeal()
{
	edict_t *pNewEnemy = nullptr;
	float fNearestDistance = 1000.0f;

	extern bool b_observer_mode;

	// search the world for players...
	for( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		edict_t *pPlayer = INDEXENT(i);

		// skip invalid players and skip self (i.e. this bot)
		if( pGame->IsValidEdict( pPlayer ) && this->IsValidEnemy( pPlayer ) )
		{
			if ((b_observer_mode) && !(pPlayer->v.flags & FL_FAKECLIENT))
				continue;

			// check if player needs to be healed...
			if ((pPlayer->v.health / pPlayer->v.max_health) > 0.50)
				continue;  // health greater than 50% so ignore

			Vector vecEnd = pPlayer->v.origin + pPlayer->v.view_ofs;

			// see if bot can see the player...
			if (FInViewCone( &vecEnd, this->pEdict ) && FVisible( vecEnd, this->pEdict ))
			{
				float distance = (pPlayer->v.origin - pEdict->v.origin).Length();

				if (distance < fNearestDistance)
				{
					fNearestDistance = distance;
					pNewEnemy = pPlayer;

					this->pBotUser = nullptr;  // don't follow user when enemy found
				}
			}
		}
	}

	return pNewEnemy;
}

bool bot_t::IsValidEnemy( const edict_t *pEnemy )
{
	// a bot can't be its own enemy
	if( pEnemy == this->pEdict )
	{
		return false;
	}
	if( !IsAlive( pEnemy ) )
	{
		return false;
	}
	// is team play enabled?
	if( pGame->IsTeamPlay() )
	{
		// don't target your teammates
		if( UTIL_GetTeam( this->pEdict ) == UTIL_GetTeam( pEnemy ) )
		{
			return false;
		}
	}

	return true;
}

float bot_t::GetDistanceToEnemy() const
{
	if( !this->pBotEnemy )
	{
		ALERT( at_error, "Call to __FUNCTION__ when pBotEnemy is NULL!\n" );

		return -1.0f;
	}

	return (this->pBotEnemy->v.origin - GetGunPosition( this->pEdict )).Length();
}

float bot_t::GetSpeedToEnemy() const
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
	// walk if distance is closer
	else if( fDistanceToEnemy > 20.0 )
	{
		fSpeed = this->GetMaxSpeed() / 2.0;
	}
	// don't move if close enough
	else
	{
		fSpeed = 0.0;
	}

	return fSpeed;
}

int bot_t::GetEnemiesInLineOfSight( const float fMinDistance, const float fMaxDistance )
{
	int iEnemiesInLineOfSight = 0;

	for( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		edict_t *pPlayer = INDEXENT(i);

		if( !pGame->IsValidEdict( pPlayer ) && !this->IsValidEnemy( pPlayer ) )
		{
			continue;
		}
	}

	return iEnemiesInLineOfSight;
}

float bot_t::GetAimSpread() const
{
	Vector enemyOrigin = this->pBotEnemy->v.origin;
	float fDistanceToEnemy = (enemyOrigin - this->GetOrigin()).Length();
	float fSpread = 0.0;

	// wide spread for for-away enemies
	if( fDistanceToEnemy > 1000.0 )
	{
		fSpread = 1.0;
	}
	// proportional spread for nearby enemies
	else if( fDistanceToEnemy > 100.0 )
	{
		fSpread = fDistanceToEnemy / 1000.0;
	}
	// small spread for close enemies
	else
	{
		fSpread = 0.1;
	}

	return fSpread;
}

Vector bot_t::GetPointToShootAt() const
{
	Vector target;
	Vector enemyOrigin = this->pBotEnemy->v.origin;
	float fSpread = this->GetAimSpread();
	int d_x, d_y, d_z;

	edict_t *pEdict = this->pEdict;

	switch( this->GetSkill() )
	{
	case 1:
		// VERY GOOD, same as from CBasePlayer::BodyTarget (in player.h)
		target = enemyOrigin + this->pBotEnemy->v.view_ofs * RANDOM_FLOAT( 0.5, 1.1 );
		d_x = 0;  // no offset
		d_y = 0;
		d_z = 0;
		break;
	case 2:
		// GOOD, offset a little for x, y, and z
		target = enemyOrigin + this->pBotEnemy->v.view_ofs;  // aim for the head (if you can find it)
		d_x = RANDOM_FLOAT(-5, 5) * fSpread;
		d_y = RANDOM_FLOAT(-5, 5) * fSpread;
		d_z = RANDOM_FLOAT(-10, 10) * fSpread;
		break;
	case 3:
		// FAIR, offset somewhat for x, y, and z
		target = enemyOrigin;  // aim for the body
		d_x = RANDOM_FLOAT(-10, 10) * fSpread;
		d_y = RANDOM_FLOAT(-10, 10) * fSpread;
		d_z = RANDOM_FLOAT(-18, 18) * fSpread;
		break;
	case 4:
		// POOR, offset for x, y, and z
		target = enemyOrigin;  // aim for the body
		d_x = RANDOM_FLOAT(-20, 20) * fSpread;
		d_y = RANDOM_FLOAT(-20, 20) * fSpread;
		d_z = RANDOM_FLOAT(-32, 32) * fSpread;
		break;
	case 5:
		// BAD, offset lots for x, y, and z
		target = enemyOrigin;  // aim for the body
		d_x = RANDOM_FLOAT(-35, 35) * fSpread;
		d_y = RANDOM_FLOAT(-35, 35) * fSpread;
		d_z = RANDOM_FLOAT(-50, 50) * fSpread;
		break;
	}

	target = target + Vector(d_x, d_y, d_z);

	return target;
}

void bot_t::Reload()
{
	// TODO: override in subclasses and say something in chat or whatever
	pEdict->v.button |= IN_RELOAD;
}

bool bot_t::ShouldReload() const
{
	return true;
}

bool bot_t::CanShoot() const
{
	extern cvar_t bot_shoot;
	// TODO: in Natural Selection at least, bot_shoot.value is 0 but bot_shoot.string is 1
	return CvarGetValue( &bot_shoot ) > 0.0 || atof( CvarGetString( &bot_shoot ) ) > 0.0;
}

int bot_t::GetPistol() const
{
	return VALVE_WEAPON_GLOCK;
}

int bot_t::GetHealingWeapon() const
{
	return NO_SUCH_WEAPON;
}

bool bot_t::IsSniping() const
{
	// TODO: crossbow/357?
	return false;
}

bool bot_t::CanSeePoint( const Vector& point )
{
	TraceResult tr;

	// trace a line from bot's eyes to destination...
	UTIL_TraceLine( this->pEdict->v.origin + this->pEdict->v.view_ofs, point, ignore_monsters, this->pEdict->v.pContainingEntity, &tr );

	// check if line of sight to object is not blocked (i.e. visible)
	return tr.flFraction >= 1.0;
}

int bot_t::GetAngleToPoint( const Vector& point )
{
	// find angles from source to destination...
	Vector entity_angles = UTIL_VecToAngles( point );

	// make yaw angle 0 to 360 degrees if negative...
	if( entity_angles.y < 0 )
	{
		entity_angles.y += 360;
	}

	// get bot's current view angle...
	float fViewAngle = this->pEdict->v.v_angle.y;

	// make view angle 0 to 360 degrees if negative...
	if( fViewAngle < 0.0 )
	{
		fViewAngle += 360;
	}

	// return the absolute value of angle to destination entity
	// zero degrees means straight ahead,  45 degrees to the left or
	// 45 degrees to the right is the limit of the normal view angle

	// angle bug fix below - rsm
	int iAngle = abs((int)fViewAngle - (int)entity_angles.y);

	if( iAngle > 180 )
	{
		iAngle = 360 - iAngle;
	}

	return iAngle;
}

bool bot_t::CanUseItem( const edict_t *pItem )
{
	// TODO: make this handle stuff - okay as it's only used by NSBot right now
	return false;
}

void bot_t::PickUpItem()
{
}

void bot_t::SetMaxSpeed( float fMaxSpeed )
{
	this->fMaxSpeed = fMaxSpeed;
}

float bot_t::GetMaxSpeed() const
{
	return CVAR_GET_FLOAT("sv_maxspeed");
}

void bot_t::SetSpeed( float fSpeed )
{
	this->fSpeed = fSpeed;
}

float bot_t::GetSpeed() const
{
	return this->fSpeed;
}

int bot_t::GetLightLevel()
{
	// TODO: Foxbot uses this->pBotEnemy instead of this->pEdict
	this->pLightEnt = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    MDLL_Spawn(this->pLightEnt);
	this->pLightEnt->v.origin = this->pEdict->v.origin;
    this->pLightEnt->v.takedamage = DAMAGE_NO;
    this->pLightEnt->v.solid = SOLID_NOT;
    this->pLightEnt->v.owner = this->pEdict;
    this->pLightEnt->v.movetype = MOVETYPE_FLY; // noclip
	// TODO: give this a real entity name? like info_target?
    this->pLightEnt->v.classname = MAKE_STRING("entity_botlightvalue");
    this->pLightEnt->v.nextthink = gpGlobals->time;
    this->pLightEnt->v.rendermode = kRenderNormal;
    this->pLightEnt->v.renderfx = kRenderFxNone;
    this->pLightEnt->v.renderamt = 0;
    SET_MODEL(this->pLightEnt, "models/mechgibs.mdl"); // invisible model

	int iIllumination = GETENTITYILLUM(this->pLightEnt);

	// TODO: keep this for the life-time of the bot?
	REMOVE_ENTITY(this->pLightEnt);

	return iIllumination;
}

bool bot_t::IsDead()
{
	return this->pEdict->v.health < 1 || this->pEdict->v.deadflag != DEAD_NO;
}

bool bot_t::IsUnderWater() const
{
	return this->pEdict->v.waterlevel == 3;
}

bool bot_t::IsSniper()
{
	return false;
}

bool bot_t::IsCapturing() const
{
	return this->bCapturing;
}

void bot_t::UpdateSounds()
{
	edict_t *pPlayer;

	this->f_sound_update_time = gpGlobals->time + 1.0;

	for( int idx = 1; idx <= gpGlobals->maxClients; idx++ )
	{
		pPlayer = INDEXENT(idx);

		// is this player slot is valid and it's not this bot...
		if( pPlayer && !pPlayer->free && (pPlayer != pEdict) )
		{
			// if observer mode enabled, don't listen to this player...
			if( b_observer_mode && !(pPlayer->v.flags & FL_FAKECLIENT) )
				continue;

			if( IsAlive(pPlayer) && (FBitSet(pPlayer->v.flags, FL_CLIENT) || FBitSet(pPlayer->v.flags, FL_FAKECLIENT)) )
			{
				// check for sounds being made by other players...
				if( ::UpdateSounds(pEdict, pPlayer) )
				{
					// don't check for sounds for another 30 seconds
					this->f_sound_update_time = gpGlobals->time + 30.0;
				}
			}
		}
	}
}

bool bot_t::ShouldLookForNewGoal()
{
	return true;
}

int bot_t::GetGoalType()
{
	// TODO: something better?
	return W_FL_HEALTH;
}

float bot_t::GetWaypointRadius()
{
	float fRadius = 500.0f;

	// use a much smaller search radius when waypoints are available
	if( (num_waypoints > 0 ) && ( this->curr_waypoint_index != -1 ) )
	{
		fRadius = 100.0;
	}

	return fRadius;
}

bool bot_t::BaseCanUseWeapon() const
{
	return this->HasEnemy();
}

std::vector<weapon_t> bot_t::GetUsableWeapons( bool strict )
{
	std::vector<weapon_t> usableWeapons;

	for( unsigned int i = 0; i < this->weapons.size(); i++ )
	{
		weapon_t weapon = this->weapons[i];

		CanUseWeapon pfnCanUseWeapon = weapon.pfnCanUseWeapon;

		// check if the bot actually has the weapon
		if(!(this->pEdict->v.weapons & (1<<weapon.iWeaponId)))
		{
			continue;
		}

		// check if the bot can use the weapon
		if( (this->*pfnCanUseWeapon)(strict) )
		{
			usableWeapons.push_back( weapon );
		}
	}

	return usableWeapons;
}

Vector bot_t::GetOrigin() const
{
	return this->pEdict->v.origin;
}

void bot_t::FixIdealPitch()
{
	// check for wrap around of angle
	if( this->pEdict->v.idealpitch >= 180.0 )
	{
		this->pEdict->v.idealpitch -= 360.0 * ((int)(this->pEdict->v.idealpitch / 360.0) + 1);
	}

	if( this->pEdict->v.idealpitch < -180 )
	{
		this->pEdict->v.idealpitch += 360.0 * ((int)(-this->pEdict->v.idealpitch / 360.0) + 1);
	}
}

bool bot_t::ShouldJumpAfterDeath()
{
	return RANDOM_LONG(1, 100) <= 10;
}

bool bot_t::HasFlag() const
{
	return false;
}

bool bot_t::ShouldCapturePoint( edict_t * pControlPoint )
{
	return false;
}

void bot_t::SetIsCapturing( const bool bIsCapturing )
{
	this->bCapturing = bIsCapturing;
}
