/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: CurrentView.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Current view header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_CURRENT_VIEW_H
#define NCS_API_CURRENT_VIEW_H

#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif

#include "ViewSize.h"
#include "ImageCoordinatesRect.h"
#include "WorldCoordinatesRect.h"
#include <vector>

typedef struct NCSSetViewInfo NCSSetViewInfo;

namespace NCS {
namespace API {

/**
 * \class	NCS::API::CCurrentView
 * 
 * \brief	The current view parameters defined by either a SetView call or a ReadRegion call.
 * \ingroup	simplifiedclasses
 * 
 * \author	Intergraph Perth
 * \date	9/08/2013
 */
class NCSECW_IMPEX CCurrentView {
public:
	CCurrentView();
	virtual ~CCurrentView();
	CCurrentView(const CCurrentView &s);
            
#ifndef SWIG
	const CCurrentView &operator=(const CCurrentView &s);
	const CCurrentView &operator=(const NCSSetViewInfo *pFileBandInfo);
	NCSSetViewInfo *Get() const { return m_pCurrentView; };
#endif

	/**
	 * \fn	std::vector<unsigned int> CCurrentView::GetBandlist();
	 *
	 * \brief	Gets the bandlist.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The bandlist.
	 */
	std::vector<unsigned int> GetBandlist();

	/**
	 * \fn	NCSImageCoordinatesRect CCurrentView::GetImageCoordinates();
	 *
	 * \brief	Gets view image coordinates.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The view image coordinates.
	 */
	NCSImageCoordinatesRect GetImageCoordinates();

	/**
	 * \fn	NCSWorldCoordinatesRect CCurrentView::GetWorldCoordinates();
	 *
	 * \brief	Gets view world coordinates.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The view world coordinates.
	 */
	NCSWorldCoordinatesRect GetWorldCoordinates();

	/**
	 * \fn	NCSViewSize CCurrentView::GetSize();
	 *
	 * \brief	Gets view size in pixel dimensions.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The view size.
	 */
	NCSViewSize GetSize();

	/**
	 * \fn	unsigned int CCurrentView::GetBlocksAvailable();
	 *
	 * \brief	Gets blocks available.  Only really useful for ECWP.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The blocks available.
	 */
	unsigned int GetBlocksAvailable();

	/**
	 * \fn	unsigned int CCurrentView::GetBlocksAvailableAtSetView();
	 *
	 * \brief	Gets blocks available at set view.  Only really useful for ECWP.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The blocks available at set view.
	 */
	unsigned int GetBlocksAvailableAtSetView();

	/**
	 * \fn	unsigned int CCurrentView::GetBlocks();
	 *
	 * \brief	Gets blocks in view.  Only really useful for ECWP.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The blocks in view.
	 */
	unsigned int GetBlocks();

private:
#ifndef SWIG
	NCSSetViewInfo *m_pCurrentView;
#endif
};
        
} // API
} // NCS

#endif // NCS_API_CURRENT_VIEW_H
	