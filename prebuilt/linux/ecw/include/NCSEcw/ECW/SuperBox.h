/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     SuperBox.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSuperBox class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWSUPERBOX_H
#define NCSECWSUPERBOX_H

// Box class
#ifndef NCSSDKBOX_H
#include "NCSEcw/SDK/Box.h"
#endif // NCSSDKBOX_H

namespace NCS {
namespace ECW {
/**
 * CSuperBox class - a box containing a collection of ECW file boxes.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: mdunne $ $Date: 2011/01/10 $ 
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
		 * Parse the base box from the ECW file.
		 * @param		ECWFile		ECW file being parsed
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual void UpdateXLBox(void);

	virtual CError Parse(class SDK::CFileBase &FileBasee, CIOStream &Stream);
		/** 
		 * UnParse the box to the ECW file.
		 * @param		ECWFile		ECW file being parsed
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class SDK::CFileBase &FileBase, CIOStream &Stream);
		/** 
		 * Get a box of the specified type from within this superbox.  Recursive.
		 * @param		nTBox		ECW box type to find
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
		 * the superbox to do specific tests, eg for a GeoECW-style UUID box.
		 * 
		 * @param		TestBox		Box being tested against
		 * @param		ThisBox		Box being tested
		 * @return      bool		true/false "Required" test result.
		 */
	virtual bool Required(CBox &TestBox, CBox &ThisBox);

	//whether the superbox itself means to be serialised.
	bool m_bItselfSerialised;
};
}
}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::ECW::CSuperBox CNCSECWSuperBox;
#endif

#endif // !NCSECWSUPERBOX_H
