/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: FileColorSpace.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: File color space header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_COLORSPACE_H
#define NCS_COLORSPACE_H


/** 
 *	@enum NCSFileColorSpace
 *  @ingroup enums
 *	The color space used by a compressed file.
 *	For compatibility with ECW, these values cannot be changed or reordered.
 */
typedef enum {
	/** No color space */
	NCSCS_NONE						= 0,
	/** Greyscale image */
	NCSCS_GREYSCALE					= 1,	// Greyscale
	/** Luminance-chrominance color space */
	NCSCS_YUV						= 2,	// YUV - JPEG Digital, JP2 ICT
	/** Multiband image */
	NCSCS_MULTIBAND					= 3,	// Multi-band imagery
	/** sRGB color space */
	NCSCS_sRGB						= 4,	// sRGB
	/** Modified luminance-chrominance color space */
	NCSCS_YCbCr						= 5		// YCbCr - JP2 ONLY, Auto-converted to sRGB
} NCSFileColorSpace;

#endif // NCS_COLORSPACE_H