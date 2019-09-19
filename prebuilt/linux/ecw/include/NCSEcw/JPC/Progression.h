/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Progression.h 
** CREATED:  13/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CProgression class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPROGRESSION_H
#define NCSJPCPROGRESSION_H

#include "NCSDefs.h"
#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"

namespace NCS {
namespace JPC {
	/**
	 * CProgression class - Implements JPC progressions.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CProgression {
public:
		/** Current Tile being decoded */
	UINT16	m_nCurTile;
		/** Current Layer being decoded */
	UINT16	m_nCurLayer;
		/** Current Resolution being decoded */
	UINT8	m_nCurResolution;
		/** Current Component being decoded */
	UINT16	m_nCurComponent;
		/** Current Precinct X being decoded */
	UINT32	m_nCurPrecinctX;
		/** Current Precinct Y being decoded */
	UINT32	m_nCurPrecinctY;

		/** Default constructor, initialises members */
	CProgression();
		/** Virtual destructor */
	virtual ~CProgression();
	
		/** 
		 * Get the current POC marker PO if present.
		 * @return      CPOCMarker::ProgressionOrder	Current PO.
		 */
	CPOCMarker::ProgressionOrder *CurrentPO(class CTilePartHeader *pTP);
		/** 
		 * Increment the current POC marker PO if present and current PO is complete.
		 * @param		bComplete	Flag indicating if current PO is complete
		 * @return      CError	NCS_SUCCESS or error on failure.
		 */
	CError IncrementPO(CTilePartHeader *pMainTP, bool &bComplete);

		/** 
		 * Start the progression order from the beginning.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Start(CTilePartHeader *pMainTP);
	virtual CError Start(CTilePartHeader *pMainTP, INT32 nComponent, INT32 nResolution);
	CError StartParameters(CTilePartHeader *pMainTP, INT32 nComponent, INT32 nResolution);
	static void CalculateMinIncremements(CTilePartHeader *pMainTP, CPOCMarker::ProgressionOrder *pPO, INT32 &nIncTX, INT32 &nIncTY);
		/** 
		 * Increment the progression order from the current location, parsing packet headers in the process.
		 * @param		pData		Data for Step function
		 * @param		bComplete	Progression order is complete.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Iterate(CTilePartHeader *pMainTP, void *pData, bool &bComplete);

		/** 
		 * Perform the "Step" for this progression.
		 * @return      bool		true, or false on error & m_Error is set to actual error code.
		 */
	virtual bool Step(class CComponent *pComponent,
					  class CResolution *pResolution,
					  class CPrecinct *pPrecinct);
		/** 
		 * Perform the "StepRow" for this progression.
		 * The progression stepper can return true here to skip the entire row
		 * 
		 * @return      bool		true, or false on row not skipped.
		 */
	virtual bool StepRow(class CComponent *pComponent,
						 class CResolution *pResolution);

		/**
		 * Calculate the Precinct 'k' X from the current progression
		 * @return		INT32		Precinct 'k' X
		 */
	UINT32 CalculatePrecinctX(CTilePartHeader *pMainTP, class CComponent *pComponent, class CResolution *pResolution);
		/**
		 * Calculate the Precinct 'k' Y from the current progression
		 * @return		INT32		Precinct 'k' Y
		 */
	UINT32 CalculatePrecinctY(CTilePartHeader *pMainTP, class CComponent *pComponent, class CResolution *pResolution);
		/**
		 * Calculate the Precinct 'k' X given X position
		 * @return		INT32		Precinct 'k' X
		 */
	static UINT32 CalculatePrecinctX(INT32 nCurTX, CResolution *pResolution);
		/**
		 * Calculate the Precinct 'k' Y given Y position
		 * @return		INT32		Precinct 'k' Y
		 */
	static UINT32 CalculatePrecinctY(INT32 nCurTY, CResolution *pResolution);	

	PacketId CurrentPacket() { return(m_nCurPacket); };
	PacketId NextPacket() { return(m_nCurPacket++); };
	void SetPacket(PacketId n) { m_nCurPacket = n; }; 

	bool InitAvailableParam(CTilePartHeader *pMainTP); // addd on 25/06/2012
#if 0
	bool AssignAvailablePacketsToEachRes(CTilePartHeader *pMainTP, UINT32 inNumPLTs, class CPLTMarker *inPLTs);	// added on 22/06/2012
	void ClearAvailablePacketsToEachRes(CTilePartHeader *pMainTP); // addd on 25/06/2012
#endif

	bool CalcFirstPacketNr(CPrecinct *pPrecinct); // addd on 29/06/2012
	bool CalcFirstPacketNr(CResolution *pRequResolution, UINT32 *pioFirstPacketNr = NULL, UINT32 nRequPrecinctX = 0, UINT32 nRequPrecinctY = 0); // addd on 29/06/2012
	bool CalcFirstPacketNr(CComponent *pRequComponent); // addd on 29/06/2012
	bool CalcFirstPacketNr_CPRL(CPrecinct *pPrecinct); // addd on 29/11/2012


protected:
		/** Current TX being decoded */
	INT32	m_nCurTX;
		/** Current TY being decoded */
	INT32	m_nCurTY;
		/** Current TX increment amount */
	INT32	m_nIncTX;
		/** Current TY increment amount */
	INT32	m_nIncTY;
	
		/** Current Packet Number */
	PacketId m_nCurPacket;

	// Data for step function
	void *m_pData;
	// Progression order is complete
	bool m_bDone;
	// Error code when Step() returns false
	CError m_Error;

		/** Current Layer being available */
	UINT16	m_nCurAvaiLayer;	// added on 25/06/2012
		/** Current Resolution being available */
	UINT8	m_nCurAvaiResolution;	// added on 25/06/2012
		/** Current Component being available */
	UINT16	m_nCurAvaiComponent;	// added on 25/06/2012
		/** Current Precinct X being available */
	UINT32	m_nCurAvaiPrecinctX;	// added on 25/06/2012
		/** Current Precinct Y being available */
	UINT32	m_nCurAvaiPrecinctY;	// added on 25/06/2012

		/** Current TX being available */
	INT32	m_nCurAvaiTX;	// added on 25/06/2012
		/** Current TY being available */
	INT32	m_nCurAvaiTY;	// added on 25/06/2012

	typedef std::pair<INT32, INT32> pairINT32;
	typedef std::pair<bool, bool> pairBOOL;
};

}
}

#endif // !NCSJPCPROGRESSION_H
