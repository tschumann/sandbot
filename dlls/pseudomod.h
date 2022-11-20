//=============================================================================
//
// Pseudo Mod - replacement for MetaMod
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes:
//
//=============================================================================

enum FUNCTION_USE
{
	IGNORE = 1,
	CALL_AND_OVERRIDE = 3,
	SKIP_AND_OVERRIDE = 4
};

struct pseudo_state
{
	FUNCTION_USE result;
};

extern pseudo_state *gGlobalState;

inline void PSEUDO_RETURN(FUNCTION_USE result)
{
	gGlobalState->result = result;
}