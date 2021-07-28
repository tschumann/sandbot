//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_client.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_client.h"
#include "bot_weapons.h"
#include "game.h"

// https://wiki.alliedmods.net/Half-life_1_game_events

// types of damage to ignore...
#define IGNORE_DAMAGE (DMG_CRUSH | DMG_FREEZE | DMG_FALL | DMG_SHOCK | \
                       DMG_DROWN | DMG_NERVEGAS | DMG_RADIATION | \
                       DMG_DROWNRECOVER | DMG_ACID | DMG_SLOWBURN | \
                       DMG_SLOWFREEZE | 0xFF000000)

bot_weapon_t weapon_defs[MAX_WEAPONS]; // array of weapon definitions


// This message is sent when the TFC VGUI menu is displayed.
void BotClient_TFC_VGUI(void *p, int bot_index)
{
	if ((*(int *)p) == 2)  // is it a team select menu?
		pBots[bot_index]->start_action = MSG_TFC_TEAM_SELECT;
	else if ((*(int *)p) == 3)  // is is a class selection menu?
		pBots[bot_index]->start_action = MSG_TFC_CLASS_SELECT;
}


// This message is sent when the Counter-Strike VGUI menu is displayed.
void BotClient_CS_VGUI(void *p, int bot_index)
{
   if ((*(int *)p) == 2)  // is it a team select menu?
      pBots[bot_index]->start_action = MSG_CS_TEAM_SELECT;
   else if ((*(int *)p) == 26)  // is is a terrorist model select menu?
      pBots[bot_index]->start_action = MSG_CS_T_SELECT;
   else if ((*(int *)p) == 27)  // is is a counter-terrorist model select menu?
      pBots[bot_index]->start_action = MSG_CS_CT_SELECT;
}

// This message is sent when the Day of Defeat VGUI menu is displayed.
void BotClient_DOD_VGUI(void *p, int bot_index)
{
	if ((*(int *)p) == 2)  // is it a team select menu?
		pBots[bot_index]->start_action = MSG_DOD_TEAM_SELECT;
	else if ((*(int *)p) == 10)  // is is a allied class select menu?
		pBots[bot_index]->start_action = MSG_DOD_ALLIED_SELECT;
	else if ((*(int *)p) == 13)  // is is an axis class select menu?
		pBots[bot_index]->start_action = MSG_DOD_AXIS_SELECT;
}

// This message is sent when a menu is being displayed in Counter-Strike.
void BotClient_CS_ShowMenu(void *p, int bot_index)
{
   static int state = 0;   // current state machine state

   if (state < 3)
   {
      state++;  // ignore first 3 fields of message
      return;
   }

   if (strcmp((char *)p, "#Team_Select") == 0)  // team select menu?
   {
      pBots[bot_index]->start_action = MSG_CS_TEAM_SELECT;
   }
   else if (strcmp((char *)p, "#Terrorist_Select") == 0)  // T model select?
   {
      pBots[bot_index]->start_action = MSG_CS_T_SELECT;
   }
   else if (strcmp((char *)p, "#CT_Select") == 0)  // CT model select menu?
   {
      pBots[bot_index]->start_action = MSG_CS_CT_SELECT;
   }

   state = 0;  // reset state machine
}


// This message is sent when the TFC VGUI menu is displayed.
void BotClient_Gearbox_VGUI(void *p, int bot_index)
{
	if ((*(int *)p) == 2)  // is it a team select menu?
	{
		pBots[bot_index]->start_action = MSG_OPFOR_TEAM_SELECT;
	}
	else if ((*(int *)p) == 3)  // is is a class selection menu?
	{
		pBots[bot_index]->start_action = MSG_OPFOR_CLASS_SELECT;
	}
}

