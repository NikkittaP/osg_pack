/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/

#ifndef NCS_ECWOPACITY_VIEW
#define NCS_ECWOPACITY_VIEW

#include "NCSEcw/API/View.h"

namespace NCS {
namespace ECW {
	class CReader;
#ifndef NCS_MOBILE
	class CWriter;
#endif
}
namespace JP2 {
	class CReader;
#ifndef NCS_MOBILE
	class CWriter;
#endif
}
namespace JPC {
	class CResampler;
}
namespace SDK {
	class CFileBase;
}

namespace ECW {

class NCSECW_IMPEX CECWOpacityView: public NCS::CView {
public:
	CECWOpacityView();

	CError OpenOpacityFileView(NCS::CView *pParentView, SDK::CFileBase *pFile, bool bProgressiveDisplay, NCSFileType eType);
	CError ReadOpacityBand(SDK::CBuffer2DVector &Buffers, INT32 m_nAllOpacityBandNumber);
	CError Close ( bool bFreeCache = false );

	virtual bool ReceivedBlock(NCSBlockId nBlock);

	virtual void Queue();
	virtual void Work();
protected:
	NCS::CView *m_pParentView;
};
}
}

#endif