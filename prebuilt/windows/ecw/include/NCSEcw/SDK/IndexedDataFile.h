/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	IndexedDataFile.h
** CREATED:	01/03/2012
** PURPOSE:	Internal Datafile for reassembling data
**
********************************************************/

#include "NCSFileIOStream.h"
#include "NCSMutex.h"
#include "NCSEcw/SDK/Buffer2D.h"

namespace NCS {

class CHashKey;

namespace SDK { 

/**
 * \class	CIndexedDataFile
 *
 * \brief	Indexed data file.  A class that facilitates out of order serialisation Blocks / Packets
 * 			that are compressed out-of-order and then can reassemble the data into the correct sequence.
 **/
class NCSECW_IMPEX CIndexedDataFile
{
public:
	/**
	 * \var	CIndexedDataFile::sm_nSignature
	 *
	 * \brief	The signature (preamble) of the file on disk
	 **/
	static UINT32 sm_nSignature;

	/**
	 * \var	CIndexedDataFile::sm_nBuffer2DSignature
	 *
	 * \brief	The signature (preamble) of a buffer2d type in the index file
	 **/
	static UINT16 sm_nBuffer2DSignature;

	/**
	 * \var	CIndexedDataFile::sm_nVersion
	 *
	 * \brief	The version of this version
	 **/
	static UINT8 sm_nVersion;

	/**
	 * \enum	CIndexedDataFile::Order
	 *
	 * \brief	The order of the Block index table. 
	 * \todo	Not currently implemented
	**/
	typedef enum {
		Ascending,
		Descending
	} Order;

	/**
	 * \fn	CIndexedDataFile::CIndexedDataFile();
	 *
	 * \brief	Default constructor.
	**/
	CIndexedDataFile();

	/**
	 * \fn	virtual CIndexedDataFile::~CIndexedDataFile();
	 *
	 * \brief	Destructor.
	**/
	virtual ~CIndexedDataFile();

