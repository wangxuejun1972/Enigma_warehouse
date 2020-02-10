#ifndef _H_MEMDEBUG_H__
#define _H_MEMDEBUG_H__

#ifdef _DEBUG

#include "memtracer.h"

#define  new  new(__FILE__,__LINE__)

#endif //!_DEBUG

#endif //!_H_MEMDEBUG_H__
