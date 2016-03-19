//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// dll.h
//

#ifndef DLL_H
#define DLL_H

// DLL prototypes (from engine\eiface.h)...
void GameDLLInit( void );
int DispatchSpawn( edict_t *pent );
void DispatchThink( edict_t *pent );
void DispatchUse( edict_t *pentUsed, edict_t *pentOther );
void DispatchTouch( edict_t *pentTouched, edict_t *pentOther );
void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther );
void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd );
void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData );
int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
void DispatchObjectCollisionBox( edict_t *pent );
void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
void SaveGlobalState( SAVERESTOREDATA *pSaveData );
void RestoreGlobalState( SAVERESTOREDATA *pSaveData );
void ResetGlobalState( void );
BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  );
void ClientDisconnect( edict_t *pEntity );
void ClientKill( edict_t *pEntity );
void ClientPutInServer( edict_t *pEntity );
void ClientCommand( edict_t *pEntity );
void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer );
void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
void ServerDeactivate( void );
void PlayerPreThink( edict_t *pEntity );
void PlayerPostThink( edict_t *pEntity );
void StartFrame( void );
void ParmsNewLevel( void );
void ParmsChangeLevel( void );
const char *GetGameDescription( void );
void PlayerCustomization( edict_t *pEntity, customization_t *pCust );
void SpectatorConnect( edict_t *pEntity );
void SpectatorDisconnect( edict_t *pEntity );
void SpectatorThink( edict_t *pEntity );
void Sys_Error( const char *error_string );
void PM_Move ( struct playermove_s *ppmove, int server );
void PM_Init ( struct playermove_s *ppmove );
char PM_FindTextureType( char *name );
void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas );
void UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd );
int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet );
void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs );
void RegisterEncoders( void );
int GetWeaponData( struct edict_s *player, struct weapon_data_s *info );
void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed );
void CmdEnd ( const edict_t *player );
int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
int GetHullBounds( int hullnumber, float *mins, float *maxs );
void CreateInstancedBaselines( void );
int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message );
int AllowLagCompensation( void );

#endif // DLL_H