	/**
	 * \fn	bool CIndexedDataFile::Open(const NCS::CString &sFileName, UINT64 nItems, Order eOrder,
	 * 		bool bExclusiveIO);
	 *
	 * \brief	Opens a Index file for rw.  If the file exists on disk it will attempt to parse the header, 
	 * 			if the header values match the parameters, and it is big enough to house the block index table, 
	 * 			the file will open successfully.  Also, blocks that don't match the MD5 hash in the block index
	 * 			table will be erased on open.
	 *
	 * \param	sFileName   	Filename of the file.
	 * \param	nItems			The number of items to handle.
	 * \param	eOrder			The order.
	 * \param	bExclusiveIO	true so that no other process can open the file
	 * \param	bValidateIfExists	(optional)	If openning an existing file, check that the file is not corrupt, this is a slow process for large files.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Open(const NCS::CString &sFileName, UINT64 nItems, Order eOrder, bool bExclusiveIO, bool bValidateIfExists = true);

	/**
	 * \fn	bool CIndexedDataFile::Close(bool bDelete = false);
	 *
	 * \brief	Closes the indexed data file.
	 *
	 * \param	bDelete	(optional) the deletes the file.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Close(bool bDelete = false);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, void *pData, UINT32 nSize);
	 *
	 * \brief	Writes a block to the end of the file and updates the block index table with the 
	 * 			offset, length and MD5 hash.
	 *
	 * \param	nID			 	The identifier.  Must be less than m_nItems
	 * \param [in,out]	pData	The data to write.
	 * \param	nSize		 	The size of the data to write.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, void *pData, UINT32 nSize);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const IEEE4 &fV);
	 *
	 * \brief	Helper function if you are indexing floats.
	 *
	 * \param	nID	The identifier.
	 * \param	fV 	The f v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const IEEE4 &fV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const IEEE8 &dV);
	 *
	 * \brief	Helper function if you are indexing double's.
	 *
	 * \param	nID	The identifier.
	 * \param	dV 	The d v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const IEEE8 &dV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const INT8 &nV);
	 *
	 * \brief	Helper function if you are indexing INT8's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const INT8 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const INT16 &nV);
	 *
	 * \brief	Helper function if you are indexing INT16's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const INT16 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const INT32 &nV);
	 *
	 * \brief	Helper function if you are indexing INT32's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const INT32 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const INT64 &nV);
	 *
	 * \brief	Helper function if you are indexing INT64's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const INT64 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const UINT8 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT8's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const UINT8 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const UINT16 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT16's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const UINT16 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const UINT32 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT32's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const UINT32 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const UINT64 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT64's.
	 *
	 * \param	nID	The identifier.
	 * \param	nV 	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const UINT64 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Write(const NCSBlockId nID, const NCS::SDK::CBuffer2D &Buffer);
	 *
	 * \brief	Writes a buffer2d to the datafile.  Does not preserve line stride in serialised data.
	 *
	 * \param	nID   	The identifier.
	 * \param	Buffer	The buffer.
	 * \param	bIsNULL	If this buffer is null and the data doesn't need to be written.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Write(const NCSBlockId nID, const NCS::SDK::CBuffer2D &Buffer, bool bIsNULL = false);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, void **pData, UINT32 &nSize,
	 * 		NCS::CHashKey *pHashKey = NULL);
	 *
	 * \brief	Reads a block of data from disk if it is available into a new buffer.  Caller
	 * 			is responsible for freeing memory allocated in this function.
	 *
	 * \param	nID			 	The identifier.  Must be less than m_nItems
	 * \param [in,out]	pData	The buffer to write into.  This will be alloc'd, caller must free.
	 * \param [in,out]	nSize	The size of the data.
	 * \param	pHashKey	 	(optional) [in,out] If non-null, the hash key.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, void **pData, UINT32 &nSize, NCS::CHashKey *pHashKey = NULL);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, void *pData, UINT32 nSize,
	 * 		UINT32 &nRead);
	 *
	 * \brief	Reads a block of data from disk into pre-alloced pData.
	 *
	 * \param	nID			 	The identifier.
	 * \param [in,out]	pData	If non-null, the data.
	 * \param	nSize		 	The size of pData.
	 * \param [in,out]	nRead	The number of bytes read.
	 * \param	pHashKey	 	(optional) [in,out] If non-null, the hash key.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, void *pData, UINT32 nSize, UINT32 &nRead, NCS::CHashKey *pHashKey = NULL);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, IEEE4 &fV);
	 *
	 * \brief	Helper function if you are indexing float's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	fV	The f v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, IEEE4 &fV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, IEEE8 &dV);
	 *
	 * \brief	Helper function if you are indexing double's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	dV	The d v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, IEEE8 &dV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, INT8 &nV);
	 *
	 * \brief	Helper function if you are indexing INT8's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, INT8 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, INT16 &nV);
	 *
	 * \brief	Helper function if you are indexing INT16's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, INT16 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, INT32 &nV);
	 *
	 * \brief	Helper function if you are indexing INT32's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, INT32 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, INT64 &nV);
	 *
	 * \brief	Helper function if you are indexing INT64's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, INT64 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, UINT8 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT8's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, UINT8 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, UINT16 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT16's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, UINT16 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, UINT32 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT32's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, UINT32 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, UINT64 &nV);
	 *
	 * \brief	Helper function if you are indexing UINT64's.
	 *
	 * \param	nID		  	The identifier.
	 * \param [in,out]	nV	The n v.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, UINT64 &nV);

	/**
	 * \fn	bool CIndexedDataFile::Read(const NCSBlockId nID, NCS::SDK::CBuffer2D &Buffer);
	 *
	 * \brief	Reads a serialised buffer2d from the indexed datafile and copies the intersecting 
	 * 			region into Buffer.  If buffer is not alloc'd, it will be alloc'd to the indexed
	 *			item's specification.
	 * 
	 * \see		NCS::SDK::CBuffer2D::Read
	 *
	 * \param	nID			  	The identifier.
	 * \param [in,out]	Buffer	The buffer.
	 * \param [in,out]	bIsNULL	If the buffer is null, this bool will be set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Read(const NCSBlockId nID, NCS::SDK::CBuffer2D &Buffer, bool &bIsNULL);

	/**
	 * \fn	bool CIndexedDataFile::IsComplete();
	 *
	 * \brief	Query if this object is complete.
	 *
	 * \return	true if all Items have been added.
	**/
	bool IsComplete();

