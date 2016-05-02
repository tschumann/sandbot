//
// gbot - The GoldSource bot
//
// <no site>
//
// bot_client.h
//

void BotClient_Valve_WeaponList(void *p, int bot_index);
void BotClient_Gearbox_WeaponList(void *p, int bot_index);
void BotClient_Gunman_WeaponList(void *p, int bot_index);
void BotClient_NS_WeaponList(void *p, int bot_index);

void BotClient_Valve_CurrentWeapon(void *p, int bot_index);
void BotClient_Gearbox_CurrentWeapon(void *p, int bot_index);
void BotClient_NS_CurrentWeapon(void *p, int bot_index);

void BotClient_Valve_AmmoX(void *p, int bot_index);
void BotClient_Gearbox_AmmoX(void *p, int bot_index);
void BotClient_NS_AmmoX(void *p, int bot_index);

void BotClient_Valve_AmmoPickup(void *p, int bot_index);
void BotClient_Gearbox_AmmoPickup(void *p, int bot_index);
void BotClient_NS_AmmoPickup(void *p, int bot_index);

void BotClient_Valve_Damage(void *p, int bot_index);
void BotClient_Gearbox_Damage(void *p, int bot_index);
void BotClient_NS_Damage(void *p, int bot_index);

void BotClient_Valve_DeathMsg(void *p, int bot_index);
void BotClient_Gearbox_DeathMsg(void *p, int bot_index);
void BotClient_Gunman_DeathMsg(void *p, int bot_index);
void BotClient_NS_DeathMsg(void *p, int bot_index);

void BotClient_Valve_ScreenFade(void *p, int bot_index);
void BotClient_Gearbox_ScreenFade(void *p, int bot_index);
void BotClient_NS_ScreenFade(void *p, int bot_index);

void BotClient_NS_CountDown(void *p, int bot_index);