// This message is sent when a client joins the game.  All of the weapons
// are sent with the weapon ID and information about what ammo is used.
void BotClient_Valve_WeaponList(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static bot_weapon_t bot_weapon;

   if (state == 0)
   {
      state++;
      strcpy(bot_weapon.szClassname, (char *)p);
   }
   else if (state == 1)
   {
      state++;
      bot_weapon.iAmmo1 = *(int *)p;  // ammo index 1
   }
   else if (state == 2)
   {
      state++;
      bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
   }
   else if (state == 3)
   {
      state++;
      bot_weapon.iAmmo2 = *(int *)p;  // ammo index 2
   }
   else if (state == 4)
   {
      state++;
      bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
   }
   else if (state == 5)
   {
      state++;
      bot_weapon.iSlot = *(int *)p;  // slot for this weapon
   }
   else if (state == 6)
   {
      state++;
      bot_weapon.iPosition = *(int *)p;  // position in slot
   }
   else if (state == 7)
   {
      state++;
      bot_weapon.iId = *(int *)p;  // weapon ID
   }
   else if (state == 8)
   {
      state = 0;

      bot_weapon.iFlags = *(int *)p;  // flags for weapon (WTF???)

      // store away this weapon with it's ammo information...
      weapon_defs[bot_weapon.iId] = bot_weapon;
   }
}

