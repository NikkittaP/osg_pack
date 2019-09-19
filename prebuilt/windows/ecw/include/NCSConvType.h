/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSConvType.h 
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSConvType header (cell type conversion routine)
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSCONVTYPE_H
#define NCSCONVTYPE_H

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif
#include "API/CellType.h"

/**
 * <summary>Convert buffer from one type to another.</summary>
 *
 * <remarks> Intergraph Perth, 07 Jan YYYY.</remarks>
 *
 * <param name="pDst">		    [in,out] destination buffer.</param>
 * <param name="eDstType">	    Type of the destination.</param>
 * <param name="nDstStep">	    Destination step.</param>
 * <param name="pSrc">		    [in,out] Source buffer.</param>
 * <param name="eSrcType">	    Type of the source.</param>
 * <param name="nSrcStep">	    Source step.</param>
 * <param name="nWidth">	    The width.</param>
 * <param name="bCanDoScaling"> (optional) the can do scaling.</param>
 * <param name="nScaleRatio">   (optional) the scale ratio.</param>
 *
 * <returns> true if it succeeds, false if it fails.</returns>
 */
extern "C" bool NCSConvType(void *pDst, NCSCellType eDstType, UINT8 nDstStep, void *pSrc, NCSCellType eSrcType, UINT8 nSrcStep, UINT32 nWidth, bool bCanDoScaling = false, UINT8 nScaleRatio = 2);

/**
 * <summary>Get NCSCellType size in bytes</summary>
 *
 * <param name="eCellType"> The cell type.</param>
 *
 * <returns>INT32 size in bytes</returns>
 */
extern "C" INT32 NCSCellSize(NCSCellType eCellType);

#endif // !NCSJPCBuffer2D_H
