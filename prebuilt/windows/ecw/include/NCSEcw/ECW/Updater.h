/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Updater.h 
** CREATED:  10/01/2006 3:27:34 PM
** AUTHOR:   Yakun Wang
** PURPOSE:  CWriter class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWUPDATER_H
#define NCSECWUPDATER_H

#ifndef NCSFILE_H
#include "NCSEcw/API/View.h"
#endif // NCSFILE_H

#include "NCSEcw/ECW/File.h"
#include "NCSEcw/ECW/Compress.h"
#include "NCSMemoryIOStream.h"
#include "NCSFileIOStream.h"
#include "NCSEcw/SDK/Rect2D.h"
#include "NCSEcw/ECW/Writer.h"

namespace NCS {
namespace ECW {
/**
 * CReader class - the JP2 file view class.
 * 
 * @author       Yakun Wang
 * @version      $Revision: #24 $ $Author: ywang $ $Date: 2014/03/04 $ 
 */
class NCSECW_IMPEX CUpdater :  public NCS::CView::CWriterBase, EcwCompressionTask {
public:
		/** Default constructor, initialises members */
	explicit CUpdater(NCS::CView *pView);
		/** Virtual Destructor */
	virtual ~CUpdater();
	virtual CError Open(const CString &URLPath, CIOStream *pStream);
	virtual NCSFileInfo *GetFileInfo();
	virtual NCSFileType GetFileType() { return(NCS_FILE_ECW); };
	virtual CError Update(const NCS::SDK::CRect2D& updateRect);
	virtual QmfLevel *GetTopQmf() { return EcwCompressionTask::pQmf; };
	virtual CError WriteLineBIL(NCSCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);


	class CTileJob : public CWriter::CTileJob
	{
	public:
		CTileJob(NCSBlockId nID, UINT32 nX, UINT32 nY, QmfLevel *pQmf, SDK::CBuffer2DVector *pBILData, 
			CUpdater &Writer, ECW::CFile &File, ECW::CFile::IWriterContext *pContext,
			const std::vector<IEEE4>& backgroundColor, bool bPrefillUpdateRegion, bool bCompressionUseSSE);
		virtual ~CTileJob();
		void Work();
		static NCSThreadLSKey s_ThreadKey;
	private:
		std::vector<IEEE4> m_backgroundColor;
		bool m_bPrefillUpdateRegion;
	};

	/**
	 * \fn	virtual bool CWriter::WriteBlockLength(NCSBlockId nID, UINT32 nLength);
	 *
	 * \brief	Writes a block length to the block offset table on disk.  This function
	 * 			needs work for the multiprocess approach, or we could get the lengths out of
	 * 			the indexed data file for each resolution.
	 *
	 * \param	nID	   	The block id.
	 * \param	nLength	The length of the block.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	virtual bool WriteBlockAndBlockLengthTable(NCSBlockId nID, UINT8 nResolution, NCS::CMemoryIOStream *pBlock);
	//virtual bool WriteBlockLength(NCSBlockId nID, UINT32 nLength);

	//NCSError WriteCompressedPreamble();
	virtual CError ConvertGeotiffToFileInfo(const GeotiffTagBox* pGeotiffConverter);
	bool WriteNULLBlock(NCSBlockId nID);

	void RegisterLocalViews(NCS::CView* pView);

	virtual bool NeedConvertRGBToYUV();
protected:
	NCSFileInfo	m_FileInfo;
	bool	m_bOurStream;
	virtual void Work(void *pDate);
	virtual void CleanUp();
	virtual SDK::CBuffer2DVector *GetTileRegion(INT32 nX, INT32 nY, QmfLevel *pLevel);

	INT32 m_nBlockSizeX;
	INT32 m_nBlockSizeY;

	NCS::CMutex m_mBlockLength;
	INT64 m_nOffsetToWaveletData;
	NCS::CFileIOStream m_BlockLengthTable;
	UINT64 m_nCurrentQMFEnd;


	bool m_bSerialiseBlocks;

	// todo: Might be able to push these into the CWriterBase when JP2 tile based
	//		 compression is implemented
	CThreadPool *m_pThreadPool;
	virtual UINT64 EstimateMemoryRequired();

	CNCSMutex m_localViewVecMutex;
	std::vector<NCS::CView*> m_localViewVector;
private:
	// Estimate memory required for outputing the file
	// Added on 21/03/2012
	UINT64 EstimateMemoryRequired(QmfLevel *pStartingLevel, INT32 nBlockSizeY);
};
}
}

#endif // !NCSECWWRITER_H
