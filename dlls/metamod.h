//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: MetaMod code
//
//=============================================================================

void METAMOD_RETURN( META_RES result );

template <class T> T METAMOD_RETURN_VALUE( META_RES result, T value )
{
	RETURN_META_VALUE(result, value);
}