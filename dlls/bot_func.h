//
// gbot - The GoldSource bot
//
// <no site>
//
// bot_func.h
//

#ifndef BOT_FUNC_H
#define BOT_FUNC_H


// prototypes for bot functions

void BotSpawnInit( bot_t *pBot );
void BotCreate();
void BotStartGame( bot_t *pBot );
int BotInFieldOfView( bot_t *pBot, Vector dest );
bool BotEntityIsVisible( bot_t *pBot, Vector dest );
void BotFindItem( bot_t *pBot );
void BotThink( bot_t *pBot );

BOOL Bot_IsTeamplay();


void BotFixIdealPitch( edict_t *pEdict );
float BotChangePitch( bot_t *pBot, float speed );
void BotFixIdealYaw( edict_t *pEdict );
float BotChangeYaw( bot_t *pBot, float speed );
void BotFixBodyAngles(edict_t *pEdict);
void BotFixViewAngles(edict_t *pEdict);
bool BotFindWaypoint( bot_t *pBot );
bool BotHeadTowardWaypoint( bot_t *pBot );
void BotOnLadder( bot_t *pBot, float moved_distance );
void BotUnderWater( bot_t *pBot );
void BotUseLift( bot_t *pBot, float moved_distance );
void BotUseDoor( bot_t *pBot );
bool BotStuckInCorner( bot_t *pBot );
void BotTurnAtWall( bot_t *pBot, TraceResult *tr );
bool BotCantMoveForward( bot_t *pBot, TraceResult *tr );
bool BotCanJumpUp( bot_t *pBot );
bool BotCanDuckUnder( bot_t *pBot );
void BotRandomTurn( bot_t *pBot );
bool BotFollowUser( bot_t *pBot );
bool BotCheckWallOnLeft( bot_t *pBot );
bool BotCheckWallOnRight( bot_t *pBot );

edict_t *BotFindEnemy( bot_t *pBot );
edict_t *BotFindFriend( bot_t *pBot );
Vector BotBodyTarget( edict_t *pBotEnemy, bot_t *pBot );
bool BotFireWeapon( Vector v_enemy, bot_t *pBot, int iWeapon );

#define FRIEND	true
#define ENEMY	false

void Bot_Shoot( bot_t *pBot, bool bIsTargetFriend );
bool BotShootTripmine( bot_t *pBot );


#endif // BOT_FUNC_H

