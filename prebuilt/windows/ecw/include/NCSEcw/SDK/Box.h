/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Box.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CBox class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSSDKBOX_H
#define NCSSDKBOX_H


// Our own 64bit IOSTREAM class
#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H

// STD list template
#include <list>

namespace NCS {
namespace SDK {
	class CFileBase;

/**
 * CBox class - the basic SDK file box.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: mdunne $ $Date: 2011/01/10 $ 
 */
class NCSECW_IMPEX CBox {
public:
		/** A list of boxes. */
	typedef std::vector<CBox *> CBoxList;

		/** Absolute file Offset to the Box in bytes */
	UINT64			m_nBoxOffset;
		/** Length of this box in bytes. */
	UINT64			m_nXLBox;
		/** Type of box, 4 char code stored in a UINT32. */
	UINT32			m_nTBox;
		/** Absolute file Offset to the DBox in bytes */
	UINT64			m_nDBoxOffset;
		/** Length of box data in bytes */
	UINT64			m_nLDBox;
		/** We have parsed this box */
	bool			m_bHaveBox;
		/** This box is valid */
	bool			m_bValid;
		/** Is a superbox */
	bool			m_bIsSuperBox;
		/** Box(es) that must preceed this one in the file */
	CBoxList	m_Prev;
		/** Box(es) that must proceed this one in the file */
	CBoxList	m_Next;

		/** Default constructor for a box.  Initialises members */
	CBox();
		/** Copy constructor for a box.  Initialises members */
	CBox(const CBox &s);
		/** Virtual destructor for a box.  Releases members */
	virtual ~CBox();

		/** 
		 * Set which Box(es) preceed this one in the file 
		 * @param       pFirst		NULL terminated varargs list of boxes that must preceed this one
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError SetPrevBoxes(CBox *pFirst, ...);  
		/** 
		 * Set which Box(es) proceed this one in the file
		 * @param       pFirst		NULL terminated varargs list of boxes that must proceed this one
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError SetNextBoxes(CBox *pFirst, ...); 

		/** 
		 * Update the extended length of the box before writing
		 */
	virtual void UpdateXLBox(void);
		/** 
		 * Parse the base box from the SDK file.
		 * @param		SDKFile		SDK file being parsed
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(CFileBase &FileBase, CIOStream &Stream);
		/** 
		 * UnParse the box to the SDK file.
		 * @param		SDKFile		SDK file being parsed
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(CFileBase &FileBase, CIOStream &Stream);
		/** 
		 * assignment operator
		 * @param       s			Source CBox
		 * @return      CBox&	Reference to post-assigned CBox
		 */
	CBox& operator=(const CBox& s) {
			m_nBoxOffset = s.m_nBoxOffset;
			m_nXLBox = s.m_nXLBox;
			m_nTBox = s.m_nTBox;
			m_nDBoxOffset = s.m_nDBoxOffset;
			m_nLDBox = s.m_nLDBox;
			m_bHaveBox = s.m_bHaveBox;
			m_bValid = s.m_bValid;
			m_bAppendBoxForUpdate = s.m_bAppendBoxForUpdate;
			return(*this);
	   };
		/** 
		 * check if acutal data length equals to required one
		 * @param       pwChar	char to print
		 * @return      Stream		IOStream to use to parse file.
		 * @return      nOffset0	offset when started the mark of the stream.
		 */
	CError CheckReadLength(const NCSTChar *pwChar, CIOStream &Stream, UINT64 nOffset0);

	void SetAppendBoxForUpdate(bool bAppendBoxForUpdate) { m_bAppendBoxForUpdate = bAppendBoxForUpdate; }

	bool GetAppendBoxForUpdate() { return m_bAppendBoxForUpdate; }
protected:
	/** Append box for update, this is used for ingore the original box and just append updated to the end of file stream  */
	bool			m_bAppendBoxForUpdate;

};
}
}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::SDK::CBox CNCSSDKBox;
#endif

#endif // !NCSSDKBOX_H