void BotClient_Gearbox_WeaponList(void *p, int bot_index)
{
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_CS_WeaponList(void *p, int bot_index)
{
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_DOD_WeaponList(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static bot_weapon_t bot_weapon;

   if (state == 0)
   {
      state++;
	  // strcpy(bot_weapon.szClassname, (char *)p);
	  bot_weapon.iAmmo1 = *(int *)p;  // ammo index 1
   }
   else if (state == 1)
   {
      state++;
      // bot_weapon.iAmmo1 = *(int *)p;  // ammo index 1
	  bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
   }
   else if (state == 2)
   {
      state++;
	  // bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
	  bot_weapon.iAmmo2 = *(int *)p;  // ammo index 2
   }
   else if (state == 3)
   {
      state++;
      // bot_weapon.iAmmo2 = *(int *)p;  // ammo index 2
	  bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
   }
   else if (state == 4)
   {
      state++;
      // bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
	  // maybe slot?
	  bot_weapon.iSlot = *(int *)p;  // slot for this weapon
   }
   else if (state == 5)
   {
      state++;
      // bot_weapon.iSlot = *(int *)p;  // slot for this weapon
   }
   else if (state == 6)
   {
      state++;
      bot_weapon.iId = *(int *)p;  // weapon ID

		switch(bot_weapon.iId)
		{
		case DOD_WEAPON_GARAND:
			strcpy(bot_weapon.szClassname, "weapon_garand");
			break;
		case DOD_WEAPON_THOMPSON:
			strcpy(bot_weapon.szClassname, "weapon_thompson");
			break;
		case DOD_WEAPON_MP44:
			strcpy(bot_weapon.szClassname, "weapon_mp44");
			break;
		case DOD_WEAPON_KAR:
			strcpy(bot_weapon.szClassname, "weapon_kar");
			break;
		case DOD_WEAPON_MP40:
			strcpy(bot_weapon.szClassname, "weapon_mp40");
			break;
		case DOD_WEAPON_HANDGRENADE:
			strcpy(bot_weapon.szClassname, "weapon_handgrenade");
			break;
		case DOD_WEAPON_STICKGRENADE:
			strcpy(bot_weapon.szClassname, "weapon_stickgrenade");
			break;
		case DOD_WEAPON_M1CARBINE:
			strcpy(bot_weapon.szClassname, "weapon_m1carbine");
			break;
		case DOD_WEAPON_GREASEGUN:
			strcpy(bot_weapon.szClassname, "weapon_greasegun");
			break;
		case DOD_WEAPON_K43:
			strcpy(bot_weapon.szClassname, "weapon_k43");
			break;
		case DOD_WEAPON_BAZOOKA:
			strcpy(bot_weapon.szClassname, "weapon_bazooka");
			break;
		case DOD_WEAPON_PSCHRECK:
			strcpy(bot_weapon.szClassname, "weapon_pschreck");
			break;
		}
   }
   else if (state == 7)
   {
      state++;
	  // flags?
      // bot_weapon.iId = *(int *)p;  // weapon ID
   }
   else if (state == 8)
   {
      state = 0;

	  // clip size?
      // bot_weapon.iFlags = *(int *)p;  // flags for weapon (WTF???)

      // store away this weapon with it's ammo information...
      weapon_defs[bot_weapon.iId] = bot_weapon;
   }
}

void BotClient_TFC_WeaponList(void *p, int bot_index)
{
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_Gunman_WeaponList(void *p, int bot_index)
{
	static int state = 0;	// current state machine state
	static bot_weapon_t bot_weapon;

	// TODO: this might be better as a switch statement
	if (state == 0)
	{
		state++;
		strcpy(bot_weapon.szClassname, (char *)p);
	}
	else if (state == 1)
	{
		state++;
		bot_weapon.iAmmo1 = *(int *)p;  // ammo index 1
	}
	else if (state == 2)
	{
		state++;
		bot_weapon.iAmmo1Max = *(int *)p;  // max ammo1
	}
	else if (state == 3)
	{
		state++;
		bot_weapon.iAmmo2 = *(int *)p;  // ammo index 2
	}
	else if (state == 4)
	{
		state++;
		bot_weapon.iAmmo2Max = *(int *)p;  // max ammo2
	}
	else if (state == 5)
	{
		state++;
		bot_weapon.iSlot = *(int *)p;  // slot for this weapon
	}
	else if (state == 6)
	{
		state++;
		bot_weapon.iPosition = *(int *)p;  // position in slot
	}
	else if (state == 7)
	{
		state++;
		bot_weapon.iId = *(int *)p;  // weapon ID
	}
	else if (state == 8)
	{
		state++;
		bot_weapon.iFlags = *(int *)p;  // flags for weapon (WTF???)
	}
	else if (state == 9)
	{
		state++;
		strcpy(bot_weapon.szParentClass, (char *)p);
	}
	else if (state == 10)
	{
		state++;
		bot_weapon.iUnknown[0] = *(int *)p;
	}
	else if (state == 11)
	{
		state++;
		bot_weapon.iUnknown[1] = *(int *)p;
	}
	else if (state == 12)
	{
		state = 0;
		bot_weapon.iUnknown[2] = *(int *)p;

		weapon_defs[bot_weapon.iId] = bot_weapon;
	}
}

void BotClient_NS_WeaponList( void *p, int bot_index )
{
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_Hunger_WeaponList( void *p, int bot_index )
{
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_Ship_WeaponList(void *p, int bot_index)
{
	// this is just like the Valve Weapon List message
	BotClient_Valve_WeaponList(p, bot_index);
}


// This message is sent when a weapon is selected (either by the bot chosing
// a weapon or by the server auto assigning the bot a weapon).
void BotClient_Valve_CurrentWeapon(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static int iState;
   static int iId;
   static int iClip;

   if (state == 0)
   {
      state++;
      iState = *(int *)p;  // state of the current weapon
   }
   else if (state == 1)
   {
      state++;
      iId = *(int *)p;  // weapon ID of current weapon
   }
   else if (state == 2)
   {
      state = 0;

      iClip = *(int *)p;  // ammo currently in the clip for this weapon

      if (iId <= 31)
      {
         if (iState == 1)
         {
            pBots[bot_index]->current_weapon.iId = iId;
            pBots[bot_index]->current_weapon.iClip = iClip;

            // update the ammo counts for this weapon...
            pBots[bot_index]->current_weapon.iAmmo1 = pBots[bot_index]->m_rgAmmo[weapon_defs[iId].iAmmo1];
            pBots[bot_index]->current_weapon.iAmmo2 = pBots[bot_index]->m_rgAmmo[weapon_defs[iId].iAmmo2];
         }
      }
   }
}

void BotClient_Gearbox_CurrentWeapon(void *p, int bot_index)
{
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index);
}

void BotClient_CS_CurrentWeapon(void *p, int bot_index)
{
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index);
}

void BotClient_TFC_CurrentWeapon(void *p, int bot_index)
{
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index);
}

void BotClient_Gunman_CurrentWeapon(void *p, int bot_index)
{
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index);
}

void BotClient_NS_CurrentWeapon( void *p, int bot_index )
{
    // this is just like the Valve Current Weapon message
    BotClient_Valve_CurrentWeapon(p, bot_index);
}

void BotClient_Hunger_CurrentWeapon(void *p, int bot_index)
{
	// this is just like the Valve Current Weapon message
	BotClient_Valve_CurrentWeapon(p, bot_index);
}

void BotClient_Ship_CurrentWeapon( void *p, int bot_index )
{
    // this is just like the Valve Current Weapon message
    BotClient_Valve_CurrentWeapon(p, bot_index);
}


// This message is sent whenever ammo ammounts are adjusted (up or down).
void BotClient_Valve_AmmoX(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static int index;
   static int ammount;
   int ammo_index;

   if (state == 0)
   {
      state++;
      index = *(int *)p;  // ammo index (for type of ammo)
   }
   else if (state == 1)
   {
      state = 0;

      ammount = *(int *)p;  // the ammount of ammo currently available

      pBots[bot_index]->m_rgAmmo[index] = ammount;  // store it away

      ammo_index = pBots[bot_index]->current_weapon.iId;

      // update the ammo counts for this weapon...
      pBots[bot_index]->current_weapon.iAmmo1 = pBots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo1];
      pBots[bot_index]->current_weapon.iAmmo2 = pBots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo2];
   }
}

