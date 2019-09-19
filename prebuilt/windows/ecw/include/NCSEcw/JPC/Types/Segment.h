/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Segment.h
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSJPC Segment class
** EDITS:    [xx] ddMmmyy NAME COMMENTS
*******************************************************/

#ifndef NCSJPCSEGMENT_H
#define NCSJPCSEGMENT_H

#ifndef NCSJPCTYPES_H
#include "NCSEcw/JPC/Types/Types.h"
#endif // NCSJPCTYPES_H

namespace NCS {
	namespace JPC {
		// Segment class
		class NCSECW_IMPEX CSegment {
		public:
			/** Segment length */
			UINT32	m_nLength;
			/** Segment Index */
			UINT16	m_nIndex;
			/** Number of passes in this segment */
			UINT8	m_nPasses;
			/** Segment data */
			UINT8	*m_pData;
			/** Number of passes needed to decode nth layer is in m_vPassesPerQualityLayers[n]*/
			std::vector<UINT8> m_vPassesPerQualityLayers;
			/** Data length to decode nth layer is in m_vDataLengthsPerQualityLayers[n]*/
			std::vector<UINT32> m_vDataLengthsPerQualityLayers;
			/** Default constructor */
			CSegment();
			/** Copy constructor */
			CSegment(const CSegment &s);
			/** Virtual destructor */
			virtual ~CSegment();
			UINT8 GetNumberOfPassesNeededToDecodeQualityLayer(UINT16 nQualityLayer) const;
			UINT32 GetDataLengthNeededToDecodeQualityLayer(UINT16 nQualityLayer) const;
		};
		class NCSECW_IMPEX CNextSegment:public CSegment{
		public: 
			UINT16 m_nQualityLayer;
		};
	}
}

#endif // NCSJPCSEGMENT_H
