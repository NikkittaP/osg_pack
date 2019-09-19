/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     MainHeader.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CMainHeader class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCMAINHEADER_H
#define NCSJPCMAINHEADER_H

#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Markers/SOCMarker.h"
#include "NCSEcw/JPC/Markers/SIZMarker.h"
#include "NCSEcw/JPC/Markers/CODMarker.h"
#include "NCSEcw/JPC/Markers/COCMarker.h"
#include "NCSEcw/JPC/Markers/EOCMarker.h"
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#include "NCSEcw/JPC/Markers/QCCMarker.h"
#include "NCSEcw/JPC/Markers/RGNMarker.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"
#include "NCSEcw/JPC/Markers/TLMMarker.h"
#include "NCSEcw/JPC/Markers/PLMMarker.h"
#include "NCSEcw/JPC/Markers/PPMMarker.h"
#include "NCSEcw/JPC/Markers/CRGMarker.h"
#include "NCSEcw/JPC/Markers/COMMarker.h"
#include "NCSEcw/JPC/Types/Types.h"

namespace NCS {
namespace JP2 {
	class CFile;
}
namespace JPC {
	/**
	 * CMainHeader class - the JPC codestream Main Header.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CMainHeader {
public:
		/** Main header is valid */
	bool	m_bValid;

		/** SOC start of codestream Marker - required */
	CSOCMarker	m_SOC;
		/** SIZ image size Marker - required */
	CSIZMarker	m_SIZ;
		/** COD coding style default Marker - required */
	CCODMarker	m_COD;
		/** QCD quantization default Marker - required */
	CQCDMarker	m_QCD;
		/** EOC end of codestream Marker - required */
	CEOCMarker	m_EOC;
		/** RGN markers */
	std::vector<CRGNMarker> m_RGNs;

		/** Any progression order changes 
		 * NOTE: this over-rides any progression in a tile-part COD marker
		 */
	CPOCMarker	m_POC;
		/** TLM markers */
	std::vector<CTLMMarker> m_TLMs;
		/** PLM markers */
	std::vector<CPLMMarker> m_PLMs;
		/** PPM markers */
	std::vector<CPPMMarker> m_PPMs;
		/** Are the PPM's from the file or dynamically created? */
	bool		m_bFilePPMs;

		/** CRG component registration - optional */
	CCRGMarker	m_CRG;
		/** COM Marker - compression only */
	CCOMMarker	m_COM;
		/** locations for COM Markers - decompression only */
	std::vector< UINT64	> m_COM_Loc_Vec;

		/** Are tiles (SOTs) stored order of by Node Wide? */
	bool		m_bSOTsOrderNodeWide; // Against bug EMS-407, added 03/11/2011

		/** parse state of the 1st SOTs */
	UINT8		m_nFirstSOTsReadAlready; // added 06/04/2012

		/** Start offsets of Tile Parts, very useful when some tiles are read in many times */
	std::vector< std::vector<UINT64> > m_nTPOffsets; // [main tile part #][tile part # in the main tile part] added 26/03/2012
		/** parse state of the offsets of Tile Parts */
	UINT8		m_nTPOffsetsReadAlready;
	
		/** creation state of Tile Part Header */
	UINT8		m_nCreateTilesAlready;
	

		/** Tile-row list, added on 22/12/2011 to reduce calculation for every line 
		 *  Each tile-row holds the number of references which are decoding
		 */
	std::vector<UINT32> m_vTileRowList;	

		/** dimensions of every resolution level upto 256 levels.
		 *  m_vResolutionSizeList[0] has full image size, until m_vResolutionSizeList[i] with image size 1
		 */
	std::vector<std::pair<UINT32, UINT32> > m_vResolutionSizeList; // added 09/07/2014

		/** Default constructor, initialises members */
	CMainHeader();
		/** Virtual destructor */
	virtual ~CMainHeader();

		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream);

