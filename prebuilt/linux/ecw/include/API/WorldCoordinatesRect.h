/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: WorldCoordinatesRect.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: World coordinates rect header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_WORLD_COORDINATES_RECT_H
#define NCS_API_WORLD_COORDINATES_RECT_H

/** 
 *	@struct NCSWorldCoordinatesRect
 *	@ingroup structures
 *	@brief Struct containing world coordinates of a dataset. 
 *
 *			These coordinates do not need to be aligned to pixels and the 
 *			entire region can be smaller than one pixel.
 */
struct NCSWorldCoordinatesRect {
	/** top left X */
	double TLX;
	/** top left Y */
	double TLY;
	/** bottom right X */
    double BRX;
	/** bottom right Y */
    double BRY;
};

#endif // NCS_API_WORLD_COORDINATES_RECT_H