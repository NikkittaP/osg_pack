/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:    Marker.h 
** CREATED:  11/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCMARKER_H
#define NCSJPCMARKER_H

// Our own 64bit IOSTREAM class
#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H


namespace NCS {
namespace JPC {
	/**
	 * CMarker class - the base JPC codestream marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: scope $ $Date: 2007/12/13 $ 
	 */	
class NCSECW_IMPEX CMarker {
public:
	typedef enum {
		UNKNOWN		= 0x0000,
		SOC			= 0xFF4F,
		SIZ			= 0xFF51,
		COD			= 0xFF52,
		COC			= 0xFF53,
		TLM			= 0xFF55,
		PLM			= 0xFF57,
		PLT			= 0xFF58,
		QCD			= 0xFF5C,
		QCC			= 0xFF5D,
		RGN			= 0xFF5E,
		POC			= 0xFF5F,
		PPM			= 0xFF60,
		PPT			= 0xFF61,
		CRG			= 0xFF63,
		COM			= 0xFF64,
		SOT			= 0xFF90,
		SOP			= 0xFF91,
		EPH			= 0xFF92,
		SOD			= 0xFF93,
		EOC			= 0xFFD9
	} Type;

		/** Marker type */ 
	Type	m_eMarker;

		/** Marker Offset into stream */
	UINT64  m_nOffset;
		/** Marker Length */
	UINT16  m_nLength;
		/** Do we have this marker */
	bool	m_bHaveMarker;
		/** Is this marker valid */
	bool	m_bValid;

		/** Default constructor, initialises members */
	CMarker();
		/** Virtual destructor */
	virtual ~CMarker();

		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream);
		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);


		/** 
		 * Get length of a packet in the file.
		 * @param		src			Copy source
		 * @return      void
		 */
	void operator=( const CMarker& src );

		/** 
		 * check if acutal data length equals to required one
		 * @param       pwChar	char to print
		 * @return      Stream		IOStream to use to parse file.
		 * @return      nOffset0	offset when started the mark of the stream.
		 */
	CError CheckReadLength(const NCSTChar *pwChar, CIOStream &Stream, UINT64 nOffset0);
};
}
}
#endif // !NCSJPCMARKER_H