void BotClient_Gearbox_AmmoX(void *p, int bot_index)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_CS_AmmoX(void *p, int bot_index)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_TFC_AmmoX(void *p, int bot_index)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_Gunman_AmmoX(void *p, int bot_index)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_NS_AmmoX(void *p, int bot_index)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_Hunger_AmmoX(void *p, int bot_index)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_Ship_AmmoX(void *p, int bot_index)
{
	// this is just like the Valve AmmoX message
	BotClient_Valve_AmmoX(p, bot_index);
}


// This message is sent when the bot picks up some ammo (AmmoX messages are
// also sent so this message is probably not really necessary except it
// allows the HUD to draw pictures of ammo that have been picked up.  The
// bots don't really need pictures since they don't have any eyes anyway.
void BotClient_Valve_AmmoPickup(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static int index;
   static int ammount;
   int ammo_index;

   if (state == 0)
   {
      state++;
      index = *(int *)p;
   }
   else if (state == 1)
   {
      state = 0;

      ammount = *(int *)p;

      pBots[bot_index]->m_rgAmmo[index] = ammount;

      ammo_index = pBots[bot_index]->current_weapon.iId;

      // update the ammo counts for this weapon...
      pBots[bot_index]->current_weapon.iAmmo1 = pBots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo1];
      pBots[bot_index]->current_weapon.iAmmo2 = pBots[bot_index]->m_rgAmmo[weapon_defs[ammo_index].iAmmo2];
   }
}

void BotClient_Gearbox_AmmoPickup(void *p, int bot_index)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index);
}

void BotClient_CS_AmmoPickup(void *p, int bot_index)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index);
}

void BotClient_TFC_AmmoPickup(void *p, int bot_index)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index);
}

void BotClient_Gunman_AmmoPickup(void *p, int bot_index)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index);
}

void BotClient_NS_AmmoPickup(void *p, int bot_index)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index);
}

