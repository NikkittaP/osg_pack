/*
 * This header defines a slightly modified public type and utility definition inherited from libgeotiff library.
 * It includes the content from geo_tiffp.h, geotiff.h, xtiffio.h
 */


#ifndef _GEO_PUBLIC_TYPES_H_
#define _GEO_PUBLIC_TYPES_H_

typedef unsigned short pinfo_t;    /* SHORT ProjectionInfo tag type */

#include "geokeys.h"
/* 
 *  Define public Tag names and values here 
 */

/* tags 33550 is a private tag registered to SoftDesk, Inc */
#define TIFFTAG_GEOPIXELSCALE       33550
/* tags 33920-33921 are private tags registered to Intergraph, Inc */
#define TIFFTAG_INTERGRAPH_MATRIX    33920   /* $use TIFFTAG_GEOTRANSMATRIX ! */
#define TIFFTAG_GEOTIEPOINTS         33922
/* tags 34263-34264 are private tags registered to NASA-JPL Carto Group */
#ifdef JPL_TAG_SUPPORT
#define TIFFTAG_JPL_CARTO_IFD        34263    /* $use GeoProjectionInfo ! */
#endif
#define TIFFTAG_GEOTRANSMATRIX       34264    /* New Matrix Tag replaces 33920 */
/* tags 34735-3438 are private tags registered to SPOT Image, Inc */
#define TIFFTAG_GEOKEYDIRECTORY      34735
#define TIFFTAG_GEODOUBLEPARAMS      34736
#define TIFFTAG_GEOASCIIPARAMS       34737


#define GTIFF_GEOKEYDIRECTORY   TIFFTAG_GEOKEYDIRECTORY /* from xtiffio.h */
#define GTIFF_DOUBLEPARAMS      TIFFTAG_GEODOUBLEPARAMS
#define GTIFF_ASCIIPARAMS       TIFFTAG_GEOASCIIPARAMS
#define GTIFF_PIXELSCALE        TIFFTAG_GEOPIXELSCALE
#define GTIFF_TRANSMATRIX       TIFFTAG_GEOTRANSMATRIX
//#define GTIFF_INTERGRAPH_MATRIX TIFFTAG_INTERGRAPH_MATRIX 
#define GTIFF_TIEPOINTS         TIFFTAG_GEOTIEPOINTS
#define GTIFF_LOCAL          0

typedef enum {
   TYPE_BYTE=1,
   TYPE_SHORT=2,
   TYPE_LONG=3,
   TYPE_RATIONAL=4,
   TYPE_ASCII=5,
   TYPE_FLOAT=6,
   TYPE_DOUBLE=7,
   TYPE_SBYTE=8,
   TYPE_SSHORT=9,
   TYPE_SLONG=10,
   TYPE_UNKNOWN=11
} tagtype_t;

/**********************************************************************
 * Do we want to build as a DLL on windows?
 **********************************************************************/
#if !defined(CPL_DLL)
#  if defined(_WIN32) && defined(BUILD_AS_DLL)
#    define CPL_DLL     __declspec(dllexport)
#  else
#    define CPL_DLL
#  endif
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/* name, code conversion utilities */
char  CPL_DLL *NCS_GTIFKeyName(geokey_t key);
char  CPL_DLL *NCS_GTIFValueName(geokey_t key,int value);
char  CPL_DLL *NCS_GTIFTypeName(tagtype_t type);
char  CPL_DLL *NCS_GTIFTagName(int tag);
int   CPL_DLL  NCS_GTIFKeyCode(char * key);
int   CPL_DLL  NCS_GTIFValueCode(geokey_t key,char *value);
int   CPL_DLL  NCS_GTIFTypeCode(char *type);
int   CPL_DLL  NCS_GTIFTagCode(char *tag);

#if defined(__cplusplus)
}
#endif

#endif