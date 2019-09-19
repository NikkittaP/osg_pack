/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     SuperBox.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSuperBox class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJP2SUPERBOX_H
#define NCSJP2SUPERBOX_H

// Box class
#ifndef NCSSDKBOX_H
#include "NCSEcw/SDK/Box.h"
#endif // NCSSDKBOX_H

namespace NCS {
namespace JP2 {
/**
 * CSuperBox class - a box containing a collection of JP2 file boxes.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
 */
	class NCSECW_IMPEX CSuperBox: public SDK::CBox {
public:
		/** List of other box(es) that we found in this superbox - UUID, XML etc */
	CBoxList m_OtherBoxes;	
		/** List of other box(es) that we own and must delete on destruction */
	CBoxList m_OwnedBoxes;	

		/** Default constructor */
	CSuperBox();
		/** Virtual destructor */
	virtual ~CSuperBox();

		/** 
		 * Set which Box(es) can exist within this superbox
		 * @param       pFirst		NULL terminated varargs list of boxes that can exist in this superbox
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError SetBoxes(CBox *pFirst, ...);
		/** 
		 * Parse the base box from the JP2 file.
		 * @param		JP2File		JP2 file being parsed
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual void UpdateXLBox(void);

	virtual CError Parse(class SDK::CFileBase &FileBase, CIOStream &Stream);
		/** 
		 * UnParse the box to the JP2 file.
		 * @param		JP2File		JP2 file being parsed
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class SDK::CFileBase &FileBase, CIOStream &Stream);

		/** 
		 * Get a box of the specified type from within this superbox.  Recursive.
		 * @param		nTBox		JP2 box type to find
		 * @param		pLast		Last box returned to iterate.
		 * @param		pbSkip		Skip a box.
		 * @return      CBox*	Box instance, or NULL on failure.
		 */
	CBox *GetBox(UINT32 nTBox, CBox *pLast = NULL, bool *pbSkip = NULL);

		/**
		 * Get the last box that has a box length of 0. Recursive.
		 * @param		pLast		Last box returned to iterate.
		 * @param		pbSkip		Skip a box.
		 * @return      CBox*		Box instance, or NULL on failure.
		 */
	CBox *GetLastBox(CIOStream &Stream, CBox *pLast = NULL, bool *pbSkip = NULL);
	/**
	* Get number of direct child boxes of the superbox.
	* @return      int		Number of top level boxes.
	*/
	int GetTopLevelBoxCount();
	/*
	* Get top level box of the superbox by number
	* @return     CBox*		Box instance, or NULL on failure.
	*/
	SDK::CBox* GetTopLevelBox(int nBoxNumber);
protected:
		/** List of box(es) that can exist within this superbox */
	CBoxList m_Boxes;	

		/** 
		 * Is this box required.  This test is in a separate method to allow derived classes of
		 * the superbox to do specific tests, eg for a GeoJP2-style UUID box.
		 * 
		 * @param		TestBox		Box being tested against
		 * @param		ThisBox		Box being tested
		 * @return      bool		true/false "Required" test result.
		 */
	virtual bool Required(CBox &TestBox, CBox &ThisBox);
};
}
}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::JP2::CSuperBox CNCSJP2SuperBox;
#endif

#endif // !NCSJP2SUPERBOX_H
