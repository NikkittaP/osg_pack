/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Writer.h 
** CREATED:  10/01/2006 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CWriter class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWWRITER_H
#define NCSECWWRITER_H

#ifndef NCSFILE_H
#include "NCSEcw/API/View.h"
#endif // NCSFILE_H

#include "NCSEcw/ECW/File.h"
#include "NCSEcw/ECW/Compress.h"
#include "NCSMemoryIOStream.h"
#include "NCSFileIOStream.h"
#include <bitset>

namespace NCS {
namespace ECW {
/**
 * CReader class - the JP2 file view class.
 * 
 * @author       Simon Cope
 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/12/03 $ 
 */
class NCSECW_IMPEX CWriter: public NCS::CView::CWriterBase, EcwCompressionTask {
public:
		/** Default constructor, initialises members */
	explicit CWriter(NCS::CView *pView);
		/** Virtual Destructor */
	virtual ~CWriter();
	virtual CError Open(const CString &URLPath, CIOStream *pStream);
	virtual NCSFileInfo *GetFileInfo();
	virtual CError WriteLineBIL(NCSCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);
	virtual NCSFileType GetFileType() { return(NCS_FILE_ECW); };
	virtual CError WriteTiled();
	virtual QmfLevel *GetTopQmf() { return EcwCompressionTask::pQmf; };

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
	virtual bool WriteBlock(NCSBlockId nID, UINT8 nResolution, NCS::CMemoryIOStream &Block);
	virtual bool WriteBlockLength(NCSBlockId nID, UINT32 nLength);

	virtual bool WriteBlockOpacity(NCSBlockId nID, UINT8 nResolution, NCS::CMemoryIOStream &Block);
	virtual bool WriteBlockLengthOpacity(NCSBlockId nID, UINT32 nLength);

	NCSError WriteCompressedPreamble();
	virtual CError ConvertGeotiffToFileInfo(const GeotiffTagBox* pGeotiffConverter);
	bool WriteNULLBlock(NCSBlockId nID);
	virtual bool NeedConvertRGBToYUV();

	NCSTimeStampMs GetTotalWaitingTimeForReadQueue() { return m_totalWaitingTimeForReadQueue; };
	void IncrementWaitingTimeForReadQueue(const NCSTimeStampMs& inc)
	{
		m_totalWaitingTimeForReadQueue += inc;
	}

	class CTileJob : public CThreadPool::CJob
	{
	protected: 
		SDK::CBuffer2DVector m_TempVector;
	private: 
		SDK::CRect2D CalculateRegionAndTileCenter(INT32 &nCenterX, INT32 &nCenterY);
		void SetUpNullBuffersIfNeeded(const SDK::CRect2D &subRegion, bool bIsNullRegion);
		bool GetInputPartFromTileInput(const SDK::CRect2D &subRegion, const SDK::CRect2D *pUpdateRect, bool &bIsNULLRegion);
		void ThresholdOpacityValuesIfNeeded();
		void ConvertRGBToYUVIfNeeded();
		
	public:
		CTileJob(NCSBlockId nID, UINT32 nX, UINT32 nY, QmfLevel *pQmf, SDK::CBuffer2DVector *pBILData, NCS::CView::CWriterBase &Writer, ECW::CFile &File, ECW::CFile::IWriterContext *pContext, bool bCompressionUseSSE);
		virtual ~CTileJob();
		void Work();
		bool GetInputFromLLData(std::bitset<4> &bIsNULLRegion);
		bool GetInputFromTileInput(std::bitset<4> &bIsNULLRegion, SDK::CRect2D* pUpdateRect);
	protected:
		NCSBlockId m_nID;
		QmfLevel *m_pQmf;
		UINT32 m_nX;
		UINT32 m_nY;
		SDK::CBuffer2DVector *m_pBILData;
		NCS::CView::CWriterBase &m_Writer;
		ECW::CFile &m_File;
		ECW::CFile::IWriterContext *m_pWriterContext;

		// compiler wanted this?
		const CTileJob &operator=(const CTileJob &s) {
			m_pBILData = s.m_pBILData;
			m_nID = s.m_nID;
			m_Writer = s.m_Writer;
			m_File = s.m_File;
			m_nX = s.m_nX;
			m_nY = s.m_nY;
			m_pQmf = s.m_pQmf;
			m_pWriterContext = s.m_pWriterContext;
			return *this;
		}
	private: 
		bool m_bCompressionUseSSE;
	};

protected:
	NCSFileInfo	m_FileInfo;
	bool	m_bOurStream;
	virtual void Work(void *pDate);
	virtual void CleanUp();
	virtual SDK::CBuffer2DVector *GetTileRegion(INT32 nX, INT32 nY, QmfLevel *pLevel);
		/** Static Compression read callback hook */
	static NCSError	sECWCompressRead(void *pClient, UINT32 nNextLine, IEEE4 **ppInputArray);	
		/** Static Compression status callback hook */
	static void sECWCompressStatus(void *pClient, UINT32 nCurrentLine);
		/** Statuc Compression Cancel callback hook */
	static BOOLEAN	sECWCompressCancel(void *pClient);

	INT32 m_nBlockSizeX;
	INT32 m_nBlockSizeY;

	NCS::CMutex m_mBlockLength;
	SDK::CIndexedDataFile *m_pUnorderedBlocks;
	INT64 m_nOffsetToWaveletData;
	NCS::CFileIOStream m_BlockLengthTable;
	//only for version 2 with opacity band
	NCS::CFileIOStream* m_pBlockLengthTableOpacity; 

	bool m_bSerialiseBlocks;

	// todo: Might be able to push these into the CWriterBase when JP2 tile based
	//		 compression is implemented
	CThreadPool *m_pThreadPool;
	virtual UINT64 EstimateMemoryRequired();
private:
	// Estimate memory required for outputing the file
	// Added on 21/03/2012
	UINT64 EstimateMemoryRequired(QmfLevel *pStartingLevel, INT32 nBlockSizeY);

	NCSTimeStampMs m_totalWaitingTimeForReadQueue;
};
}
}

#endif // !NCSECWWRITER_H
