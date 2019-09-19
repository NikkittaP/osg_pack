/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: CellSizeUnitType.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Cell size unit type header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_CELL_SIZE_UNIT_TYPE_H
#define NCS_API_CELL_SIZE_UNIT_TYPE_H

/** 
 *	@enum NCSCellSizeUnitType
 *	@ingroup enums
 *  Enumerated type for the cell sizes supported by the SDK.  These values are informational
 *  and do not affect image decoding when specifying world world coordinates
 */
typedef enum {
	/** Invalid cell units */
	ECW_CELL_UNITS_INVALID	=	0,
	/** Cell units are standard meters */
	ECW_CELL_UNITS_METERS	=	1,
	/** Degrees */
	ECW_CELL_UNITS_DEGREES	=	2,
	/** US Survey feet */
	ECW_CELL_UNITS_FEET		=	3,
	/** Unknown cell units */
	ECW_CELL_UNITS_UNKNOWN	=	4
} NCSCellSizeUnitType;

#endif // NCS_API_CELL_SIZE_UNIT_TYPE_H