		/** 
		 * Parse the SOT markers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	//CError ParseSOTs_MemoryWise(class CJPC &JPC, CIOStream &Stream,
	//	UINT32 inTileNrStart, UINT32 inTileNrEnd);

		/** 
		 * Parse the SOT markers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError ParseSOTs_MemoryWise_OneTile(class CJPC &JPC, CIOStream &Stream,
		UINT32 inTileNr, UINT32 nCurrPacketNrToRead = NCSJPC_INIT_VALUE);

		/** 
		 * Parse the MAIN (FIRST) SOT markers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError ParseMainSOTs(class CJPC &JPC, CIOStream &Stream, INT8 inResolutionLevel);

		/** 
		 * Calculate the MAIN (FIRST) SOT markers psotions from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError CalcTPOffsets(CJPC &JPC, CIOStream &Stream);

		/** 
		 * Parse the MAIN (FIRST) SOT markers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError ParseMainSOT(class CJPC &JPC, CIOStream &Stream, UINT32 inTileNr, UINT32 inResolutionLevel = -1);

		/** 
		 * Parse the SOT markers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError ParseSOTs(class CJPC &JPC, CIOStream &Stream, UINT8 inTPreadOption = 2, INT8 inResolutionLevel = -1);

		/** 
		 * Parse comments from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		COM_Vec		comments to return.
		 */
	CError ParseComments(CJPC &JPC, CIOStream &Stream, std::vector< CCOMMarker > &COM_Vec);

		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

		/**
		 * Get X0, the component top left in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentX0(INT32 iComponent);
		/**
		 * Get X1, the component bottom right in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentX1(INT32 iComponent);
		/**
		 * Get Y0, the component top left in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentY0(INT32 iComponent);
		/**
		 * Get Y1, the component bottom right in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component coordinate
		 */
	INT32 GetComponentY1(INT32 iComponent);
		/**
		 * Get the component width in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component width
		 */
	INT32 GetComponentWidth(INT32 iComponent);
		/**
		 * Get the component height in reference grid
		 * @param		INT32		Component Index
		 * @return		INT32		Component height
		 */
	INT32 GetComponentHeight(INT32 iComponent);

		/**
		 * Get the number of tile across the image
		 * @return		INT32		Number of tiles across image
		 */
	INT32 GetNumXTiles();
		/**
		 * Get the number of tile down the image
		 * @return		INT32		Number of tiles down image
		 */
	INT32 GetNumYTiles();
		/**
		 * Get the Tile P index from the tile index
		 * @param		INT32		Tile Index, >= 0
		 * @return		INT32		X of the tile, >= 0
		 */
	INT32 GetTilePFromIndex(INT32 iIndex);
		/**
		 * Get the Tile Q index from the tile index
		 * @param		INT32		Tile Index, >= 0
		 * @return		INT32		Y of the tile, >= 0
		 */
	INT32 GetTileQFromIndex(INT32 iIndex);
		/**
		 * Get the Tile Index from the tile PQ index
		 * @param		INT32		Tile P Index
		 * @param		INT32		Tile Q Index
		 * @return		INT32		Tile Index
		 */
	INT32 GetTileIndexFromPQ(INT32 iPIndex, INT32 iQIndex);

		/** Tmp directory for encoding */
	NCSTChar				*m_pTmpDir;

		/** Encoder PLT Stream - holds all packet lengths during encoding */
	CIOStream		*m_pEncoderPLTFile;
		/** Encoder Resolution Packet Offset temp files */
	CIOStream		*m_pEncoderOffsetFile;
		/** Encoder Resolution Packet temp files */
	CIOStreamVector m_EncoderFiles;

	bool OpenEncoderFiles(bool bWrite);
	bool CloseEncoderFiles();
	bool ResetEncoderFiles();

		/** Calculate every resolution's dimension */
	bool CalResolutionDimensions();
		
		/** Get image dimension of the given resolution */
	bool GetResolutionDimension(UINT32 nRes, UINT32 &nioImageResXSize, UINT32 &nioImageResYSize);

protected:
	friend class CTilePartHeader;
	friend class CTilePartHeaderBase;
	friend class JP2::CFile;
			/** CodingStyle for each component */
	std::vector<CCODMarker> m_CodingStyles;
		/** QuantizationStyle for each component */
	std::vector<CQCDMarker> m_QuantizationStyles;

private:
	/** start of the first SOT */
	UINT64		m_nFirstSOToffset; // added 27/10/2011
	bool		m_bFirstSOTsToReadWithinParse;	// added on 22/03/2012

};
}
}


#endif // !NCSJPCMAINHEADER_H
