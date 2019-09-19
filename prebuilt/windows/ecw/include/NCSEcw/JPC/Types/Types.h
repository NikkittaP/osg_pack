/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Types.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSJPC additional Types header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCTYPES_H
#define NCSJPCTYPES_H

#include "NCSDefs.h"

#ifndef NCSJPCDEFS_H
#include "NCSEcw/JPC/Types/Defs.h"
#endif // NCSJPCDEFS_H

#ifndef NCSMALLOC_H
#include "NCSMalloc.h"
#endif // NCSMALLOC_H
#ifndef NCSERROR_H
#include "NCSError.h"
#endif // NCSERROR_H

// STL headers we need
#include <vector>
#include <map>
#include <list>
#include <memory>

// stdarg header.
#include <stdarg.h>

#ifdef SOLARIS
#include <sys/varargs.h>
#endif // POSIX

#endif // NCSJPCTYPES_H

