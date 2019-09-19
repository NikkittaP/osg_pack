/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: CellType.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Cell type header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_CELL_TYPE_H
#define NCS_API_CELL_TYPE_H

/**
 *	@enum NCSGeodataUsage
 *	@ingroup enums
 *	@brief Enumerated type to describe different kinds of cell types.
 */
typedef enum {						
	NCSCT_UINT8				=	0,	
	NCSCT_UINT16			=	1,	
	NCSCT_UINT32			=	2,	
	NCSCT_UINT64			=	3,	
	NCSCT_INT8				=	4,	
	NCSCT_INT16				=	5,	
	NCSCT_INT32				=	6,	
	NCSCT_INT64				=	7,	
	NCSCT_IEEE4				=	8,	
	NCSCT_IEEE8				=	9,

	NCSCT_NUMVALUES			=   10
} NCSCellType;

#endif // NCS_API_CELL_TYPE_H