void BotClient_Hunger_AmmoPickup(void *p, int bot_index)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index);
}

void BotClient_Ship_AmmoPickup(void *p, int bot_index)
{
	// this is just like the Valve Ammo Pickup message
	BotClient_Valve_AmmoPickup(p, bot_index);
}


// This message gets sent when the bots are getting damaged.
void BotClient_Valve_Damage(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static int damage_armor;
   static int damage_taken;
   static int damage_bits;  // type of damage being done
   static Vector damage_origin;

   if (state == 0)
   {
      state++;
      damage_armor = *(int *)p;
   }
   else if (state == 1)
   {
      state++;
      damage_taken = *(int *)p;
   }
   else if (state == 2)
   {
      state++;
      damage_bits = *(int *)p;
   }
   else if (state == 3)
   {
      state++;
      damage_origin.x = *(float *)p;
   }
   else if (state == 4)
   {
      state++;
      damage_origin.y = *(float *)p;
   }
   else if (state == 5)
   {
      state = 0;

      damage_origin.z = *(float *)p;

      if ((damage_armor > 0) || (damage_taken > 0))
      {
         // ignore certain types of damage...
         if (damage_bits & IGNORE_DAMAGE)
            return;

         // if the bot doesn't have an enemy and someone is shooting at it then
         // turn in the attacker's direction...
         if (!pBots[bot_index]->HasEnemy())
         {
            // face the attacker...
            Vector v_enemy = damage_origin - pBots[bot_index]->pEdict->v.origin;
            Vector bot_angles = UTIL_VecToAngles( v_enemy );

            pBots[bot_index]->pEdict->v.ideal_yaw = bot_angles.y;

            BotFixIdealYaw(pBots[bot_index]->pEdict);

            // stop using health or HEV stations...
            pBots[bot_index]->b_use_health_station = FALSE;
            pBots[bot_index]->b_use_HEV_station = FALSE;
            pBots[bot_index]->b_use_capture = FALSE;
         }
      }
   }
}

void BotClient_Gearbox_Damage(void *p, int bot_index)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index);
}

void BotClient_CS_Damage(void *p, int bot_index)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index);
}

void BotClient_TFC_Damage(void *p, int bot_index)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index);
}

void BotClient_Gunman_Damage(void *p, int bot_index)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index);
}

void BotClient_NS_Damage(void *p, int bot_index)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index);
}

void BotClient_Hunger_Damage(void *p, int bot_index)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index);
}

void BotClient_Ship_Damage(void *p, int bot_index)
{
	// this is just like the Valve Battery message
	BotClient_Valve_Damage(p, bot_index);
}


// This message gets sent when the bots money ammount changes (for CS)
void BotClient_CS_Money(void *p, int bot_index)
{
	static int state = 0;	// current state machine state

	if (state == 0)
	{
		state++;

		pBots[bot_index]->bot_money = *(int *)p;	// amount of money
	}
	else
	{
		state = 0;  // ingore this field
	}
}


// This message gets sent when the bots get killed
void BotClient_Valve_DeathMsg(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static int killer_index;
   static int victim_index;
   static edict_t *victim_edict;
   static int index;

   if (state == 0)
   {
      state++;
      killer_index = *(int *)p;  // ENTINDEX() of killer
   }
   else if (state == 1)
   {
      state++;
      victim_index = *(int *)p;  // ENTINDEX() of victim
   }
   else if (state == 2)
   {
      state = 0;

      victim_edict = INDEXENT(victim_index);

      index = UTIL_GetBotIndex(victim_edict);

      // is this message about a bot being killed?
      if (index != -1)
      {
         if ((killer_index == 0) || (killer_index == victim_index))
         {
            // bot killed by world (worldspawn) or bot killed self...
            pBots[index]->killer_edict = NULL;
         }
         else
         {
            // store edict of player that killed this bot...
            pBots[index]->killer_edict = INDEXENT(killer_index);
         }
      }
   }
}

