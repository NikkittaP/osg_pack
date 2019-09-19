/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Reader.h 
** CREATED:  10/01/2006 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CReader class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJP2READER_H
#define NCSJP2READER_H

#ifndef NCSFILE_H
#include "NCSEcw/API/View.h"
#endif // NCSFILE_H

#include "NCSEcw/JP2/File.h"

namespace NCS {
namespace JPC {
	class CResampler;
}
namespace JP2 {
/**
 * CReader class - the JP2 file view class.
 * 
 * @author       Simon Cope
 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/12/03 $ 
 */
class NCSECW_IMPEX CReader: public NCS::CView::CReaderBase {
public:
		/** Default constructor, initialises members */
	explicit CReader(NCS::CView *pView);
		/** Virtual Destructor */
	virtual ~CReader();

		/** 
		 * Set a view into the ECW file for reading.
		 * @param		nBands			Number of bands in pBandList to read
		 * @param		pBandList		Array of band indices to read.
		 * @param		nWidth			Width of the view in pixels
		 * @param		nHeight			Height of the view in pixels
		 * @param		nDatasetTLX		Top left X dataset coordinate of view
		 * @param		nDatasetTLY		Top left Y dataset coordinate of view
		 * @param		nDatasetBRX		Bottom right X dataset coordinate of view
		 * @param		nDatasetBRY		Bottom right Y dataset voordinate of view
		 * @param		dWorldTLX		Top left X world coordinate of view (informative only)
		 * @param		dWorldTLY		Top left Y world coordinate of view (informative only)
		 * @param		dWorldBRX		Bottom right X world coordinate of view (informative only)
		 * @param		dWorldBRY		Bottom right Y world coordinate of view (informative only)
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError SetView(UINT32 nBands, UINT32 *pBandList, 
					   		  UINT32 nDatasetTLX, UINT32 nDatasetTLY,
							  UINT32 nDatasetBRX, UINT32 nDatasetBRY,
							  UINT32 nWidth, UINT32 nHeight,
							  IEEE8 dWorldTLX = 0.0, IEEE8 dWorldTLY = 0.0,
							  IEEE8 dWorldBRX = 0.0, IEEE8 dWorldBRY = 0.0);

	virtual NCSReadStatus Read(SDK::CBuffer2DVector &Buffers, NCSCellType eOutputType);
	virtual NCSReadStatus ReadLine(NCSCellType eOutputBufferType, UINT16 nBands, void **ppOutputLine, NCSCellType eOuputDataType, UINT32 *pLineSteps);
	virtual bool ReceivedBlock(NCSBlockId nBlock);

	virtual void UpdateViewStats();
	virtual NCSFileType GetFileType() { return(NCS_FILE_JP2); };

		/** Copy the parameters from the m_Parameters vector to the File instance */
	virtual void SetFileParameters();

	virtual void SetParameterInternal(NCS::CView::Parameter eType, ...);

	// added on 31/01/2013
	virtual bool ReadCancel();

protected:
		/** JP2 resampler */
	JPC::CResampler *m_pInput;

	virtual NCSError AutoScaleOpacityUp(SDK::CBuffer2D &Opacity, NCSCellType eType);
private:
	CError CreateMainTileParts(UINT32 nDatasetTLX, UINT32 nDatasetTLY, UINT32 nDatasetBRX, UINT32 nDatasetBRY, UINT32 nSizeX = 100, UINT32 nSizeY = 100);
	UINT64 MemoryToUse(INT32 nTopX, INT32 nLeftY,
		INT32 nBottomX,	INT32 nRightY,
		UINT32 nSizeX,	UINT32 nSizeY,
		INT32 &nNumVisibleTiles_X, INT32 &nNumVisibleTiles_Y, UINT32 &nResToDecode);
};
}
}

#endif // !NCSJP2READER_H
