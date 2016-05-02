//
// gbot - The GoldSource bot
//
// <no site>
//
// bot_client.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "bot_func.h"
#include "bot_client.h"
#include "bot_weapons.h"

// types of damage to ignore...
#define IGNORE_DAMAGE (DMG_CRUSH | DMG_FREEZE | DMG_FALL | DMG_SHOCK | DMG_DROWN | DMG_NERVEGAS | DMG_RADIATION | \
                              DMG_DROWNRECOVER | DMG_ACID | DMG_SLOWBURN | DMG_SLOWFREEZE | 0xFF000000)

extern int g_iMod;

extern bool g_bInGame;
extern int g_iCountDown;
extern float g_fCountDownTime;

extern bot_t bots[32];

bot_weapon_t weapon_defs[MAX_WEAPONS];	// array of weapon definitions

// This message is sent when a client joins the game.  All of the weapons
// are sent with the weapon ID and information about what ammo is used.
void BotClient_Valve_WeaponList(void *p, int bot_index)
{
    static int state = 0;    // current state machine state
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

void BotClient_Gearbox_WeaponList( void *p, int bot_index )
{
    // this is just like the Valve Weapon List message
    BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_Hunger_WeaponList( void *p, int bot_index )
{
    // this is just like the Valve Weapon List message
    BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_NS_WeaponList( void *p, int bot_index )
{
    // this is just like the Valve Weapon List message
    BotClient_Valve_WeaponList(p, bot_index);
}

void BotClient_Gunman_WeaponList( void *p, int bot_index )
{
	static int state = 0;    // current state machine state
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
        bot_weapon.stuff[0] = *(int *)p;
    }
	else if (state == 11)
    {
        state++;
        bot_weapon.stuff[1] = *(int *)p;
    }
	else if (state == 12)
    {
        state = 0;
        bot_weapon.stuff[2] = *(int *)p;

		weapon_defs[bot_weapon.iId] = bot_weapon;
    }
}


// This message is sent when a weapon is selected (either by the bot chosing
// a weapon or by the server auto assigning the bot a weapon).
void BotClient_Valve_CurrentWeapon( void *p, int bot_index )
{
    static int state = 0;    // current state machine state
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
                bots[bot_index].current_weapon.iId = iId;
                bots[bot_index].current_weapon.iClip = iClip;

                // update the ammo counts for this weapon...
                bots[bot_index].current_weapon.iAmmo1 =
                    bots[bot_index].m_rgAmmo[weapon_defs[iId].iAmmo1];
                bots[bot_index].current_weapon.iAmmo2 =
                    bots[bot_index].m_rgAmmo[weapon_defs[iId].iAmmo2];
            }
        }
    }
}

void BotClient_Gearbox_CurrentWeapon( void *p, int bot_index )
{
    // this is just like the Valve Current Weapon message
    BotClient_Valve_CurrentWeapon(p, bot_index);
}

void BotClient_NS_CurrentWeapon( void *p, int bot_index )
{
    // this is just like the Valve Current Weapon message
    BotClient_Valve_CurrentWeapon(p, bot_index);
}

// This message is sent whenever ammo ammounts are adjusted (up or down).
void BotClient_Valve_AmmoX( void *p, int bot_index )
{
    static int state = 0;    // current state machine state
    static int index;
    static int amount;
    int ammo_index;

    if (state == 0)
    {
        state++;
        index = *(int *)p;  // ammo index (for type of ammo)
    }
    else if (state == 1)
    {
        state = 0;

        amount = *(int *)p;  // the ammount of ammo currently available

        bots[bot_index].m_rgAmmo[index] = amount;  // store it away

        ammo_index = bots[bot_index].current_weapon.iId;

        // update the ammo counts for this weapon...
        bots[bot_index].current_weapon.iAmmo1 = bots[bot_index].m_rgAmmo[weapon_defs[ammo_index].iAmmo1];
        bots[bot_index].current_weapon.iAmmo2 = bots[bot_index].m_rgAmmo[weapon_defs[ammo_index].iAmmo2];
    }
}

