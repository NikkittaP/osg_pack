/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: ProjectionFormat.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Projection format header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_PROJECTION_FORMAT_H
#define NCS_API_PROJECTION_FORMAT_H

/** 
 *	@enum NCSProjectionFormat
 *  @ingroup enums
 *  Enumerated type for the format of projection/datum pair
 *  This only works for ecw file which has version 3 or above
 */
typedef enum {
	/* Return projection information in "DATUM:EPSG:XXXXX, PROJECTION:ESPG:XXXXX" format */
	NCS_PROJECTION_EPSG_FORMAT = 0,
	/* Return projection information in old format such as "DATUM:WGS84, PROJECTION:GEODETIC"*/
	NCS_PROJECTION_ERMAPPER_FORMAT = 1
} NCSProjectionFormat;

#endif // NCS_API_PROJECTION_FORMAT_H