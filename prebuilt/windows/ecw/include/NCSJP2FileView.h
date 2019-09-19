/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	NCSJP2FileView.h
** CREATED:	04 May 2007
** AUTHOR: 	Simon Cope
** PURPOSE:	define for compatibility with old CFileView class
** EDITS:
** [01] sjc	04May07 Created file
** 
** NOTES
**
********************************************************/

#ifndef NCSJP2FILEVIEW_H
#define NCSJP2FILEVIEW_H

#include "NCSEcw/API/View.h"

#ifdef __cplusplus
//extern "C" {
#endif

#ifndef NCS_NO_COMPAT_NAMES
/*
// Pull in full CNCSJP2File definition as well
#pragma warning(push)
#pragma warning(disable: 4251)
#include "NCSEcw/JP2/File.h"
#pragma warning(pop)
*/
typedef NCS::CView CNCSJP2FileView;
#endif

#ifdef __cplusplus
//};
#endif

#endif	// NCSJP2FILEVIEW_H
