/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     TilePartHeader.h 
** CREATED:  13/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CTilePartHeader class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCTILEPARTHEADER_H
#define NCSJPCTILEPARTHEADER_H


#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Markers/SOTMarker.h"
#include "NCSEcw/JPC/Markers/CODMarker.h"
#include "NCSEcw/JPC/Markers/COCMarker.h"
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#include "NCSEcw/JPC/Markers/QCCMarker.h"
#include "NCSEcw/JPC/Markers/RGNMarker.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"
#include "NCSEcw/JPC/Markers/SODMarker.h"
#include "NCSEcw/JPC/Markers/PLTMarker.h"
#include "NCSEcw/JPC/Markers/PPTMarker.h"
#include "NCSEcw/JPC/Markers/COMMarker.h"
#include "NCSEcw/JPC/Nodes/Component.h"
#include "NCSEcw/JPC/Progression.h"
#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/JPC/JPC.h"

#include "NCSEcw/JPC/Nodes/TilePartHeaderBase.h"

// 21/03/2013
// turn on 13/05/2014
#ifndef NCSJPC_READ_MT_TILE 
#define NCSJPC_READ_MT_TILE
#endif

namespace NCS {
namespace JPC {
	/**
	 * CTilePartHeader class - the JPC codestream Tile-Part Header.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/12/03 $ 
	 */	
class NCSECW_IMPEX CTilePartHeader: public CTilePartHeaderBase, public SDK::CNode2D, public CProgression {
public:

	/** Enumerated header state types */
	typedef enum {
			/** Not Parse Yet */
		NotParseYet					= 0,
			/** Being Parse */
		BeingParse					= 1,
			/** Parsed */
		Parsed						= 2
	} HeaderState;

		/** COD coding style default Marker - optional */
	CCODMarker	m_COD;
		/** QCD quantization default Marker - optional */
	CQCDMarker	m_QCD;
		/** RGN markers */
	std::vector<CRGNMarker> m_Regions;

		/** If the tile has more than 1 tile part and this is tile part 0, this vectors
			contains all the other tile parts */
	std::vector<CTilePartHeaderBase *> m_TileParts;

		/** Up to which SOT has been read in? */
	INT16				m_nSOTsReadIn; // added 25/06/2012
		/** Are the SOTs read in already? */
	bool				m_bSOTsReadAlready; // Against bug EMS-407, added 12/12/2011
		/** State of header. May be associated with m_nSOTsReadIn */
	HeaderState			m_nHeaderState; // added 31/07/2012

		/** Tile Components */
	std::vector<CComponent*> m_Components;

		/** Tile m_nDeepestAllocatedResolution increased  */
	INT16 	m_nDeepestDynamicPLTAllocatedResolution; // added 16/03/2012

		/** The reruired number of packets for every resolution for main tile part after reading PLTs or dynamically creating PLT */
	std::vector<UINT32> m_NrRequiredPacketsList; // added on 22/06/2012

		/** The available number of packets for every resolution for main tile part after reading PLTs or dynamically creating PLT */
	//std::vector<UINT32> m_NrAvailablePacketsList; // added on 22/06/2012

		/** The available number of packets for main tile part after reading PLTs or dynamically creating PLT */
	UINT32				m_NrAvailablePackets; // added on 29/06/2012
		
	UINT32				m_nCurrDecodePacketNr; // added on 30/06/2012

		/** packet number range (last packet number - first packet number) for one precinct. 
		 *	It is different for each level for roder RLCP
		 */
	std::vector<UINT32>	m_PacketNrRange;

		/** only locked when parsing other TP. added on 26/10/2012	*/
	NCS::CMutex m_Mutex_ParseOtherTP;

		/** Encoder PLT Stream */
//	CIOStream		*m_pEncoderPLTFile;

	// added on 21/03/2013
	static NCS::CThreadPool *sm_pNodeWorker;
	static INT32	sm_nMaxThreads;

