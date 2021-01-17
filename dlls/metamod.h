//
// Sandbot
//
// (http://www.teamsandpit.com/)
//

void METAMOD_RETURN( META_RES result );

template <class T> T METAMOD_RETURN_VALUE( META_RES result, T value )
{
	RETURN_META_VALUE(result, value);
}