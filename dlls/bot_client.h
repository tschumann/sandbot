//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// bot_client.h
//

#ifndef BOTCLIENT_H
#define BOTCLIENT_H

void BotClient_CS_VGUI(void *p, int bot_index);
void BotClient_DOD_VGUI(void *p, int bot_index);
void BotClient_TFC_VGUI(void *p, int bot_index);
void BotClient_CS_ShowMenu(void *p, int bot_index);
void BotClient_Gearbox_VGUI(void *p, int bot_index);

void BotClient_Valve_WeaponList(void *p, int bot_index);
void BotClient_Gearbox_WeaponList(void *p, int bot_index);
void BotClient_CS_WeaponList(void *p, int bot_index);
void BotClient_DOD_WeaponList(void *p, int bot_index);
void BotClient_TFC_WeaponList(void *p, int bot_index);
void BotClient_Gunman_WeaponList(void *p, int bot_index);
void BotClient_NS_WeaponList(void *p, int bot_index);
void BotClient_Hunger_WeaponList(void *p, int bot_index);
void BotClient_Ship_WeaponList(void *p, int bot_index);

void BotClient_Valve_CurrentWeapon(void *p, int bot_index);
void BotClient_Gearbox_CurrentWeapon(void *p, int bot_index);
void BotClient_CS_CurrentWeapon(void *p, int bot_index);
void BotClient_TFC_CurrentWeapon(void *p, int bot_index);
void BotClient_Gunman_CurrentWeapon(void *p, int bot_index);
void BotClient_NS_CurrentWeapon(void *p, int bot_index);
void BotClient_Hunger_CurrentWeapon(void *p, int bot_index);
void BotClient_Ship_CurrentWeapon(void *p, int bot_index);

void BotClient_Valve_AmmoX(void *p, int bot_index);
void BotClient_Gearbox_AmmoX(void *p, int bot_index);
void BotClient_CS_AmmoX(void *p, int bot_index);
void BotClient_TFC_AmmoX(void *p, int bot_index);
void BotClient_Gunman_AmmoX(void *p, int bot_index);
void BotClient_NS_AmmoX(void *p, int bot_index);
void BotClient_Hunger_AmmoX(void *p, int bot_index);
void BotClient_Ship_AmmoX(void *p, int bot_index);

void BotClient_Valve_AmmoPickup(void *p, int bot_index);
void BotClient_Gearbox_AmmoPickup(void *p, int bot_index);
void BotClient_CS_AmmoPickup(void *p, int bot_index);
void BotClient_TFC_AmmoPickup(void *p, int bot_index);
void BotClient_Gunman_AmmoPickup(void *p, int bot_index);
void BotClient_NS_AmmoPickup(void *p, int bot_index);
void BotClient_Hunger_AmmoPickup(void *p, int bot_index);
void BotClient_Ship_AmmoPickup(void *p, int bot_index);

void BotClient_Valve_Damage(void *p, int bot_index);
void BotClient_Gearbox_Damage(void *p, int bot_index);
void BotClient_CS_Damage(void *p, int bot_index);
void BotClient_TFC_Damage(void *p, int bot_index);
void BotClient_Gunman_Damage(void *p, int bot_index);
void BotClient_NS_Damage(void *p, int bot_index);
void BotClient_Hunger_Damage(void *p, int bot_index);
void BotClient_Ship_Damage(void *p, int bot_index);

void BotClient_CS_Money(void *p, int bot_index);

void BotClient_Valve_DeathMsg(void *p, int bot_index);
void BotClient_Gearbox_DeathMsg(void *p, int bot_index);
void BotClient_CS_DeathMsg(void *p, int bot_index);
void BotClient_TFC_DeathMsg(void *p, int bot_index);
void BotClient_Gunman_DeathMsg(void *p, int bot_index);
void BotClient_NS_DeathMsg(void *p, int bot_index);
void BotClient_Hunger_DeathMsg(void *p, int bot_index);

void BotClient_Valve_ScreenFade(void *p, int bot_index);
void BotClient_Gearbox_ScreenFade(void *p, int bot_index);
void BotClient_CS_ScreenFade(void *p, int bot_index);
void BotClient_TFC_ScreenFade(void *p, int bot_index);
void BotClient_Gunman_ScreenFade(void *p, int bot_index);
void BotClient_NS_ScreenFade(void *p, int bot_index);
void BotClient_Hunger_ScreenFade(void *p, int bot_index);
void BotClient_Ship_ScreenFade(void *p, int bot_index);

void BotClient_CS_HLTV(void *p, int bot_index);

void BotClient_NS_CountDown( void *p, int bot_index );
void BotClient_NS_GameStatus( void *p, int bot_index );

void BotClient_Ship_Quarry( void *p, int bot_index );

#endif // BOTCLIENT_H
