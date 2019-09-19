/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     TagTree.h 
** CREATED:  28/01/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CTagTree class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCTAGTREE_H
#define NCSJPCTAGTREE_H

#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif
#ifdef NOTDEF
#ifndef NCSJPCDUMP_H
#include "NCSJPCDump.h"
#endif // NCSJPCDUMP_H
#endif

#include <vector>

namespace NCS {
namespace JPC {
	/**
	 * CTagTree class - the JPC codestream TagTree Header.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CTagTree
#ifdef NOTDEF
: public CDump 
#endif //_DEBUG
{
public:
		/** Default constructor, initialises members */
	CTagTree();
		/** Virtual destructor */
	virtual ~CTagTree();

		/** 
		 * Set the dimensions of the 2D tag tree.
		 * @param		nWidth		Number of columns in the tag tree.
		 * @param		nHeight		Number of rows in the tag tree.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual void SetDimensions(INT32 nWidth, INT32 nHeight);
		/** 
		 * Parse the tree from the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		nX			X coordinate of cell to terminate parsing on.
		 * @param		nY			Y coordinate of cell to terminate parsing on.
		 * @param		nAbort		Abort parsing when cell value >= this value
		 * @return      bool	NCS_SUCCESS, or Error code on failure.
		 */
	virtual bool Parse(CIOStream &Stream, INT32 nX, INT32 nY, INT32 nAbort, bool &bVal);
		/** 
		 * UnParse the tree to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		nX			X coordinate of cell to terminate unparsing on.
		 * @param		nY			Y coordinate of cell to terminate unparsing on.
		 * @return      bool	NCS_SUCCESS, or Error code on failure.
		 */
	virtual bool UnParse(CIOStream &Stream, INT32 nX, INT32 nY, INT32 nThreshold);
	virtual bool SetValue(INT32 nX, INT32 nY, INT32 value);

		/** 
		 * Dump the tag tree.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual bool Dump();
protected:
    class Node {
    public:
        Node();

		void Dump(void); 

	    Node *m_pParent;

	    UINT16 m_nValue;
	    UINT16 m_nState; // lower bound on the value associated with this node

	    bool m_bKnown;
    };

protected:
	std::vector<Node> m_Nodes;

	UINT32  m_nWidth;
	UINT32  m_nHeight;
	UINT16	m_nLevels;
};

}
}

#endif // NCSJPCTAGTREE_H
