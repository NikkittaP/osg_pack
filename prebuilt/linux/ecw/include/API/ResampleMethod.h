/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: ResampleMethod.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Resample method header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_RESAMPLE_METHOD_H
#define NCS_API_RESAMPLE_METHOD_H

/**
 *	@enum NCSResampleMethod
 *	@ingroup enums
 *	@brief Enumerated type to describe the values of possible resampling/supersampling when reading files.
 */
typedef enum { 
	/** Nearest neighbour interpolation */
	NCS_RESAMPLE_NEAREST_NEIGHBOUR_INTERPOLATION = 0,
	/** Bilinear interpolation */
	NCS_RESAMPLE_BILINEAR_INTERPOLATION = 1
} NCSResampleMethod;

#endif // NCS_API_RESAMPLE_METHOD_H