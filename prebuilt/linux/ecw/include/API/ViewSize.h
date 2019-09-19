/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: ViewSize.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: View size header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_VIEW_SIZE_H
#define NCS_API_VIEW_SIZE_H

/** 
 *	@struct NCSViewSize
 *	@ingroup structures
 *	@brief Struct containing a requested view size.
 */
struct NCSViewSize {
	/** Width in pixels */
	unsigned int Width;
	/** Height in pixels */
	unsigned int Height;
};

#endif // NCS_API_VIEW_SIZE_H