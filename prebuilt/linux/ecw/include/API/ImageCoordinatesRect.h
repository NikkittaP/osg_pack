/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: ImageCoordinatesRect.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Image coordinates rect header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_IMAGE_COORDINATES_RECT_H
#define NCS_API_IMAGE_COORDINATES_RECT_H

 /** 
 *	@struct NCSImageCoordinatesRect
 *	@ingroup structures
 *	@brief Struct containing image coordinates of a dataset. 
 *
 *			These coordinates are inclusive so if your dataset is 1000px wide, the valid 
 *			range is 0-999
 */
struct NCSImageCoordinatesRect {
	/** top left X */
	unsigned int TLX;
	/** top left Y */
	unsigned int TLY;
	/** bottom right X */
    unsigned int BRX;
	/** bottom right Y */
    unsigned int BRY;
};

#endif // NCS_API_IMAGE_COORDINATES_RECT_H