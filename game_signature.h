#ifndef _INCLUDE_L4D1_SIGNATURE_LINUX_
#define _INCLUDE_L4D1_SIGNATURE_LINUX_

#ifdef WIN32
#ifdef L4D1
#include "l4d1_signature_win32.h"
#else
#include "l4d2_signature_win32.h"
#endif
#else
#ifdef L4D1
#include "l4d1_signature_linux.h"
#else
#include "l4d2_signature_linux.h"
#endif
#endif

#endif //_INCLUDE_L4D1_SIGNATURE_LINUX_
