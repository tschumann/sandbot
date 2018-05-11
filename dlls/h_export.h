#ifndef _H_EXPORT_H_
#define _H_EXPORT_H_

#include "extdll.h"

#ifndef __linux__
extern HINSTANCE h_Library;
#else
extern void *h_Library;
#endif

#endif // _H_EXPORT_H_

