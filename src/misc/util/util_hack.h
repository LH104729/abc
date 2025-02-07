/**CFile****************************************************************

  FileName    [util_hack.h]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [This file is used to simulate the presence of "util.h".]

  Synopsis    [External declarations.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: util_hack.h,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#ifndef ABC__misc__util__util_hack_h
#define ABC__misc__util__util_hack_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "abc_global.h"

ABC_NAMESPACE_HEADER_START

#define NIL(type)           ((type *) 0)

#define util_cpu_time       Extra_CpuTime            
#define getSoftDataLimit    Extra_GetSoftDataLimit   
#define MMoutOfMemory       Extra_UtilMMoutOfMemory      

extern abctime              Extra_CpuTime();
extern int                  Extra_GetSoftDataLimit();
extern void               (*Extra_UtilMMoutOfMemory)( unsigned long size );


/**
 * @def PRIszt
 * @brief Format string for a size_t value.
 */
#if defined(_WIN32) && !defined(__USE_MINGW_ANSI_STDIO)
#ifndef PRIuPTR
#define PRIuPTR "Iu"
#endif
#ifndef PRIxPTR
#define PRIxPTR "Ix"
#endif
#ifndef PRIiPTR
#define PRIiPTR "Id"
#endif
#define PRIszt "Iu"
#else
#define PRIszt "zu"
#endif

ABC_NAMESPACE_HEADER_END

#endif
