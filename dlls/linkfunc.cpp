//
// HPB bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// linkfunc.cpp
//

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "studio.h"

#include "bot.h"
#include "linkfunc.h"
#include "h_export.h"

#define LINK_ENTITY_TO_FUNC(mapClassName) \
	extern "C" EXPORT void mapClassName( entvars_t *pev ); \
	void mapClassName( entvars_t *pev ) { \
		static LINK_ENTITY_FUNC otherClassName = NULL; \
		static int skip_this = 0; \
		if (skip_this) return; \
		if (otherClassName == NULL) \
			otherClassName = (LINK_ENTITY_FUNC)GetProcAddress(h_Library, #mapClassName); \
		if (otherClassName == NULL) { \
			skip_this = 1; return; \
		} \
		(*otherClassName)(pev); }

// to get singleplayer working the below two functions need to know about
// the mangled name exports - take a look at https://msdn.microsoft.com/en-us/library/ms809762.aspx
// in conjuction with singlep.cpp from MetaMod singleplayer to get an idea
// about how to translate offsets etc, or better yet just use the output of
// dumpbin in conjunction with the real return values of the below methods
// to figure it out - that way the export scripts can be modified to output
// appropriate offsets etc and the output can just be dumped into a .cpp
// file so the implementation of pfnFunctionFromName and pfnNameForFunction
// can be platform independent (hopefully) while the offsets etc will be
// platform dependent (which is unavoidable)

extern char g_szLibraryPath[64];

#define MAX_FUNCTION_NAME_LENGTH 256

#ifdef WIN32
// offset of first exported function?
int g_iBaseOffset = 0;
int g_iOrdinalCount = 0;

char szFunctionNames[4096][MAX_FUNCTION_NAME_LENGTH];

WORD *pOrdinals = nullptr;
DWORD *pFunctionAddresses = nullptr;
DWORD *pNameAddresses = nullptr;

// https://blog.kowalczyk.info/articles/pefileformat.html has a lot of useful information
void LoadExtraExports()
{
	IMAGE_DOS_HEADER sDOSHeader;
	LONG iNTSignature;
	IMAGE_FILE_HEADER sPEHeader;
	IMAGE_OPTIONAL_HEADER sOptionalHeader;
	IMAGE_SECTION_HEADER *pSectionHeader;
	LONG iedataOffset;
	LONG iedataDelta;
	IMAGE_EXPORT_DIRECTORY sExportDirectory;

	// open the game .dll
	FILE *pFile = fopen( g_szLibraryPath, "rb" );

	if( !pFile )
	{
		ALERT( at_error, "Unable to fopen %s with errno %d\n", g_szLibraryPath, errno );

		return;
	}

	// read the DOS header
	fread( &sDOSHeader, sizeof(IMAGE_DOS_HEADER), 1, pFile );
	// go to the PE header
	fseek(pFile, sDOSHeader.e_lfanew, SEEK_SET );
	// reader the NT signature
	fread( &iNTSignature, sizeof(iNTSignature), 1, pFile );
	// read the PE header
	fread( &sPEHeader, sizeof(IMAGE_FILE_HEADER), 1, pFile );
	// read the optional header
	fread( &sOptionalHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, pFile );

	iedataOffset = sOptionalHeader.DataDirectory[0].VirtualAddress;
	iedataDelta = 0;

	// TODO: read into sHeader instead of creating a fake sHeader for IMAGE_FIRST_SECTION
	IMAGE_NT_HEADERS32 sHeader;
	sHeader.Signature = iNTSignature;
	sHeader.FileHeader = sPEHeader;
	sHeader.OptionalHeader = sOptionalHeader;

	pSectionHeader = IMAGE_FIRST_SECTION(&sHeader);

	// look at each section
	for( int i = 0; i < sPEHeader.NumberOfSections; i++ )
	{
		// if it's the export data
		if( !strcmp( (char *)pSectionHeader->Name, ".edata" ) )
		{
			iedataOffset = pSectionHeader->PointerToRawData;
			iedataDelta = pSectionHeader->VirtualAddress - pSectionHeader->PointerToRawData;
			break;
		}
		else
		{
			// try the next one
			pSectionHeader++;
		}
	}

	fseek( pFile, iedataOffset, SEEK_SET );
	// read the export directory
	fread( &sExportDirectory, sizeof(IMAGE_EXPORT_DIRECTORY), 1, pFile );

	// save number of ordinals
	g_iOrdinalCount = sExportDirectory.NumberOfNames;

	// remember the offset to the ordinals
	LONG iOrdinalOffset = sExportDirectory.AddressOfNameOrdinals - iedataDelta;
	fseek( pFile, iOrdinalOffset, SEEK_SET );
	// allocate space for ordinals
	pOrdinals = new WORD[g_iOrdinalCount];
	// get the list of ordinals
	fread( pOrdinals, g_iOrdinalCount * sizeof(WORD), 1, pFile );

	// remember the offset to the function addresses
	LONG iFunctionOffset = sExportDirectory.AddressOfFunctions - iedataDelta;
	fseek( pFile, iFunctionOffset, SEEK_SET );
	pFunctionAddresses = new DWORD[g_iOrdinalCount];
	// get the list of functions
	fread( pFunctionAddresses, g_iOrdinalCount * sizeof(DWORD), 1, pFile );

	// remember the offset to the name addresses
	LONG iNameOffset = sExportDirectory.AddressOfNames - iedataDelta;
	fseek( pFile, iNameOffset, SEEK_SET );
	// allocate space for names
	pNameAddresses = new DWORD[g_iOrdinalCount];
	// get the list of names
	fread( pNameAddresses, g_iOrdinalCount * sizeof(DWORD), 1, pFile );

	// populate the exports array
	for( int i = 0; i < g_iOrdinalCount; i++ )
	{
		if( !fseek( pFile, pNameAddresses[i] - iedataDelta, SEEK_SET ) )
		{
			char szFunctionName[MAX_FUNCTION_NAME_LENGTH];
			char *pFunctionName;
			char *pEnd;

			int iFunctionNameLength = fread( szFunctionName, sizeof(char), sizeof(szFunctionName) - 1, pFile );
			szFunctionName[iFunctionNameLength - 1] = '\0';
			ALERT( at_console, "Found %s\n", szFunctionName );

			pFunctionName = szFunctionName;
			// possibly skip the leading ? in a Visual Studio mangled name
			if( pFunctionName[0] == '?' )
			{
				pFunctionName++;
			}
			// possibly truncate after @@ in a Visual Studio mangled name
			if( pEnd = strstr(pFunctionName, "@@") )
			{
				*pEnd = '\0';
			}

			strncpy( szFunctionNames[i], pFunctionName, strlen( pFunctionName ) );
			ALERT( at_console, "Stored %s\n", szFunctionNames[i] );
		}
	}

	fclose( pFile );

	// cycle through all function names to find the GiveFnptrsToDll function
	for( int i = 0; i < g_iOrdinalCount; i++ )
	{
		if( !strcmp( "GiveFnptrsToDll", szFunctionNames[i] ) )
		{
			void *pGiveFnptrsToDll = (void *)GetProcAddress( h_Library, "GiveFnptrsToDll" );
			g_iBaseOffset = (unsigned long)(pGiveFnptrsToDll) - pFunctionAddresses[pOrdinals[i]];
			break;
		}
	}
}

uint32 NameToAddress( const char *pName )
{
	uint32 iAddress = 0;

	for( int i = 0; i < g_iOrdinalCount; i++ )
	{
		if( !strcmp( pName, szFunctionNames[i] ) )
		{
			// get the address of the function
			iAddress = pFunctionAddresses[pOrdinals[i]] + g_iBaseOffset;
		}
	}

	return iAddress;
}

const char *AddressToName(uint32 function)
{
	const char *szName = NULL;

	for( int i = 0; i < g_iOrdinalCount; i++ )
	{
		if( (function - g_iBaseOffset) == pFunctionAddresses[pOrdinals[i]] )
		{
			szName = szFunctionNames[i];
		}
	}

	return szName;
}
#endif // WIN32

#ifdef __linux__
void LoadExtraExports()
{
	//
}

uint32 NameToAddress(const char *pName)
{
	uint32 iAddress = 0;

	return iAddress;
}

const char *AddressToName( uint32 function )
{
	const char *szName = NULL;

	return szName;
}
#endif // __linux__

// TODO: these aren't entities?
// LINK_ENTITY_TO_FUNC(CountTeamPlayers);
// LINK_ENTITY_TO_FUNC(CountTeams);
// LINK_ENTITY_TO_FUNC(DelayedUse);
// LINK_ENTITY_TO_FUNC(DmlHandGrenade);
LINK_ENTITY_TO_FUNC(aiscripted_sequence);
LINK_ENTITY_TO_FUNC(alienresourcetower);
LINK_ENTITY_TO_FUNC(ambient_generic);
LINK_ENTITY_TO_FUNC(ammo_30cal);
LINK_ENTITY_TO_FUNC(ammo_338magnum);
LINK_ENTITY_TO_FUNC(ammo_357);
LINK_ENTITY_TO_FUNC(ammo_357sig);
LINK_ENTITY_TO_FUNC(ammo_45acp);
LINK_ENTITY_TO_FUNC(ammo_50ae);
LINK_ENTITY_TO_FUNC(ammo_556);
LINK_ENTITY_TO_FUNC(ammo_556nato);
LINK_ENTITY_TO_FUNC(ammo_556natobox);
LINK_ENTITY_TO_FUNC(ammo_57mm);
LINK_ENTITY_TO_FUNC(ammo_762);
LINK_ENTITY_TO_FUNC(ammo_762nato);
LINK_ENTITY_TO_FUNC(ammo_9mm);
LINK_ENTITY_TO_FUNC(ammo_9mmAR);
LINK_ENTITY_TO_FUNC(ammo_9mmbox);
LINK_ENTITY_TO_FUNC(ammo_9mmclip);
LINK_ENTITY_TO_FUNC(ammo_ARgrenades);
LINK_ENTITY_TO_FUNC(ammo_bar);
LINK_ENTITY_TO_FUNC(ammo_bazooka);
LINK_ENTITY_TO_FUNC(ammo_beamgunclip);
LINK_ENTITY_TO_FUNC(ammo_bren);
LINK_ENTITY_TO_FUNC(ammo_buckshot);
LINK_ENTITY_TO_FUNC(ammo_chemical);
LINK_ENTITY_TO_FUNC(ammo_colt);
LINK_ENTITY_TO_FUNC(ammo_crossbow);
LINK_ENTITY_TO_FUNC(ammo_dmlclip);
LINK_ENTITY_TO_FUNC(ammo_dmlsingle);
LINK_ENTITY_TO_FUNC(ammo_eagleclip);
LINK_ENTITY_TO_FUNC(ammo_egonclip);
LINK_ENTITY_TO_FUNC(ammo_einar1);
LINK_ENTITY_TO_FUNC(ammo_enfield);
LINK_ENTITY_TO_FUNC(ammo_fg42);
LINK_ENTITY_TO_FUNC(ammo_garand);
LINK_ENTITY_TO_FUNC(ammo_gaussclip);
LINK_ENTITY_TO_FUNC(ammo_generic_american);
LINK_ENTITY_TO_FUNC(ammo_generic_british);
LINK_ENTITY_TO_FUNC(ammo_generic_german);
LINK_ENTITY_TO_FUNC(ammo_glockclip);
LINK_ENTITY_TO_FUNC(ammo_greasegun);
LINK_ENTITY_TO_FUNC(ammo_k43);
LINK_ENTITY_TO_FUNC(ammo_kar);
LINK_ENTITY_TO_FUNC(ammo_luger);
LINK_ENTITY_TO_FUNC(ammo_m1carbine);
LINK_ENTITY_TO_FUNC(ammo_mechgunClip);
LINK_ENTITY_TO_FUNC(ammo_mg34);
LINK_ENTITY_TO_FUNC(ammo_mg42);
LINK_ENTITY_TO_FUNC(ammo_minigunClip);
LINK_ENTITY_TO_FUNC(ammo_mp40);
LINK_ENTITY_TO_FUNC(ammo_mp44);
LINK_ENTITY_TO_FUNC(ammo_mp5clip);
LINK_ENTITY_TO_FUNC(ammo_mp5grenades);
LINK_ENTITY_TO_FUNC(ammo_piat);
LINK_ENTITY_TO_FUNC(ammo_pschreck);
LINK_ENTITY_TO_FUNC(ammo_rpgclip);
LINK_ENTITY_TO_FUNC(ammo_scopedkar);
LINK_ENTITY_TO_FUNC(ammo_spore);
LINK_ENTITY_TO_FUNC(ammo_spring);
LINK_ENTITY_TO_FUNC(ammo_sten);
LINK_ENTITY_TO_FUNC(ammo_th_ap9);
LINK_ENTITY_TO_FUNC(ammo_th_sniper);
LINK_ENTITY_TO_FUNC(ammo_th_taurus);
LINK_ENTITY_TO_FUNC(ammo_thompson);
LINK_ENTITY_TO_FUNC(ammo_webley);
LINK_ENTITY_TO_FUNC(anime_rocket);
LINK_ENTITY_TO_FUNC(animebackpack_rocket);
LINK_ENTITY_TO_FUNC(antirocketflare);
LINK_ENTITY_TO_FUNC(armoury_entity);
LINK_ENTITY_TO_FUNC(authenticationmanager);
LINK_ENTITY_TO_FUNC(ball_lightning);
LINK_ENTITY_TO_FUNC(bankinterest);
LINK_ENTITY_TO_FUNC(beakbirther_gib);
LINK_ENTITY_TO_FUNC(beam);
LINK_ENTITY_TO_FUNC(bmortar);
LINK_ENTITY_TO_FUNC(bodyque);
LINK_ENTITY_TO_FUNC(boulder);
LINK_ENTITY_TO_FUNC(building_dispenser);
LINK_ENTITY_TO_FUNC(building_sentrygun);
LINK_ENTITY_TO_FUNC(building_sentrygun_base);
LINK_ENTITY_TO_FUNC(building_teleporter);
LINK_ENTITY_TO_FUNC(button_aiwallplug);
LINK_ENTITY_TO_FUNC(button_target);
LINK_ENTITY_TO_FUNC(charged_bolt);
LINK_ENTITY_TO_FUNC(chem_grenade);
LINK_ENTITY_TO_FUNC(cine_blood);
LINK_ENTITY_TO_FUNC(controller_energy_ball);
LINK_ENTITY_TO_FUNC(controller_head_ball);
LINK_ENTITY_TO_FUNC(critter_gib);
LINK_ENTITY_TO_FUNC(crossbow_bolt);
LINK_ENTITY_TO_FUNC(ctf_hudicon);
LINK_ENTITY_TO_FUNC(cust_1BeamFrequency);
LINK_ENTITY_TO_FUNC(cust_1DMLGrenDetonate);
LINK_ENTITY_TO_FUNC(cust_1DMLLaunch);
LINK_ENTITY_TO_FUNC(cust_1GaussPistolFireType);
LINK_ENTITY_TO_FUNC(cust_1MinigunSpin);
LINK_ENTITY_TO_FUNC(cust_1SPChemicalGunPressure);
LINK_ENTITY_TO_FUNC(cust_1ShotgunSpread);
LINK_ENTITY_TO_FUNC(cust_1fistsKnife);
LINK_ENTITY_TO_FUNC(cust_2BeamCurrent);
LINK_ENTITY_TO_FUNC(cust_2ChemAcid);
LINK_ENTITY_TO_FUNC(cust_2DMLFlightpath);
LINK_ENTITY_TO_FUNC(cust_2DMLGrenPayload);
LINK_ENTITY_TO_FUNC(cust_2GaussPistolSniper);
LINK_ENTITY_TO_FUNC(cust_2MinigunCooled);
LINK_ENTITY_TO_FUNC(cust_2ShotgunShells);
LINK_ENTITY_TO_FUNC(cust_3BeamVoltage);
LINK_ENTITY_TO_FUNC(cust_3ChemBase);
LINK_ENTITY_TO_FUNC(cust_3DMLDetonate);
LINK_ENTITY_TO_FUNC(cust_4ChemNuet);
LINK_ENTITY_TO_FUNC(cust_4DMLPayload);
LINK_ENTITY_TO_FUNC(customization_coolers);
LINK_ENTITY_TO_FUNC(customization_knife);
LINK_ENTITY_TO_FUNC(customization_sniper);
LINK_ENTITY_TO_FUNC(cycler);
LINK_ENTITY_TO_FUNC(cycler_prdroid);
LINK_ENTITY_TO_FUNC(cycler_sprite);
LINK_ENTITY_TO_FUNC(cycler_weapon);
LINK_ENTITY_TO_FUNC(cycler_wreckage);
LINK_ENTITY_TO_FUNC(decore_asteroid);
LINK_ENTITY_TO_FUNC(decore_baboon);
LINK_ENTITY_TO_FUNC(decore_bodygib);
LINK_ENTITY_TO_FUNC(decore_butterflyflock);
LINK_ENTITY_TO_FUNC(decore_cactus);
LINK_ENTITY_TO_FUNC(decore_cam);
LINK_ENTITY_TO_FUNC(decore_camflare);
LINK_ENTITY_TO_FUNC(decore_eagle);
LINK_ENTITY_TO_FUNC(decore_explodable);
LINK_ENTITY_TO_FUNC(decore_foot);
LINK_ENTITY_TO_FUNC(decore_goldskull);
LINK_ENTITY_TO_FUNC(decore_gutspile);
LINK_ENTITY_TO_FUNC(decore_hatgib);
LINK_ENTITY_TO_FUNC(decore_ice);
LINK_ENTITY_TO_FUNC(decore_icebeak);
LINK_ENTITY_TO_FUNC(decore_labstuff);
LINK_ENTITY_TO_FUNC(decore_mushroom);
LINK_ENTITY_TO_FUNC(decore_mushroom2);
LINK_ENTITY_TO_FUNC(decore_nest);
LINK_ENTITY_TO_FUNC(decore_pipes);
LINK_ENTITY_TO_FUNC(decore_prickle);
LINK_ENTITY_TO_FUNC(decore_pteradon);
LINK_ENTITY_TO_FUNC(decore_sack);
LINK_ENTITY_TO_FUNC(decore_sittingtubemortar);
LINK_ENTITY_TO_FUNC(decore_spacedebris);
LINK_ENTITY_TO_FUNC(decore_swampplants);
LINK_ENTITY_TO_FUNC(decore_torch);
LINK_ENTITY_TO_FUNC(decore_torchflame);
LINK_ENTITY_TO_FUNC(defensechamber);
LINK_ENTITY_TO_FUNC(demoman_mine);
LINK_ENTITY_TO_FUNC(demoman_rocket);
LINK_ENTITY_TO_FUNC(detpack);
LINK_ENTITY_TO_FUNC(dispenser_refill_timer);
LINK_ENTITY_TO_FUNC(displacer_ball);
LINK_ENTITY_TO_FUNC(dmlRocket);
LINK_ENTITY_TO_FUNC(dml_cluster);
LINK_ENTITY_TO_FUNC(dod_ammo);
LINK_ENTITY_TO_FUNC(dod_camera);
LINK_ENTITY_TO_FUNC(dod_capture_area);
LINK_ENTITY_TO_FUNC(dod_control_point);
LINK_ENTITY_TO_FUNC(dod_control_point_master);
LINK_ENTITY_TO_FUNC(dod_location);
LINK_ENTITY_TO_FUNC(dod_mapmarker);
LINK_ENTITY_TO_FUNC(dod_object);
LINK_ENTITY_TO_FUNC(dod_object_goal);
LINK_ENTITY_TO_FUNC(dod_point_relay);
LINK_ENTITY_TO_FUNC(dod_preround);
LINK_ENTITY_TO_FUNC(dod_round_timer);
LINK_ENTITY_TO_FUNC(dod_score_ent);
LINK_ENTITY_TO_FUNC(dod_stats);
LINK_ENTITY_TO_FUNC(dod_trigger_sandbag);
LINK_ENTITY_TO_FUNC(eagle_laser);
LINK_ENTITY_TO_FUNC(einar_civ);
LINK_ENTITY_TO_FUNC(einar_civ_dead);
LINK_ENTITY_TO_FUNC(einar_civ_sit);
LINK_ENTITY_TO_FUNC(einar_flame);
LINK_ENTITY_TO_FUNC(einar_hand);
LINK_ENTITY_TO_FUNC(endboss_gib);
LINK_ENTITY_TO_FUNC(endboss_kataball);
LINK_ENTITY_TO_FUNC(endboss_rocket);
LINK_ENTITY_TO_FUNC(entity_aicoreTemp);
LINK_ENTITY_TO_FUNC(entity_clustergod);
LINK_ENTITY_TO_FUNC(entity_digit);
LINK_ENTITY_TO_FUNC(entity_digitgod);
LINK_ENTITY_TO_FUNC(entity_null);
LINK_ENTITY_TO_FUNC(entity_spritegod);
LINK_ENTITY_TO_FUNC(entity_volcanospew);
LINK_ENTITY_TO_FUNC(env_beam);
LINK_ENTITY_TO_FUNC(env_beverage);
LINK_ENTITY_TO_FUNC(env_blood);
LINK_ENTITY_TO_FUNC(env_blowercannon);
LINK_ENTITY_TO_FUNC(env_bombglow);
LINK_ENTITY_TO_FUNC(env_bubbles);
LINK_ENTITY_TO_FUNC(env_clusterExplosion);
LINK_ENTITY_TO_FUNC(env_customize);
LINK_ENTITY_TO_FUNC(env_debris);
LINK_ENTITY_TO_FUNC(env_electrified_wire);
LINK_ENTITY_TO_FUNC(env_explosion);
LINK_ENTITY_TO_FUNC(env_fade);
LINK_ENTITY_TO_FUNC(env_fog);
LINK_ENTITY_TO_FUNC(env_funnel);
LINK_ENTITY_TO_FUNC(env_gamma);
LINK_ENTITY_TO_FUNC(env_genewormcloud);
LINK_ENTITY_TO_FUNC(env_genewormspawn);
LINK_ENTITY_TO_FUNC(env_global);
LINK_ENTITY_TO_FUNC(env_glow);
LINK_ENTITY_TO_FUNC(env_laser);
LINK_ENTITY_TO_FUNC(env_lightning);
LINK_ENTITY_TO_FUNC(env_message);
LINK_ENTITY_TO_FUNC(env_model);
LINK_ENTITY_TO_FUNC(env_particles);
LINK_ENTITY_TO_FUNC(env_particles_custom);
LINK_ENTITY_TO_FUNC(env_rain);
LINK_ENTITY_TO_FUNC(env_render);
LINK_ENTITY_TO_FUNC(env_rocketshooter);
LINK_ENTITY_TO_FUNC(env_rope);
LINK_ENTITY_TO_FUNC(env_shake);
LINK_ENTITY_TO_FUNC(env_shooter);
LINK_ENTITY_TO_FUNC(env_smoker);
LINK_ENTITY_TO_FUNC(env_snow);
LINK_ENTITY_TO_FUNC(env_sound);
LINK_ENTITY_TO_FUNC(env_spark);
LINK_ENTITY_TO_FUNC(env_sprite);
LINK_ENTITY_TO_FUNC(env_spritetrain);
LINK_ENTITY_TO_FUNC(env_state);
LINK_ENTITY_TO_FUNC(env_warpball);
LINK_ENTITY_TO_FUNC(fireanddie);
LINK_ENTITY_TO_FUNC(flashlight_beam);
LINK_ENTITY_TO_FUNC(func_bankteller);
LINK_ENTITY_TO_FUNC(func_bomb_target);
LINK_ENTITY_TO_FUNC(func_breakable);
LINK_ENTITY_TO_FUNC(func_button);
LINK_ENTITY_TO_FUNC(func_buyzone);
LINK_ENTITY_TO_FUNC(func_conveyor);
LINK_ENTITY_TO_FUNC(func_corruptsecurity);
LINK_ENTITY_TO_FUNC(func_door);
LINK_ENTITY_TO_FUNC(func_door_rotating);
LINK_ENTITY_TO_FUNC(func_escapezone);
LINK_ENTITY_TO_FUNC(func_friction);
LINK_ENTITY_TO_FUNC(func_grencatch);
LINK_ENTITY_TO_FUNC(func_guntarget);
LINK_ENTITY_TO_FUNC(func_healthcharger);
LINK_ENTITY_TO_FUNC(func_hostage_rescue);
LINK_ENTITY_TO_FUNC(func_illusionary);
LINK_ENTITY_TO_FUNC(func_ladder);
LINK_ENTITY_TO_FUNC(func_monsterclip);
LINK_ENTITY_TO_FUNC(func_mortar_field);
LINK_ENTITY_TO_FUNC(func_needs);
LINK_ENTITY_TO_FUNC(func_needs_boredom);
LINK_ENTITY_TO_FUNC(func_needs_dirty);
LINK_ENTITY_TO_FUNC(func_needs_drink);
LINK_ENTITY_TO_FUNC(func_needs_eat);
LINK_ENTITY_TO_FUNC(func_needs_fatigue);
LINK_ENTITY_TO_FUNC(func_needs_lonely);
LINK_ENTITY_TO_FUNC(func_needs_pee);
LINK_ENTITY_TO_FUNC(func_needs_poo);
LINK_ENTITY_TO_FUNC(func_nobuild);
LINK_ENTITY_TO_FUNC(func_nogrenades);
LINK_ENTITY_TO_FUNC(func_op4mortarcontroller);
LINK_ENTITY_TO_FUNC(func_pendulum);
LINK_ENTITY_TO_FUNC(func_plat);
LINK_ENTITY_TO_FUNC(func_platrot);
LINK_ENTITY_TO_FUNC(func_pushable);
LINK_ENTITY_TO_FUNC(func_rain);
LINK_ENTITY_TO_FUNC(func_recharge);
LINK_ENTITY_TO_FUNC(func_resource);
LINK_ENTITY_TO_FUNC(func_rot_button);
LINK_ENTITY_TO_FUNC(func_rotating);
LINK_ENTITY_TO_FUNC(func_securityzone);
LINK_ENTITY_TO_FUNC(func_seethrough);
LINK_ENTITY_TO_FUNC(func_seethroughdoor);
LINK_ENTITY_TO_FUNC(func_sickbay_bed);
LINK_ENTITY_TO_FUNC(func_sickbay_psych);
LINK_ENTITY_TO_FUNC(func_snow);
LINK_ENTITY_TO_FUNC(func_tank);
LINK_ENTITY_TO_FUNC(func_tank_of);
LINK_ENTITY_TO_FUNC(func_tankcontrols);
LINK_ENTITY_TO_FUNC(func_tankcontrols_of);
LINK_ENTITY_TO_FUNC(func_tanklaser);
LINK_ENTITY_TO_FUNC(func_tanklaser_of);
LINK_ENTITY_TO_FUNC(func_tanklaserrust);
LINK_ENTITY_TO_FUNC(func_tankmortar);
LINK_ENTITY_TO_FUNC(func_tankmortar_of);
LINK_ENTITY_TO_FUNC(func_tankrocket);
LINK_ENTITY_TO_FUNC(func_tankrocket_of);
LINK_ENTITY_TO_FUNC(func_trackautochange);
LINK_ENTITY_TO_FUNC(func_trackchange);
LINK_ENTITY_TO_FUNC(func_tracktrain);
LINK_ENTITY_TO_FUNC(func_train);
LINK_ENTITY_TO_FUNC(func_traincontrols);
LINK_ENTITY_TO_FUNC(func_vehicle);
LINK_ENTITY_TO_FUNC(func_vehiclecontrols);
LINK_ENTITY_TO_FUNC(func_vip_safetyzone);
LINK_ENTITY_TO_FUNC(func_wall);
LINK_ENTITY_TO_FUNC(func_wall_toggle);
LINK_ENTITY_TO_FUNC(func_wardrobe);
LINK_ENTITY_TO_FUNC(func_water);
LINK_ENTITY_TO_FUNC(func_weaponcheck);
LINK_ENTITY_TO_FUNC(func_weldable);
LINK_ENTITY_TO_FUNC(func_wind);
LINK_ENTITY_TO_FUNC(g_flTimeLimit);
LINK_ENTITY_TO_FUNC(game_counter);
LINK_ENTITY_TO_FUNC(game_counter_set);
LINK_ENTITY_TO_FUNC(game_end);
LINK_ENTITY_TO_FUNC(game_player_equip);
LINK_ENTITY_TO_FUNC(game_player_hurt);
LINK_ENTITY_TO_FUNC(game_player_team);
LINK_ENTITY_TO_FUNC(game_score);
LINK_ENTITY_TO_FUNC(game_team_master);
LINK_ENTITY_TO_FUNC(game_team_set);
LINK_ENTITY_TO_FUNC(game_text);
LINK_ENTITY_TO_FUNC(game_zone_player);
LINK_ENTITY_TO_FUNC(garg_stomp);
LINK_ENTITY_TO_FUNC(gauss_ball);
LINK_ENTITY_TO_FUNC(gauss_charged);
LINK_ENTITY_TO_FUNC(gauss_glow);
LINK_ENTITY_TO_FUNC(ghost);
LINK_ENTITY_TO_FUNC(gib_dropship);
LINK_ENTITY_TO_FUNC(gibshooter);
LINK_ENTITY_TO_FUNC(gonomeguts);
LINK_ENTITY_TO_FUNC(grapple_tip);
LINK_ENTITY_TO_FUNC(grenade);
LINK_ENTITY_TO_FUNC(grenade_mantabomb);
LINK_ENTITY_TO_FUNC(guass_bolt);
LINK_ENTITY_TO_FUNC(gunman_cycler);
LINK_ENTITY_TO_FUNC(gunner_mortar);
LINK_ENTITY_TO_FUNC(gunner_rocket);
LINK_ENTITY_TO_FUNC(hologram_beak);
LINK_ENTITY_TO_FUNC(hologram_damage);
LINK_ENTITY_TO_FUNC(hornet);
LINK_ENTITY_TO_FUNC(hostage_entity);
LINK_ENTITY_TO_FUNC(hvr_blkop_rocket);
LINK_ENTITY_TO_FUNC(hvr_rocket);
LINK_ENTITY_TO_FUNC(i_p_t);
LINK_ENTITY_TO_FUNC(i_t_g);
LINK_ENTITY_TO_FUNC(i_t_t);
LINK_ENTITY_TO_FUNC(info_alias);
LINK_ENTITY_TO_FUNC(info_areadef);
LINK_ENTITY_TO_FUNC(info_bigmomma);
LINK_ENTITY_TO_FUNC(info_bomb_target);
LINK_ENTITY_TO_FUNC(info_ctfdetect);
LINK_ENTITY_TO_FUNC(info_ctfspawn);
LINK_ENTITY_TO_FUNC(info_ctfspawn_powerup);
LINK_ENTITY_TO_FUNC(info_displacer_earth_target);
LINK_ENTITY_TO_FUNC(info_displacer_xen_target);
LINK_ENTITY_TO_FUNC(info_doddetect);
LINK_ENTITY_TO_FUNC(info_gameplay);
LINK_ENTITY_TO_FUNC(info_group);
LINK_ENTITY_TO_FUNC(info_hostage_rescue);
LINK_ENTITY_TO_FUNC(info_initial_player_allies);
LINK_ENTITY_TO_FUNC(info_initial_player_axis);
LINK_ENTITY_TO_FUNC(info_intermission);
LINK_ENTITY_TO_FUNC(info_join_autoassign);
LINK_ENTITY_TO_FUNC(info_join_team);
LINK_ENTITY_TO_FUNC(info_landmark);
LINK_ENTITY_TO_FUNC(info_location);
LINK_ENTITY_TO_FUNC(info_map_parameters);
LINK_ENTITY_TO_FUNC(info_mapinfo);
LINK_ENTITY_TO_FUNC(info_node);
LINK_ENTITY_TO_FUNC(info_node_air);
LINK_ENTITY_TO_FUNC(info_null);
LINK_ENTITY_TO_FUNC(info_pitworm);
LINK_ENTITY_TO_FUNC(info_pitworm_steam_lock);
LINK_ENTITY_TO_FUNC(info_player_allies);
LINK_ENTITY_TO_FUNC(info_player_axis);
LINK_ENTITY_TO_FUNC(info_player_deathmatch);
LINK_ENTITY_TO_FUNC(info_player_observer);
LINK_ENTITY_TO_FUNC(info_player_start);
LINK_ENTITY_TO_FUNC(info_player_teamspawn);
LINK_ENTITY_TO_FUNC(info_spectate);
LINK_ENTITY_TO_FUNC(info_target);
LINK_ENTITY_TO_FUNC(info_team_start);
LINK_ENTITY_TO_FUNC(info_teleport_destination);
LINK_ENTITY_TO_FUNC(info_tf_teamcheck);
LINK_ENTITY_TO_FUNC(info_tf_teamset);
LINK_ENTITY_TO_FUNC(info_tfdetect);
LINK_ENTITY_TO_FUNC(info_tfgoal);
LINK_ENTITY_TO_FUNC(info_tfgoal_timer);
LINK_ENTITY_TO_FUNC(info_vip_start);
LINK_ENTITY_TO_FUNC(infodecal);
LINK_ENTITY_TO_FUNC(item_airtank);
LINK_ENTITY_TO_FUNC(item_ammopack);
LINK_ENTITY_TO_FUNC(item_antidote);
LINK_ENTITY_TO_FUNC(item_armor);
LINK_ENTITY_TO_FUNC(item_armor1);
LINK_ENTITY_TO_FUNC(item_armor2);
LINK_ENTITY_TO_FUNC(item_armor3);
LINK_ENTITY_TO_FUNC(item_armorvest);
LINK_ENTITY_TO_FUNC(item_artifact_envirosuit);
LINK_ENTITY_TO_FUNC(item_artifact_invisibility);
LINK_ENTITY_TO_FUNC(item_artifact_invulnerability);
LINK_ENTITY_TO_FUNC(item_artifact_super_damage);
LINK_ENTITY_TO_FUNC(item_assaultsuit);
LINK_ENTITY_TO_FUNC(item_battery);
LINK_ENTITY_TO_FUNC(item_catalyst);
LINK_ENTITY_TO_FUNC(item_cells);
LINK_ENTITY_TO_FUNC(item_ctfaccelerator);
LINK_ENTITY_TO_FUNC(item_ctfbackpack);
LINK_ENTITY_TO_FUNC(item_ctfbase);
LINK_ENTITY_TO_FUNC(item_ctfflag);
LINK_ENTITY_TO_FUNC(item_ctflongjump);
LINK_ENTITY_TO_FUNC(item_ctfportablehev);
LINK_ENTITY_TO_FUNC(item_ctfregeneration);
LINK_ENTITY_TO_FUNC(item_gascan);
LINK_ENTITY_TO_FUNC(item_gastank);
LINK_ENTITY_TO_FUNC(item_generic);
LINK_ENTITY_TO_FUNC(item_genericammo);
LINK_ENTITY_TO_FUNC(item_health);
LINK_ENTITY_TO_FUNC(item_healthkit);
LINK_ENTITY_TO_FUNC(item_heavyarmor);
LINK_ENTITY_TO_FUNC(item_helmet);
LINK_ENTITY_TO_FUNC(item_jetpack);
LINK_ENTITY_TO_FUNC(item_kevlar);
LINK_ENTITY_TO_FUNC(item_longjump);
LINK_ENTITY_TO_FUNC(item_mine);
LINK_ENTITY_TO_FUNC(item_nuclearbomb);
LINK_ENTITY_TO_FUNC(item_nuclearbombbutton);
LINK_ENTITY_TO_FUNC(item_nuclearbombtimer);
LINK_ENTITY_TO_FUNC(item_rockets);
LINK_ENTITY_TO_FUNC(item_security);
LINK_ENTITY_TO_FUNC(item_shells);
LINK_ENTITY_TO_FUNC(item_sodacan);
LINK_ENTITY_TO_FUNC(item_spikes);
LINK_ENTITY_TO_FUNC(item_suit);
LINK_ENTITY_TO_FUNC(item_tfgoal);
LINK_ENTITY_TO_FUNC(item_thighpack);
LINK_ENTITY_TO_FUNC(item_vest);
LINK_ENTITY_TO_FUNC(jailer);
LINK_ENTITY_TO_FUNC(judge);
LINK_ENTITY_TO_FUNC(kwSpikeProjectile);
LINK_ENTITY_TO_FUNC(laser_spot);
LINK_ENTITY_TO_FUNC(lava);
LINK_ENTITY_TO_FUNC(lava_god);
LINK_ENTITY_TO_FUNC(leaf);
LINK_ENTITY_TO_FUNC(leaf_god);
LINK_ENTITY_TO_FUNC(light);
LINK_ENTITY_TO_FUNC(light_environment);
LINK_ENTITY_TO_FUNC(light_spot);
LINK_ENTITY_TO_FUNC(lightning_bug);
LINK_ENTITY_TO_FUNC(maggot_gib);
LINK_ENTITY_TO_FUNC(meteor_god);
LINK_ENTITY_TO_FUNC(meteor_target);
LINK_ENTITY_TO_FUNC(misterx);
LINK_ENTITY_TO_FUNC(momentary_door);
LINK_ENTITY_TO_FUNC(momentary_rot_button);
LINK_ENTITY_TO_FUNC(monster_ShockTrooper_dead);
LINK_ENTITY_TO_FUNC(monster_aigirl);
LINK_ENTITY_TO_FUNC(monster_alien_babyvoltigore);
LINK_ENTITY_TO_FUNC(monster_alien_controller);
LINK_ENTITY_TO_FUNC(monster_alien_grunt);
LINK_ENTITY_TO_FUNC(monster_alien_slave);
LINK_ENTITY_TO_FUNC(monster_alien_slave_dead);
LINK_ENTITY_TO_FUNC(monster_alien_voltigore);
LINK_ENTITY_TO_FUNC(monster_apache);
LINK_ENTITY_TO_FUNC(monster_assassin_repel);
LINK_ENTITY_TO_FUNC(monster_babycrab);
LINK_ENTITY_TO_FUNC(monster_barnacle);
LINK_ENTITY_TO_FUNC(monster_barney);
LINK_ENTITY_TO_FUNC(monster_barney_dead);
LINK_ENTITY_TO_FUNC(monster_beak);
LINK_ENTITY_TO_FUNC(monster_beakbirther);
LINK_ENTITY_TO_FUNC(monster_bigmomma);
LINK_ENTITY_TO_FUNC(monster_blkop_apache);
LINK_ENTITY_TO_FUNC(monster_blkop_osprey);
LINK_ENTITY_TO_FUNC(monster_bloater);
LINK_ENTITY_TO_FUNC(monster_bullchicken);
LINK_ENTITY_TO_FUNC(monster_butterfly);
LINK_ENTITY_TO_FUNC(monster_cine2_hvyweapons);
LINK_ENTITY_TO_FUNC(monster_cine2_scientist);
LINK_ENTITY_TO_FUNC(monster_cine2_slave);
LINK_ENTITY_TO_FUNC(monster_cine3_barney);
LINK_ENTITY_TO_FUNC(monster_cine3_scientist);
LINK_ENTITY_TO_FUNC(monster_cine_barney);
LINK_ENTITY_TO_FUNC(monster_cine_panther);
LINK_ENTITY_TO_FUNC(monster_cine_scientist);
LINK_ENTITY_TO_FUNC(monster_cleansuit_scientist);
LINK_ENTITY_TO_FUNC(monster_cleansuit_scientist_dead);
LINK_ENTITY_TO_FUNC(monster_cockroach);
LINK_ENTITY_TO_FUNC(monster_cricket);
LINK_ENTITY_TO_FUNC(monster_critter);
LINK_ENTITY_TO_FUNC(monster_dart);
LINK_ENTITY_TO_FUNC(monster_darttrap);
LINK_ENTITY_TO_FUNC(monster_dragonfly);
LINK_ENTITY_TO_FUNC(monster_drillsergeant);
LINK_ENTITY_TO_FUNC(monster_endboss);
LINK_ENTITY_TO_FUNC(monster_fgrunt_repel);
LINK_ENTITY_TO_FUNC(monster_flashlight);
LINK_ENTITY_TO_FUNC(monster_flyer);
LINK_ENTITY_TO_FUNC(monster_flyer_flock);
LINK_ENTITY_TO_FUNC(monster_furniture);
LINK_ENTITY_TO_FUNC(monster_gargantua);
LINK_ENTITY_TO_FUNC(monster_gator);
LINK_ENTITY_TO_FUNC(monster_generic);
LINK_ENTITY_TO_FUNC(monster_geneworm);
LINK_ENTITY_TO_FUNC(monster_gman);
LINK_ENTITY_TO_FUNC(monster_gonome);
LINK_ENTITY_TO_FUNC(monster_gonome_dead);
LINK_ENTITY_TO_FUNC(monster_grunt_ally_repel);
LINK_ENTITY_TO_FUNC(monster_grunt_repel);
LINK_ENTITY_TO_FUNC(monster_gunner);
LINK_ENTITY_TO_FUNC(monster_gunner_friendly);
LINK_ENTITY_TO_FUNC(monster_hatchetfish);
LINK_ENTITY_TO_FUNC(monster_headcrab);
LINK_ENTITY_TO_FUNC(monster_hevsuit_dead);
LINK_ENTITY_TO_FUNC(monster_hfgrunt_dead);
LINK_ENTITY_TO_FUNC(monster_hgrunt_dead);
LINK_ENTITY_TO_FUNC(monster_houndeye);
LINK_ENTITY_TO_FUNC(monster_houndeye_dead);
LINK_ENTITY_TO_FUNC(monster_human_assassin);
LINK_ENTITY_TO_FUNC(monster_human_bandit);
LINK_ENTITY_TO_FUNC(monster_human_chopper);
LINK_ENTITY_TO_FUNC(monster_human_demoman);
LINK_ENTITY_TO_FUNC(monster_human_friendly_grunt);
LINK_ENTITY_TO_FUNC(monster_human_grunt);
LINK_ENTITY_TO_FUNC(monster_human_grunt_ally);
LINK_ENTITY_TO_FUNC(monster_human_grunt_ally_dead);
LINK_ENTITY_TO_FUNC(monster_human_gunman);
LINK_ENTITY_TO_FUNC(monster_human_medic_ally);
LINK_ENTITY_TO_FUNC(monster_human_scientist);
LINK_ENTITY_TO_FUNC(monster_human_torch_ally);
LINK_ENTITY_TO_FUNC(monster_human_unarmed);
LINK_ENTITY_TO_FUNC(monster_ichthyosaur);
LINK_ENTITY_TO_FUNC(monster_largescorpion);
LINK_ENTITY_TO_FUNC(monster_leech);
LINK_ENTITY_TO_FUNC(monster_maggot);
LINK_ENTITY_TO_FUNC(monster_male_assassin);
LINK_ENTITY_TO_FUNC(monster_manta);
LINK_ENTITY_TO_FUNC(monster_massassin_dead);
LINK_ENTITY_TO_FUNC(monster_medic_ally_repel);
LINK_ENTITY_TO_FUNC(monster_microraptor);
LINK_ENTITY_TO_FUNC(monster_miniturret);
LINK_ENTITY_TO_FUNC(monster_mortar);
LINK_ENTITY_TO_FUNC(monster_nihilanth);
LINK_ENTITY_TO_FUNC(monster_op4loader);
LINK_ENTITY_TO_FUNC(monster_osprey);
LINK_ENTITY_TO_FUNC(monster_otis);
LINK_ENTITY_TO_FUNC(monster_otis_dead);
LINK_ENTITY_TO_FUNC(monster_ourano);
LINK_ENTITY_TO_FUNC(monster_penguin);
LINK_ENTITY_TO_FUNC(monster_pitdrone);
LINK_ENTITY_TO_FUNC(monster_pitworm);
LINK_ENTITY_TO_FUNC(monster_pitworm_up);
LINK_ENTITY_TO_FUNC(monster_raptor);
LINK_ENTITY_TO_FUNC(monster_rat);
LINK_ENTITY_TO_FUNC(monster_recruit);
LINK_ENTITY_TO_FUNC(monster_renesaur);
LINK_ENTITY_TO_FUNC(monster_rheptor);
LINK_ENTITY_TO_FUNC(monster_rosenberg);
LINK_ENTITY_TO_FUNC(monster_rustbit);
LINK_ENTITY_TO_FUNC(monster_rustbit_friendly);
LINK_ENTITY_TO_FUNC(monster_rustbot);
LINK_ENTITY_TO_FUNC(monster_rustbot_friendly);
LINK_ENTITY_TO_FUNC(monster_rustflier);
LINK_ENTITY_TO_FUNC(monster_rustgnr);
LINK_ENTITY_TO_FUNC(monster_rustgunr);
LINK_ENTITY_TO_FUNC(monster_satchel);
LINK_ENTITY_TO_FUNC(monster_scientist);
LINK_ENTITY_TO_FUNC(monster_scientist_dead);
LINK_ENTITY_TO_FUNC(monster_scorpion);
LINK_ENTITY_TO_FUNC(monster_sentry);
LINK_ENTITY_TO_FUNC(monster_sentry_mini);
LINK_ENTITY_TO_FUNC(monster_shockroach);
LINK_ENTITY_TO_FUNC(monster_shocktrooper);
LINK_ENTITY_TO_FUNC(monster_shocktrooper_repel);
LINK_ENTITY_TO_FUNC(monster_sitting_cleansuit_scientist);
LINK_ENTITY_TO_FUNC(monster_sitting_scientist);
LINK_ENTITY_TO_FUNC(monster_skeleton_dead);
LINK_ENTITY_TO_FUNC(monster_snark);
LINK_ENTITY_TO_FUNC(monster_tank);
LINK_ENTITY_TO_FUNC(monster_tank_rocket);
LINK_ENTITY_TO_FUNC(monster_targetrocket);
LINK_ENTITY_TO_FUNC(monster_tentacle);
LINK_ENTITY_TO_FUNC(monster_tentaclemaw);
LINK_ENTITY_TO_FUNC(monster_th_babykelly);
LINK_ENTITY_TO_FUNC(monster_th_boss);
LINK_ENTITY_TO_FUNC(monster_th_chicken);
LINK_ENTITY_TO_FUNC(monster_th_cyberfranklin);
LINK_ENTITY_TO_FUNC(monster_th_megasquid);
LINK_ENTITY_TO_FUNC(monster_th_zombiebull);
LINK_ENTITY_TO_FUNC(monster_torch_ally_repel);
LINK_ENTITY_TO_FUNC(monster_trainingbot);
LINK_ENTITY_TO_FUNC(monster_tripmine);
LINK_ENTITY_TO_FUNC(monster_tube);
LINK_ENTITY_TO_FUNC(monster_tube_embryo);
LINK_ENTITY_TO_FUNC(monster_tubequeen);
LINK_ENTITY_TO_FUNC(monster_turret);
LINK_ENTITY_TO_FUNC(monster_vortigaunt);
LINK_ENTITY_TO_FUNC(monster_xenome);
LINK_ENTITY_TO_FUNC(monster_xenome_embryo);
LINK_ENTITY_TO_FUNC(monster_zombie);
LINK_ENTITY_TO_FUNC(monster_zombie_barney);
LINK_ENTITY_TO_FUNC(monster_zombie_soldier);
LINK_ENTITY_TO_FUNC(monster_zombie_soldier_dead);
LINK_ENTITY_TO_FUNC(monstermaker);
LINK_ENTITY_TO_FUNC(mortar_shell);
LINK_ENTITY_TO_FUNC(movementchamber);
LINK_ENTITY_TO_FUNC(multi_alias);
LINK_ENTITY_TO_FUNC(multi_manager);
LINK_ENTITY_TO_FUNC(multi_watcher);
LINK_ENTITY_TO_FUNC(multisource);
LINK_ENTITY_TO_FUNC(nihilanth_energy_ball);
LINK_ENTITY_TO_FUNC(node_viewer);
LINK_ENTITY_TO_FUNC(node_viewer_fly);
LINK_ENTITY_TO_FUNC(node_viewer_human);
LINK_ENTITY_TO_FUNC(node_viewer_large);
LINK_ENTITY_TO_FUNC(nuke);
LINK_ENTITY_TO_FUNC(offensechamber);
LINK_ENTITY_TO_FUNC(op4mortar);
LINK_ENTITY_TO_FUNC(para_roundtimer);
LINK_ENTITY_TO_FUNC(particle_shooter);
LINK_ENTITY_TO_FUNC(path_corner);
LINK_ENTITY_TO_FUNC(path_track);
LINK_ENTITY_TO_FUNC(phasegate);
LINK_ENTITY_TO_FUNC(pitdronespike);
LINK_ENTITY_TO_FUNC(pitworm_gib);
LINK_ENTITY_TO_FUNC(pitworm_gibshooter);
LINK_ENTITY_TO_FUNC(player);
LINK_ENTITY_TO_FUNC(player_armor);
LINK_ENTITY_TO_FUNC(player_giveitems);
LINK_ENTITY_TO_FUNC(player_loadsaved);
LINK_ENTITY_TO_FUNC(player_speaker);
LINK_ENTITY_TO_FUNC(player_togglehud);
LINK_ENTITY_TO_FUNC(player_weaponstrip);
LINK_ENTITY_TO_FUNC(poison_injected);
LINK_ENTITY_TO_FUNC(random_speaker);
LINK_ENTITY_TO_FUNC(random_trigger);
LINK_ENTITY_TO_FUNC(resourcetower);
LINK_ENTITY_TO_FUNC(rope_sample);
LINK_ENTITY_TO_FUNC(rope_segment);
LINK_ENTITY_TO_FUNC(rpg_rocket);
LINK_ENTITY_TO_FUNC(rustbitshot);
LINK_ENTITY_TO_FUNC(sanity);
LINK_ENTITY_TO_FUNC(scan);
LINK_ENTITY_TO_FUNC(scripted_action);
LINK_ENTITY_TO_FUNC(scripted_sentence);
LINK_ENTITY_TO_FUNC(scripted_sequence);
LINK_ENTITY_TO_FUNC(securitymanager);
LINK_ENTITY_TO_FUNC(sensorychamber);
LINK_ENTITY_TO_FUNC(shell_bazooka);
LINK_ENTITY_TO_FUNC(shell_piat);
LINK_ENTITY_TO_FUNC(shell_pschreck);
LINK_ENTITY_TO_FUNC(shipAI_shipmate);
LINK_ENTITY_TO_FUNC(shock_beam);
LINK_ENTITY_TO_FUNC(siegeturret);
LINK_ENTITY_TO_FUNC(soundent);
LINK_ENTITY_TO_FUNC(spark_shower);
LINK_ENTITY_TO_FUNC(speaker);
LINK_ENTITY_TO_FUNC(sphere_explosion);
LINK_ENTITY_TO_FUNC(spitgunprojectile);
LINK_ENTITY_TO_FUNC(spore);
LINK_ENTITY_TO_FUNC(sporegunprojectile);
LINK_ENTITY_TO_FUNC(squidspit);
LINK_ENTITY_TO_FUNC(stompprojectile);
LINK_ENTITY_TO_FUNC(streak_spiral);
LINK_ENTITY_TO_FUNC(targ_speaker);
LINK_ENTITY_TO_FUNC(target_cdaudio);
LINK_ENTITY_TO_FUNC(target_mp3audio);
LINK_ENTITY_TO_FUNC(target_rocket);
LINK_ENTITY_TO_FUNC(team_advarmory);
LINK_ENTITY_TO_FUNC(team_armory);
LINK_ENTITY_TO_FUNC(team_armslab);
LINK_ENTITY_TO_FUNC(team_command);
LINK_ENTITY_TO_FUNC(team_hive);
LINK_ENTITY_TO_FUNC(team_infportal);
LINK_ENTITY_TO_FUNC(team_observatory);
LINK_ENTITY_TO_FUNC(team_prototypelab);
LINK_ENTITY_TO_FUNC(team_turretfactory);
LINK_ENTITY_TO_FUNC(team_webstrand);
LINK_ENTITY_TO_FUNC(teledeath);
LINK_ENTITY_TO_FUNC(test_effect);
LINK_ENTITY_TO_FUNC(test_entity2);
LINK_ENTITY_TO_FUNC(testhull);
LINK_ENTITY_TO_FUNC(tf_ammo_rpgclip);
LINK_ENTITY_TO_FUNC(tf_flame);
LINK_ENTITY_TO_FUNC(tf_flamethrower_burst);
LINK_ENTITY_TO_FUNC(tf_gl_grenade);
LINK_ENTITY_TO_FUNC(tf_ic_rocket);
LINK_ENTITY_TO_FUNC(tf_nailgun_nail);
LINK_ENTITY_TO_FUNC(tf_rpg_rocket);
LINK_ENTITY_TO_FUNC(tf_weapon_ac);
LINK_ENTITY_TO_FUNC(tf_weapon_autorifle);
LINK_ENTITY_TO_FUNC(tf_weapon_axe);
LINK_ENTITY_TO_FUNC(tf_weapon_caltrop);
LINK_ENTITY_TO_FUNC(tf_weapon_caltropgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_concussiongrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_empgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_flamethrower);
LINK_ENTITY_TO_FUNC(tf_weapon_gasgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_genericprimedgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_gl);
LINK_ENTITY_TO_FUNC(tf_weapon_ic);
LINK_ENTITY_TO_FUNC(tf_weapon_knife);
LINK_ENTITY_TO_FUNC(tf_weapon_medikit);
LINK_ENTITY_TO_FUNC(tf_weapon_mirvbomblet);
LINK_ENTITY_TO_FUNC(tf_weapon_mirvgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_nailgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_napalmgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_ng);
LINK_ENTITY_TO_FUNC(tf_weapon_normalgrenade);
LINK_ENTITY_TO_FUNC(tf_weapon_pl);
LINK_ENTITY_TO_FUNC(tf_weapon_railgun);
LINK_ENTITY_TO_FUNC(tf_weapon_rpg);
LINK_ENTITY_TO_FUNC(tf_weapon_shotgun);
LINK_ENTITY_TO_FUNC(tf_weapon_sniperrifle);
LINK_ENTITY_TO_FUNC(tf_weapon_spanner);
LINK_ENTITY_TO_FUNC(tf_weapon_superng);
LINK_ENTITY_TO_FUNC(tf_weapon_supershotgun);
LINK_ENTITY_TO_FUNC(tf_weapon_tranq);
LINK_ENTITY_TO_FUNC(timer);
LINK_ENTITY_TO_FUNC(tracer);
LINK_ENTITY_TO_FUNC(trigger);
LINK_ENTITY_TO_FUNC(trigger_auto);
LINK_ENTITY_TO_FUNC(trigger_autosave);
LINK_ENTITY_TO_FUNC(trigger_camera);
LINK_ENTITY_TO_FUNC(trigger_cdaudio);
LINK_ENTITY_TO_FUNC(trigger_changealias);
LINK_ENTITY_TO_FUNC(trigger_changelevel);
LINK_ENTITY_TO_FUNC(trigger_changetarget);
LINK_ENTITY_TO_FUNC(trigger_counter);
LINK_ENTITY_TO_FUNC(trigger_ctfgeneric);
LINK_ENTITY_TO_FUNC(trigger_endsection);
LINK_ENTITY_TO_FUNC(trigger_geneworm_hit);
LINK_ENTITY_TO_FUNC(trigger_gravity);
LINK_ENTITY_TO_FUNC(trigger_gunmanteleport);
LINK_ENTITY_TO_FUNC(trigger_hurt);
LINK_ENTITY_TO_FUNC(trigger_kill);
LINK_ENTITY_TO_FUNC(trigger_kill_nogib);
LINK_ENTITY_TO_FUNC(trigger_lightstyle);
LINK_ENTITY_TO_FUNC(trigger_monsterjump);
LINK_ENTITY_TO_FUNC(trigger_multiple);
LINK_ENTITY_TO_FUNC(trigger_multipleauto);
LINK_ENTITY_TO_FUNC(trigger_once);
LINK_ENTITY_TO_FUNC(trigger_playerfreeze);
LINK_ENTITY_TO_FUNC(trigger_presence);
LINK_ENTITY_TO_FUNC(trigger_push);
LINK_ENTITY_TO_FUNC(trigger_random);
LINK_ENTITY_TO_FUNC(trigger_relay);
LINK_ENTITY_TO_FUNC(trigger_script);
LINK_ENTITY_TO_FUNC(trigger_startpatrol);
LINK_ENTITY_TO_FUNC(trigger_tank);
LINK_ENTITY_TO_FUNC(trigger_tankeject);
LINK_ENTITY_TO_FUNC(trigger_tankoutofgas);
LINK_ENTITY_TO_FUNC(trigger_tankshell);
LINK_ENTITY_TO_FUNC(trigger_teleport);
LINK_ENTITY_TO_FUNC(trigger_transition);
LINK_ENTITY_TO_FUNC(trigger_xen_return);
LINK_ENTITY_TO_FUNC(tubequeen_mortar);
LINK_ENTITY_TO_FUNC(tuberocket);
LINK_ENTITY_TO_FUNC(turret);
LINK_ENTITY_TO_FUNC(umbracloud);
LINK_ENTITY_TO_FUNC(umbraprojectile);
LINK_ENTITY_TO_FUNC(vehicle_tank);
LINK_ENTITY_TO_FUNC(vehicle_tank_barrel);
LINK_ENTITY_TO_FUNC(vehicle_tank_body);
LINK_ENTITY_TO_FUNC(vehicle_tank_rocket);
LINK_ENTITY_TO_FUNC(vehicle_tank_shell);
LINK_ENTITY_TO_FUNC(vehicle_tank_turret);
LINK_ENTITY_TO_FUNC(watcher);
LINK_ENTITY_TO_FUNC(weapon_30cal);
LINK_ENTITY_TO_FUNC(weapon_357);
LINK_ENTITY_TO_FUNC(weapon_9mmAR);
LINK_ENTITY_TO_FUNC(weapon_9mmhandgun);
LINK_ENTITY_TO_FUNC(weapon_SPchemicalgun);
LINK_ENTITY_TO_FUNC(weapon_acidrocketgun);
LINK_ENTITY_TO_FUNC(weapon_aicore);
LINK_ENTITY_TO_FUNC(weapon_ak47);
LINK_ENTITY_TO_FUNC(weapon_amerknife);
LINK_ENTITY_TO_FUNC(weapon_aug);
LINK_ENTITY_TO_FUNC(weapon_awp);
LINK_ENTITY_TO_FUNC(weapon_bar);
LINK_ENTITY_TO_FUNC(weapon_bazooka);
LINK_ENTITY_TO_FUNC(weapon_beamgun);
LINK_ENTITY_TO_FUNC(weapon_bilebomb);
LINK_ENTITY_TO_FUNC(weapon_bilebombgun);
LINK_ENTITY_TO_FUNC(weapon_bite2gun);
LINK_ENTITY_TO_FUNC(weapon_bitegun);
LINK_ENTITY_TO_FUNC(weapon_blink);
LINK_ENTITY_TO_FUNC(weapon_bren);
LINK_ENTITY_TO_FUNC(weapon_c4);
LINK_ENTITY_TO_FUNC(weapon_charge);
LINK_ENTITY_TO_FUNC(weapon_chemgun);
LINK_ENTITY_TO_FUNC(weapon_claws);
LINK_ENTITY_TO_FUNC(weapon_colt);
LINK_ENTITY_TO_FUNC(weapon_crossbow);
LINK_ENTITY_TO_FUNC(weapon_crowbar);
LINK_ENTITY_TO_FUNC(weapon_deagle);
LINK_ENTITY_TO_FUNC(weapon_devour);
LINK_ENTITY_TO_FUNC(weapon_displacer);
LINK_ENTITY_TO_FUNC(weapon_divinewind);
LINK_ENTITY_TO_FUNC(weapon_dml);
LINK_ENTITY_TO_FUNC(weapon_dmlGrenade);
LINK_ENTITY_TO_FUNC(weapon_eagle);
LINK_ENTITY_TO_FUNC(weapon_egon);
LINK_ENTITY_TO_FUNC(weapon_einar1);
LINK_ENTITY_TO_FUNC(weapon_elite);
LINK_ENTITY_TO_FUNC(weapon_enfield);
LINK_ENTITY_TO_FUNC(weapon_famas);
LINK_ENTITY_TO_FUNC(weapon_fg42);
LINK_ENTITY_TO_FUNC(weapon_fists);
LINK_ENTITY_TO_FUNC(weapon_fiveseven);
LINK_ENTITY_TO_FUNC(weapon_flashbang);
LINK_ENTITY_TO_FUNC(weapon_g3sg1);
LINK_ENTITY_TO_FUNC(weapon_galil);
LINK_ENTITY_TO_FUNC(weapon_garand);
LINK_ENTITY_TO_FUNC(weapon_gauss);
LINK_ENTITY_TO_FUNC(weapon_gausspistol);
LINK_ENTITY_TO_FUNC(weapon_gerknife);
LINK_ENTITY_TO_FUNC(weapon_glock);
LINK_ENTITY_TO_FUNC(weapon_glock18);
LINK_ENTITY_TO_FUNC(weapon_grapple);
LINK_ENTITY_TO_FUNC(weapon_greasegun);
LINK_ENTITY_TO_FUNC(weapon_grenade);
LINK_ENTITY_TO_FUNC(weapon_grenadegun);
LINK_ENTITY_TO_FUNC(weapon_handgrenade);
LINK_ENTITY_TO_FUNC(weapon_handgrenade_ex);
LINK_ENTITY_TO_FUNC(weapon_healingspray);
LINK_ENTITY_TO_FUNC(weapon_heavymachinegun);
LINK_ENTITY_TO_FUNC(weapon_hegrenade);
LINK_ENTITY_TO_FUNC(weapon_hornetgun);
LINK_ENTITY_TO_FUNC(weapon_k43);
LINK_ENTITY_TO_FUNC(weapon_kar);
LINK_ENTITY_TO_FUNC(weapon_knife);
LINK_ENTITY_TO_FUNC(weapon_leap);
LINK_ENTITY_TO_FUNC(weapon_luger);
LINK_ENTITY_TO_FUNC(weapon_m1carbine);
LINK_ENTITY_TO_FUNC(weapon_m249);
LINK_ENTITY_TO_FUNC(weapon_m3);
LINK_ENTITY_TO_FUNC(weapon_m4a1);
LINK_ENTITY_TO_FUNC(weapon_mac10);
LINK_ENTITY_TO_FUNC(weapon_machinegun);
LINK_ENTITY_TO_FUNC(weapon_mechagun);
LINK_ENTITY_TO_FUNC(weapon_metabolize);
LINK_ENTITY_TO_FUNC(weapon_mg34);
LINK_ENTITY_TO_FUNC(weapon_mg42);
LINK_ENTITY_TO_FUNC(weapon_mine);
LINK_ENTITY_TO_FUNC(weapon_minigun);
LINK_ENTITY_TO_FUNC(weapon_mp40);
LINK_ENTITY_TO_FUNC(weapon_mp44);
LINK_ENTITY_TO_FUNC(weapon_mp5);
LINK_ENTITY_TO_FUNC(weapon_mp5navy);
LINK_ENTITY_TO_FUNC(weapon_mule);
LINK_ENTITY_TO_FUNC(weapon_p228);
LINK_ENTITY_TO_FUNC(weapon_p90);
LINK_ENTITY_TO_FUNC(weapon_parasite);
LINK_ENTITY_TO_FUNC(weapon_penguin);
LINK_ENTITY_TO_FUNC(weapon_piat);
LINK_ENTITY_TO_FUNC(weapon_pipewrench);
LINK_ENTITY_TO_FUNC(weapon_pistol);
LINK_ENTITY_TO_FUNC(weapon_polarisblade);
LINK_ENTITY_TO_FUNC(weapon_primalscream);
LINK_ENTITY_TO_FUNC(weapon_pschreck);
LINK_ENTITY_TO_FUNC(weapon_pushhands);
LINK_ENTITY_TO_FUNC(weapon_python);
LINK_ENTITY_TO_FUNC(weapon_rpg);
LINK_ENTITY_TO_FUNC(weapon_satchel);
LINK_ENTITY_TO_FUNC(weapon_scopedenfield);
LINK_ENTITY_TO_FUNC(weapon_scopedfg42);
LINK_ENTITY_TO_FUNC(weapon_scopedkar);
LINK_ENTITY_TO_FUNC(weapon_scout);
LINK_ENTITY_TO_FUNC(weapon_sg550);
LINK_ENTITY_TO_FUNC(weapon_sg552);
LINK_ENTITY_TO_FUNC(weapon_shield);
LINK_ENTITY_TO_FUNC(weapon_shockrifle);
LINK_ENTITY_TO_FUNC(weapon_shockroach);
LINK_ENTITY_TO_FUNC(weapon_shotgun);
LINK_ENTITY_TO_FUNC(weapon_smokegrenade);
LINK_ENTITY_TO_FUNC(weapon_snark);
LINK_ENTITY_TO_FUNC(weapon_sniperrifle);
LINK_ENTITY_TO_FUNC(weapon_spade);
LINK_ENTITY_TO_FUNC(weapon_spikegun);
LINK_ENTITY_TO_FUNC(weapon_spit);
LINK_ENTITY_TO_FUNC(weapon_spore);
LINK_ENTITY_TO_FUNC(weapon_sporelauncher);
LINK_ENTITY_TO_FUNC(weapon_spring);
LINK_ENTITY_TO_FUNC(weapon_sten);
LINK_ENTITY_TO_FUNC(weapon_stickgrenade);
LINK_ENTITY_TO_FUNC(weapon_stickgrenade_ex);
LINK_ENTITY_TO_FUNC(weapon_stomp);
LINK_ENTITY_TO_FUNC(weapon_swipe);
LINK_ENTITY_TO_FUNC(weapon_syringe);
LINK_ENTITY_TO_FUNC(weapon_th_ap9);
LINK_ENTITY_TO_FUNC(weapon_th_chaingun);
LINK_ENTITY_TO_FUNC(weapon_th_medkit);
LINK_ENTITY_TO_FUNC(weapon_th_shovel);
LINK_ENTITY_TO_FUNC(weapon_th_sniper);
LINK_ENTITY_TO_FUNC(weapon_th_spanner);
LINK_ENTITY_TO_FUNC(weapon_th_taurus);
LINK_ENTITY_TO_FUNC(weapon_thompson);
LINK_ENTITY_TO_FUNC(weapon_tmp);
LINK_ENTITY_TO_FUNC(weapon_tripmine);
LINK_ENTITY_TO_FUNC(weapon_umbra);
LINK_ENTITY_TO_FUNC(weapon_umbrella);
LINK_ENTITY_TO_FUNC(weapon_ump45);
LINK_ENTITY_TO_FUNC(weapon_usp);
LINK_ENTITY_TO_FUNC(weapon_webley);
LINK_ENTITY_TO_FUNC(weapon_webspinner);
LINK_ENTITY_TO_FUNC(weapon_welder);
LINK_ENTITY_TO_FUNC(weapon_xm1014);
LINK_ENTITY_TO_FUNC(weaponbox);
LINK_ENTITY_TO_FUNC(webgunprojectile);
LINK_ENTITY_TO_FUNC(world_items);
LINK_ENTITY_TO_FUNC(worldspawn);
LINK_ENTITY_TO_FUNC(xen_hair);
LINK_ENTITY_TO_FUNC(xen_hull);
LINK_ENTITY_TO_FUNC(xen_plantlight);
LINK_ENTITY_TO_FUNC(xen_spore_large);
LINK_ENTITY_TO_FUNC(xen_spore_medium);
LINK_ENTITY_TO_FUNC(xen_spore_small);
LINK_ENTITY_TO_FUNC(xen_tree);
LINK_ENTITY_TO_FUNC(xen_ttrigger);
LINK_ENTITY_TO_FUNC(xenomeshot);