	/**
	 * \fn	bool CIndexedDataFile::GetFirstEmptyIndex(NCSBlockId &nEmptyID);
	 *
	 * \brief	Gets the first empty index.
	 *
	 * \param [in,out]	nEmptyID	Identifier for the empty.
	 *
	 * \return	true if it succeeds, false if there are no empty items.
	**/
	bool GetFirstEmptyIndex(NCSBlockId &nEmptyID);

	/**
	 * \fn	bool CIndexedDataFile::GetNextEmptyIndex(const NCSBlockId nStartID, NCSBlockId &nEmptyID);
	 *
	 * \brief	Gets the next empty index.
	 *
	 * \param	nStartID			The start identifier.
	 * \param [in,out]	nEmptyID	Identifier for the empty.
	 *
	 * \return	true if it succeeds, false if there are no more empty items.
	**/
	bool GetNextEmptyIndex(const NCSBlockId nStartID, NCSBlockId &nEmptyID);

	/**
	 * \fn	bool CIndexedDataFile::Serialise(NCS::CIOStream &OutStream);
	 *
	 * \brief	Serialises the blocks the OutStream in sequential order.
	 *
	 * \param [in,out]	OutStream	Stream to write data to.
	 * \param bAllowNULLs	(optional)	If there are NULL blocks in the file don't serialise them and continue.
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Serialise(NCS::CIOStream &OutStream, bool bAllowNULLs = false);
protected:

	/**
	 * \fn	bool CIndexedDataFile::ParseHeader();
	 *
	 * \brief	Parse the header of a file from the stream.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool ParseHeader();

	/**
	 * \fn	bool CIndexedDataFile::UnParseHeader();
	 *
	 * \brief	Write the header of this new object to the stream.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool UnParseHeader();

	/**
	 * \fn	bool CIndexedDataFile::Validate();
	 *
	 * \brief	Zero-fill invalid items in the block index table.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Validate();

	/**
	 * \var	CIndexedDataFile::m_Stream
	 *
	 * \brief	The file stream to read and write data
	 **/
	NCS::CFileIOStream m_Stream;
	
	/**
	 * \var	CIndexedDataFile::m_sFileName
	 *
	 * \brief	The filename
	 **/
	NCS::CString m_sFileName;
	
	/**
	 * \var	CIndexedDataFile::m_nItems
	 *
	 * \brief	The number of items in the block index table
	 **/
	UINT64 m_nItems;
	
	/**
	 * \var	CIndexedDataFile::m_nOffsetToIndex
	 *
	 * \brief	The offset to the end of the header
	 **/
	UINT64 m_nOffsetToIndex;
	
	/**
	 * \var	CIndexedDataFile::m_nOffsetToData
	 *
	 * \brief	The offset to the end of the block index table
	 **/
	UINT64 m_nOffsetToData;
	
	/**
	 * \var	CIndexedDataFile::m_eOrder
	 *
	 * \brief	The order of the block index table
	 **/
	Order m_eOrder;
	
	/**
	 * \var	CIndexedDataFile::m_Mutex
	 *
	 * \brief	Synchronise this object in a multi-threaded environment
	 **/
	NCS::CMutex m_Mutex;
	
	/**
	 * \var	CIndexedDataFile::m_bExclusiveIO
	 *
	 * \brief	Boolean to flag if the stream is exclusive
	 **/
	bool m_bExclusiveIO;
	
	/**
	 * \var	CIndexedDataFile::m_bOpen
	 *
	 * \brief	Boolean to store whether the stream openned successfully
	 **/
	bool m_bOpen;
};

} // SDK
} // NCS

