//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: game rules code
//
//=============================================================================

#include "game_halflife.h"

ValveGame::ValveGame( GameId gameId ) : Game( gameId )
{
}

void ValveGame::GetSaveGameComment( char *pBuffer, int iMaxLength ) const
{
	// TODO: does GameUI.dll localise these names?
	if( !strncmp( STRING(gpGlobals->mapname), "t0a0", strlen( "t0a0" ) ) ) // a, b, b1, b2, c, d
	{
		strncpy( pBuffer, "HAZARD COURSE", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c0a0", strlen( "c0a0" ) ) ) // a, b, c, d, e
	{
		strncpy( pBuffer, "BLACK MESA INBOUND", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a0" ) )
	{
		strncpy( pBuffer, "ANOMOLOUS MATERIALS", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a0a" ) )
	{
		strncpy( pBuffer, "ANOMOLOUS MATERIALS", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a0b" ) )
	{
		strncpy( pBuffer, "ANOMOLOUS MATERIALS", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a0c" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a0d" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a0e" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a1" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a1a" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a1b" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a1c" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a1d" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c1a1f" ) )
	{
		strncpy( pBuffer, "UNFORSEEN CONSEQUENCES", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c1a2", strlen( "c1a2" ) ) ) // a, b, c, d
	{
		strncpy( pBuffer, "OFFICE COMPLEX", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c1a3", strlen( "c1a3" ) ) ) // a, b, c, d
	{
		strncpy( pBuffer, "\"WE'VE GOT HOSTILES\"", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c1a4", strlen( "c1a4" ) ) ) // b, d, e, f, g, i, j, k
	{
		strncpy( pBuffer, "BLAST PIT", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c2a1", strlen( "c2a1" ) ) ) // a, b
	{
		strncpy( pBuffer, "POWER UP", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c2a2" , strlen( "c2a2" ) ) ) // a, b1, b2, c, d, e, f, g, h
	{
		strncpy( pBuffer, "ON A RAIL", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a3" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a3a" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a3b" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a3c" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a3d" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a3e" ) )
	{
		strncpy( pBuffer, "APPREHENSION", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4a" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4b" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4c" ) )
	{
		strncpy( pBuffer, "RESIDUE PROCESSING", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4d" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4e" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4f" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c2a4g" ) )
	{
		strncpy( pBuffer, "QUESTIONABLE ETHICS", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c2a5", strlen( "c2a5" ) ) ) // a, b, c, d, e, f, g, w, x
	{
		strncpy( pBuffer, "SURFACE TENSION", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a1" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a1a" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a1b" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a2" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a2a" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a2b" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a2c" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a2d" ) )
	{
		strncpy( pBuffer, "\"FORGET ABOUT FREEMAN\"", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a2e" ) )
	{
		strncpy( pBuffer, "LAMBDA CORE", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c3a2f" ) )
	{
		strncpy( pBuffer, "LAMBDA CORE", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a1" ) )
	{
		strncpy( pBuffer, "XEN", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a1a" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a1b" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a1c" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a1d" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a1e" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a1f" ) )
	{
		strncpy( pBuffer, "INTERLOPER", iMaxLength );
	}
	else if( !strncmp( STRING(gpGlobals->mapname), "c4a2", strlen( "c4a2" ) ) ) // a, b
	{
		strncpy( pBuffer, "GONARCH'S LAIR", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c4a3" ) )
	{
		strncpy( pBuffer, "NIHILANTH", iMaxLength );
	}
	else if( !strcmp( STRING(gpGlobals->mapname), "c5a1" ) )
	{
		strncpy( pBuffer, "NIHILANTH", iMaxLength );
	}
	else
	{
		strncpy( pBuffer, STRING(gpGlobals->mapname), iMaxLength );
	}
}