		/** Default constructor, initialises members */
	explicit CTilePartHeader(CJPC *pJPC);
		/** Virtual destructor */
	virtual ~CTilePartHeader();

		/** 
		 * Recalc the structure sizes required based on the current decoding info.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError RecalcSizes();
		/** 
		 * check if the structure is simple
		 * @return      void
		 */

	virtual void RecalcSimpleStructure(); // Against bug EMS-407, added 19/10/2011
		/** 
		 * Init the tile rect based on the currently available info.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */

	virtual CError InitRect();
	
		/**
		* Parse the marker from the JPC codestream.
		* Start offsets of Tile Part 
		* @return      CError	NCS_SUCCESS, or Error code on failure.
		*/
	virtual CError Parse(class CJPC &JPC, INT32 iIndex, std::vector<UINT64> &nTPOffsets);
		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		inOption	the required tile part ranges 
		 * @param		inTileNrStart: start tile number [0, CBuffer2D::MAXUINT32)
		 * @param		inTileNrEnd: end tile number [0, CBuffer2D::MAXUINT32), inclusive
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream, UINT8 inOption = 0, 
		UINT32 inTileNrStart = 0, UINT32 inTileNrEnd = NCSJPC_INIT_VALUE, INT8 nResolutionLevel = -1); //CBuffer2D::MAXUINT32);
		/**
		 * Get the Tile Index
		 * @return		INT32		Index of the tile, >= 0
		 */
	INT32 GetIndex();
		/**
		 * Get the Tile P index
		 * @return		INT32		P index of the tile, >= 0
		 */
	INT32 GetP();
		/**
		 * Get the Tile Q index
		 * @return		INT32		Q of the tile, >= 0
		 */
	INT32 GetQ();
		/**
		 * Get the PPx from the component resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		PPx
		 */
	UINT32 GetPPx(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the PPy from the component resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		PPy
		 */
	UINT32 GetPPy(UINT16 iComponent, UINT8 nResolution);

		/**
		 * Get the xcb' from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		xcb'
		 */
	UINT32 GetXCBPrime(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the ycb' from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		ycb'
		 */
	UINT32 GetYCBPrime(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the CB Width from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		xcb'
		 */
	UINT32 GetCBWidth(UINT16 iComponent, UINT8 nResolution);
		/**
		 * Get the CB Height from the resolution
		 * @param		INT32		Component Index
		 * @param		INT32		Resolution Level
		 * @return		INT32		ycb'
		 */
	UINT32 GetCBHeight(UINT16 iComponent, UINT8 nResolution);

		/** 
		 * Increment the progression order from the current location, parsing packet headers in the process.
		 * @param		pData		Data for Step function
		 * @param		bComplete	Progression order is complete.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Iterate(CTilePartHeader *pMainTP, void *pData, bool &bComplete);
		/** 
		 * Perform the "Step" for this progression.
		 * @return      bool		true, or false on error & Error is set to actual error code.
		 */
	virtual bool Step(CComponent *pComponent,
					  CResolution *pResolution,
					  CPrecinct *pPrecinct);

	virtual bool StepRow(class CComponent *pComponent,
						 class CResolution *pResolution);

		/**
		 * Get the sequential Packet Header for the specified packet number
		 * @param		nPacket		Packet number
		 * @return		CPacket	Packet Header struct (may be unparsed)
		 */
	CPacket *GetSequentialPacketHeader(UINT32 nPacket, INT16 &nioTPsot, UINT32 &nioPrevPLTnr, bool bPurge = true, UINT32 nLastPacketNr = NCSJPC_INIT_VALUE);
		/**
		 * Get the first packet number for this tile
		 * @return		UINT32	First packet number
		 */
	UINT32 GetFirstPacketNr();
		/**
		 * Get the number of packets for this tile
		 * @return		UINT32	Packet count
		 */
	UINT32 GetNrPackets();
		/**
		 * Get the available number of packets for this main tile part
		 * @return		UINT32	Packet count
		 */
	UINT32 GetNrPLTsPackets();
		/**
		 * Get the number of packets upto a level of resolution for this tile
		 * @return		UINT32	Packet count
		 */
	UINT32 GetNrPacketsGivenRes(UINT8 inResolutions); // added on 17/03/2012

		/**
		 * Get the number of packets upto a level of resolution for this tile
		 * @return		UINT32	Packet count
		 */
	//UINT32 GetNrAvaiPacketsGivenRes(UINT8 inResolutions); // added on 17/03/2012

		/**
		 * Get the number of packets upto a level of resolution for this tile
		 * @return		true if enough available packets have been loaded
		 */
	//bool NrAvailablePacketsCompleteGivenRes(UINT8 inResolutions);  // added on 22/06/2012

	// must be main TP
	//bool CheckRequiredSOTsReadIn(UINT32 nCurrViewResolution);
	bool CheckRequiredSOTsReadIn(UINT32 nCurrPacketNrToRead);

		/** 
		 * Write a BufferType line to the output.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent);	

//	bool OpenEncoderPLTFile(bool bWrite);
//	bool CloseEncoderPLTFile(bool bDelete);

	bool SimpleStructure() {
		return(m_bSimpleStructure);
	}

	std::vector<std::pair<INT32, INT32> >& Get_Res_PrecinctSize_List();

	void ResetPacketInfo() {m_FirstPacketNr = CBuffer2D::MAXUINT32; m_NrPackets = CBuffer2D::MAXUINT32;}; // Against bug EMS-407, added 27/10/2011
	UINT64 GetMemorySize(); // Against bug EMS-407, added 01/11/2011
	bool CreatePackets(bool bTilePartsAndPLTtoBeDeleted = false);	// Against bug EMS-407, added 02/11/2011
	bool ClearTilePartsAndPLTs();	// Against bug EMS-407, added 11/11/2011
	bool FreePLTsLength(bool bForced = false);	// Against bug EMS-407, added 22/11/2011
	bool ClearMainTilePart(bool bForced = false);	// added 30/07/2012

	bool CalcPacketNrRange(); // addd on 29/06/2012

	CTilePartHeaderBase *GetTilePart(CIOStream &Stream, INT32 iPart);
	
	UINT32 GetRef_OtherTPAccess(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_OtherTP_ref); // added on 26/10/2012

		return m_nOtherTPAccessRef;
	};
	void AddRef_OtherTPAccess(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_OtherTP_ref); // added on 26/10/2012

		m_nOtherTPAccessRef++;
	};
	void UnRef_OtherTPAccess(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_OtherTP_ref); // added on 26/10/2012

		if (m_nOtherTPAccessRef <= 0)
			NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_ERROR_LEVEL, NCS_T("Attempting to UnRef_OtherTPAccess below zero of the tile %d"), m_SOT.m_nIsot);
		else
			m_nOtherTPAccessRef--;
	};

	void AddRef(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_TP_ref); // added on 25/10/2012

		// if purging, wait until finish
		if (IsPurging()) {
			// wait until purging is done
			NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_DEBUG_LEVEL, NCS_T("CTilePartHeader::Parse: the tile %d is being purged"), m_SOT.m_nIsot);
			while (IsPurging()) {
				NCSSleep(5);
			}
			NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_DEBUG_LEVEL, NCS_T("CTilePartHeader::Parse: the tile %d has been purged"), m_SOT.m_nIsot);
		}

		m_nMainTPAccessRef++;
	};
	void UnRef(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_TP_ref); // added on 25/10/2012

		if (m_nMainTPAccessRef <= 0)
			NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_ERROR_LEVEL, NCS_T("Attempting to unref below zero of the tile %d"), m_SOT.m_nIsot);
		else
			m_nMainTPAccessRef--;
	};

	// added on 14/03/2013
	UINT32 GetRef_Read(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_TP_Read_ref); 

		return m_nMainTPReadRef;
	};
	void AddRef_Read(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_TP_Read_ref); 

		m_nMainTPReadRef++;
	};
	void UnRef_Read(void) {
		// need a lock to prevent multithreading issues
		NCS::CMutexLock _Lock(&m_Mutex_TP_Read_ref);

		if (m_nMainTPReadRef <= 0)
			NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_ERROR_LEVEL, NCS_T("Attempting to unref below zero of the tile %d"), m_SOT.m_nIsot);
		else
			m_nMainTPReadRef--;
	};

	bool IsPurging() {
		if (m_nMainTPPurgeState)
			return true;
		else
			return false;
	};

	// added on 11/01/2013 to quickly retrieve Component, Resolution and Precinct
	bool FindPacket(UINT32 nPacket, 
					UINT8 &nResolution, 
					UINT16 &nComponent, 
					UINT32 &nPrecinctX, 
					UINT32 &nPrecinctY, 
					UINT16 &nLayer);

	// added on 18/03/2013
	CError Check_COD_QCD();

	// added on 21/03/2013
	static void MTInit();
	static void MTFini();
#ifdef NCSJPC_READ_MT_TILE 
	static void QueueJob(NCS::CThreadPool::CJob *p) {
		CTilePartHeader::sm_pNodeWorker->QueueJob(p);
	};
	static INT32 GetQueueSize() {
		return CTilePartHeader::sm_pNodeWorker->GetQueueSize();
	};
	static INT32 GetMaxThreadSize() {
		return CTilePartHeader::sm_nMaxThreads;
	};
#endif

private:
	void Init();
	void Reset();
	void Release();
	// As deleting Component at Release() causes UnLink issue, use Purge instead.
	void Purge();

	bool SeekPacket(CJPC &JPC, CTilePartHeader *pTP, CPLTMarker &inPLT, UINT32 &inPLTNr, UINT32 &inNr, std::vector<CPacketLengthType> *pLengths,
		CPacket *pioPH); // Added on 13/12/2011

protected:

	UINT32 m_FirstPacketNr; // the first packet number in terms of whole file
	UINT32 m_NrPackets; // the number packets of the tile

		// Dynamic precincts in progression Step()
	bool m_bDynPrecinct;
		// Stream is seekable
	bool m_bSeekable;
		// Parsing Dynamic precincts in progression Step()
	bool m_bParsingHeaders;
		// Simple structure, R/P/L equal in all C and no POC
	bool m_bSimpleStructure;

		// for storing every resolution's precinct size used for only simple structure (m_bSimpleStructure)
	std::vector<std::pair<INT32, INT32> > m_comp_res_precinctSize_list;
	// only locked for m_comp_res_precinctSize_list. added on 06/11/2014 to prevent ECWP client crash, eg. for 25n_068e_sw_pk_01_245952n0680812e_r4c2_imagery.jp2, ms-epje-vl-19ql.jp2
	NCS::CMutex m_Mutex_comp_res_precinctSize_list;

	NCS::CMutex m_Mutex_TP_deleting;	// only locked when deleting other tile parts. added on 18/07/2012
	UINT32 m_nOtherTPAccessRef;		// when accessing other tile parts for packet header offsets. added on 18/07/2012
	NCS::CMutex m_Mutex_OtherTP_ref;	// only locked when m_nOtherTPAccessRef is being processed. added on 26/10/2012

	// 
	UINT32 m_nMainTPAccessRef;
	NCS::CMutex m_Mutex_TP_ref;	// only locked when m_nMainTPAccessRef is being processed. added on 25/10/2012

	UINT8 m_nMainTPPurgeState;	// only used when the main tile is purging and GetTile. added on 07/09/2012

	// added on 14/03/2013
	UINT32 m_nMainTPReadRef;
	NCS::CMutex m_Mutex_TP_Read_ref;	// only locked when m_nMainTPReadRef is being processed. 
};
}
}


#endif // !NCSJPCTILEPARTHEADER_H