void BotClient_Gearbox_DeathMsg(void *p, int bot_index)
{
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_CS_DeathMsg(void *p, int bot_index)
{
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_TFC_DeathMsg(void *p, int bot_index)
{
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_Gunman_DeathMsg(void *p, int bot_index)
{
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_NS_DeathMsg(void *p, int bot_index)
{
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_Hunger_DeathMsg(void *p, int bot_index)
{
	// this is just like the Valve DeathMsg message
	BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_Valve_ScreenFade(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static int duration;
   static int hold_time;
   static int fade_flags;
   int length;

   if (state == 0)
   {
      state++;
      duration = *(int *)p;
   }
   else if (state == 1)
   {
      state++;
      hold_time = *(int *)p;
   }
   else if (state == 2)
   {
      state++;
      fade_flags = *(int *)p;
   }
   else if (state == 6)
   {
      state = 0;

      length = (duration + hold_time) / 4096;
      pBots[bot_index]->blinded_time = gpGlobals->time + length - 2.0;
   }
   else
   {
      state++;
   }
}

void BotClient_Gearbox_ScreenFade(void *p, int bot_index)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_CS_ScreenFade(void *p, int bot_index)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_TFC_ScreenFade(void *p, int bot_index)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_Gunman_ScreenFade(void *p, int bot_index)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_NS_ScreenFade(void *p, int bot_index)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_Hunger_ScreenFade(void *p, int bot_index)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_Ship_ScreenFade(void *p, int bot_index)
{
	// this is just like the Valve ScreenFade message
	BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_CS_HLTV(void *p, int bot_index)
{
   static int state = 0;   // current state machine state
   static int players;
   int index;

   if (state == 0)
   {
      players = *(int *) p;
   }
   else if (state == 1)
   {
      // new round in CS 1.6
      if ((players == 0) && (*(int *) p == 0))
      {
         for (index = 0; index < Game::MAX_PLAYERS; index++)
         {
            if (pBots[index]->is_used)
               BotSpawnInit (pBots[index]); // reset bots for new round
         }
      }
   }
}

void BotClient_NS_CountDown( void *p, int bot_index )
{
	extern bool g_bInGame;
	extern int g_iCountDown;
	extern float g_fCountDownTime;

	// reset these values
	g_bInGame = false;
	g_fCountDownTime = 0.0;

	g_iCountDown = *(int *)p;
}

void BotClient_NS_GameStatus( void *p, int bot_index )
{
	static int state = 0;   // current state machine state
	int status = -1;
	int mapMode = 0;
	int gameTime = 0;
	int timeLimit = 0;
	int data = 0;

	if (state == 0)
	{
		status = *(int *) p;

		if( bot_index != -1 && ( status == kGameStatusReset || status == kGameStatusResetNewMap || status == kGameStatusEnded ) )
		{
			pBots[bot_index]->not_started = true;
		}
	}
	else if (state == 1)
	{
		state++;
		mapMode = *(int *) p;
	}
	else if (state == 2 && (status == kGameStatusReset || status == kGameStatusResetNewMap || status == kGameStatusEnded))
	{
		state = 0;
	}
	else if (state == 2 && status == kGameStatusGameTime)
	{
		state++;
		gameTime = *(int *) p;
	}
	else if (state == 2 && status == kGameStatusUnspentLevels)
	{
		state++;
		data = *(int *) p;
	}
	else if (state == 3 && status == kGameStatusGameTime)
	{
		state++;
		timeLimit = *(int *) p;
	}
	else if (state == 4 && status == kGameStatusGameTime)
	{
		state++;
		data = *(int *) p;
	}
	else
	{
		state = 0;
	}
}

void BotClient_Ship_Quarry( void *p, int bot_index )
{
	int quarryId = *(int *)p;

	((ShipBot *)pBots[bot_index])->SetQuarry(quarryId);
}