void BotClient_Gearbox_AmmoX( void *p, int bot_index )
{
    // this is just like the Valve AmmoX message
    BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_Gunman_AmmoX( void *p, int bot_index )
{
    // this is just like the Valve AmmoX message
    BotClient_Valve_AmmoX(p, bot_index);
}

void BotClient_NS_AmmoX( void *p, int bot_index )
{
    // this is just like the Valve AmmoX message
    BotClient_Valve_AmmoX(p, bot_index);
}

// This message is sent when the bot picks up some ammo (AmmoX messages are
// also sent so this message is probably not really necessary except it
// allows the HUD to draw pictures of ammo that have been picked up.  The
// bots don't really need pictures since they don't have any eyes anyway.
void BotClient_Valve_AmmoPickup( void *p, int bot_index )
{
    static int state = 0;    // current state machine state
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

        bots[bot_index].m_rgAmmo[index] = ammount;

        ammo_index = bots[bot_index].current_weapon.iId;

        // update the ammo counts for this weapon...
        bots[bot_index].current_weapon.iAmmo1 =
            bots[bot_index].m_rgAmmo[weapon_defs[ammo_index].iAmmo1];
        bots[bot_index].current_weapon.iAmmo2 =
            bots[bot_index].m_rgAmmo[weapon_defs[ammo_index].iAmmo2];
    }
}

void BotClient_Gearbox_AmmoPickup( void *p, int bot_index )
{
    // this is just like the Valve Ammo Pickup message
    BotClient_Valve_AmmoPickup(p, bot_index);
}

void BotClient_NS_AmmoPickup( void *p, int bot_index )
{
    // this is just like the Valve Ammo Pickup message
    BotClient_Valve_AmmoPickup(p, bot_index);
}

// This message gets sent when the bots are getting damaged.
void BotClient_Valve_Damage( void *p, int bot_index )
{
    static int state = 0;    // current state machine state
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
            if( !bots[bot_index].pBotEnemy )
            {
                // face the attacker...
                Vector v_enemy = damage_origin - bots[bot_index].pEdict->v.origin;
                Vector bot_angles = UTIL_VecToAngles( v_enemy );

                bots[bot_index].pEdict->v.ideal_yaw = bot_angles.y;

                BotFixIdealYaw( bots[bot_index].pEdict );

                // stop using things...
                bots[bot_index].bUseHealthCharger = FALSE;
                bots[bot_index].bUseArmorCharger = FALSE;

				bots[bot_index].bBuild = false;
				bots[bot_index].bBuildAlienResourceTower = false;
				bots[bot_index].bUseArmory = false;

				bots[bot_index].bUseDoor = false;
            }
        }
    }
}

void BotClient_Gearbox_Damage( void *p, int bot_index )
{
    // this is just like the Valve Battery message
    BotClient_Valve_Damage(p, bot_index);
}

void BotClient_NS_Damage( void *p, int bot_index )
{
    // this is just like the Valve Battery message
    BotClient_Valve_Damage(p, bot_index);
}

// This message gets sent when the bots get killed
void BotClient_Valve_DeathMsg( void *p, int bot_index )
{
    static int state = 0;    // current state machine state
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
                bots[index].killer_edict = NULL;
            }
            else
            {
                // store edict of player that killed this bot...
                bots[index].killer_edict = INDEXENT(killer_index);
            }
        }
    }
}

void BotClient_Gearbox_DeathMsg( void *p, int bot_index )
{
    // this is just like the Valve DeathMsg message
    BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_Gunman_DeathMsg( void *p, int bot_index )
{
    // this is just like the Valve DeathMsg message
    BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_NS_DeathMsg( void *p, int bot_index )
{
    // this is just like the Valve DeathMsg message
    BotClient_Valve_DeathMsg(p, bot_index);
}

void BotClient_Valve_ScreenFade( void *p, int bot_index )
{
	static int state = 0;    // current state machine state
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
		bots[bot_index].blinded_time = gpGlobals->time + length - 2.0;
	}
	else
	{
		state++;
	}
}

void BotClient_Gearbox_ScreenFade( void *p, int bot_index )
{
    // this is just like the Valve ScreenFade message
    BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_NS_ScreenFade( void *p, int bot_index )
{
    // this is just like the Valve ScreenFade message
    BotClient_Valve_ScreenFade(p, bot_index);
}

void BotClient_NS_CountDown( void *p, int bot_index )
{
	// reset these values
	g_bInGame = false;
	g_fCountDownTime = 0.0;

	g_iCountDown = *(int *)p;
}
