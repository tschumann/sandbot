//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: Wrappers for MetaMod macros so that we have functions.
// Todo: What was this fixing? Get rid of this?
//
//=============================================================================

template <class T> T METAMOD_RETURN_VALUE( META_RES result, T value )
{
	RETURN_META_VALUE(result, value);
}