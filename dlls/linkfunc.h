//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// linkfunc.h
//

#ifndef LINKFUNC_H
#define LINKFUNC_H

#include <memory>

#include "archtypes.h"

#ifdef WIN32
extern std::unique_ptr<WORD[]> pOrdinals;
extern std::unique_ptr<DWORD[]> pFunctionAddresses;
extern std::unique_ptr<DWORD[]> pNameAddresses;
#endif // WIN32

void LoadExtraExports();

uint32 NameToAddress( const char *pName );
const char *AddressToName(uint32 function);

#endif // LINKFUNC_H
