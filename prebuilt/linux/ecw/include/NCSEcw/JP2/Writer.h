/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Writer.h 
** CREATED:  10/01/2006 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CWriter class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJP2WRITER_H
#define NCSJP2WRITER_H

#ifndef NCSFILE_H
#include "NCSEcw/API/View.h"
#endif // NCSFILE_H

#include "NCSEcw/JP2/File.h"
//#include "NCSECWCompress.h"

namespace NCS {
namespace JP2 {
/**
 * CWriter class - the JP2 file view class.
 * 
 * @author       Simon Cope
 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/12/03 $ 
 */
class NCSECW_IMPEX CWriter: public NCS::CView::CWriterBase {
public:
		/** Default constructor, initialises members */
	explicit CWriter(NCS::CView *pView);
		/** Virtual Destructor */
	virtual ~CWriter();
	virtual CError Open(const CString &URLPath, CIOStream *pStream);
	virtual NCSFileInfo *GetFileInfo();
	virtual CError WriteLineBIL(NCSCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);
	virtual NCSFileType GetFileType() { return(NCS_FILE_JP2); };
		/** Copy the parameters from the m_Parameters vector to the File instance */
	virtual void SetFileParameters();
	virtual CError ConvertGeotiffToFileInfo(const NCS::ECW::GeotiffTagBox* pGeotiffConverter);
	virtual UINT64 EstimateMemoryRequired();
protected:
			/** JP2 Compression Output node */
	SDK::CNode2D *m_pOutputNode;
	virtual void Work(void *pDate);
};
}
}

#endif // !NCSJP2WRITER_H
