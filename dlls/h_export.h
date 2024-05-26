//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: engine hooking code
//
//=============================================================================

#ifndef _H_EXPORT_H_
#define _H_EXPORT_H_

#include "extdll.h"

#include "meta_api.h"

extern globalvars_t* gpGlobals;

extern gamedll_funcs_t gGameDLLFunc;

#ifndef __linux__
extern HINSTANCE h_Library;
#else
extern void *h_Library;
#endif

#ifndef __linux__
extern void WINAPI GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals);
#else
extern "C" void GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals );
#endif

#endif // _H_EXPORT_H_

