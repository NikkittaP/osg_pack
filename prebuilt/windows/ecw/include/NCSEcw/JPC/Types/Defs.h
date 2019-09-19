/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Defs.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSJPC Defines
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCDEFS_H
#define NCSJPCDEFS_H

//
// Disable dll-interface warning, compiler gives it on protected/private members.
// Disable truncated name mangling warning
//

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif // NCSDEFS_H

//
// Use LCMS for ICC->RGB conversions, supports both
// restricted and full ICC profiles.
//
#define NCSJPC_USE_LCMS

//
// Use TinyXML for XML DOM Parsing
//
#define NCSJPC_USE_TINYXML

//
// Include ECW Compression in lib
//
#define ECW_COMPRESS

#define NCSJPC_2POW11 2048

#define NCSJP2_STRIP_HEIGHT 64
#define NCSJP2_MAX_STRIP_HEIGHT 512

#define NCSECW_GLOBAL_LOCK_NAME "NCSEcw GlobalLock"

// added on 22/08/2012
#define NCSJPC_INIT_VALUE (unsigned)0xffffffff	// 4,294,967,295
#define NCSJPC_BUSY_VALUE (unsigned)0xfffffffe	// 4,294,967,294
#define NCSJPC_DONE_VALUE (unsigned)0xfffffffd	// 4,294,967,293

#endif // NCSJPCDEFS_H
