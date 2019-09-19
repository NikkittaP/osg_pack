/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Buffer2D.h 
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CBuffer2D class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**	     [01] 12Dec05 tfl  lint fixes
**		 [02] 25Jan06 tfl  Added Multiply methods to support CFile autoscale feature
 *******************************************************/

#ifndef NCSJPCBuffer2D_H
#define NCSJPCBuffer2D_H

#ifndef NCSJPCRECT_H
#include "NCSEcw/SDK/Rect2D.h"
#endif // NCSJPCRECT_H

#ifndef NCSMISC_H
#include "NCSMisc.h"
#endif
#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif

#ifndef NCS_SDK_ATOMIC_H
#include "NCSEcw/SDK/Atomic.h"
#endif

#ifndef NCSEVENT_H
#include "NCSEvent.h"
#endif //NCSEVENT_H

#include <vector>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251 4127 4275)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#undef MAXUINT8
#undef MAXUINT16
#undef MAXUINT32
#undef MAXUINT64
#undef MAXINT8
#undef MAXINT16
#undef MAXINT32
#undef MAXINT64
#undef MINUINT8
#undef MINUINT16
#undef MINUINT32
#undef MINUINT64
#undef MININT8
#undef MININT16
#undef MININT32
#undef MININT64
#endif

namespace NCS {
namespace SDK {
	/**
	 * CBuffer2D class - the base JPC scanline buffer.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/11/28 $ 
	 * @note         When constructing a Buffer2D object by specifying the top-left and bottom-right corners, note that the first two points specify the top-left x,y corner of the rectangle (inclusive) and the third and fourth points specify the bottom-right x,y corner (exclusive). For example, a Buffer2D(0,0,2,2,BT_INT8) defines a 2x2 rectangle with valid indexes of 0, and 1 in both x and y directions.
	 */	
class NCSECW_IMPEX CBuffer2D: public CRect2D {
public:
	static const UINT32 HEAPDECODE = 1;
	static const UINT32 HEAPCOMPRESSION = 2;
	static const UINT32 HEAPDECODEJP2 = 1;
	static const UINT32 HEAPCOMPRESSIONJP2 = 2;
		/** Maximum UINT8 value */
	static const UINT32 MAXUINT8;
		/** Maximum UINT16 value */
	static const UINT32 MAXUINT16;
		/** Maximum UINT32 value */
	static const UINT32 MAXUINT32;
		/** Maximum UINT64 value */
	static const UINT64 MAXUINT64;
		/** Minimum UINT8 value */
	static const UINT32 MINUINT8;
		/** Minimum UINT16 value */
	static const UINT32 MINUINT16;
		/** Minimum UINT32 value */
	static const UINT32 MINUINT32;
		/** Minimum UINT64 value */
	static const UINT64 MINUINT64;
	
		/** Maximum INT8 value */
	static const UINT32 MAXINT8;
		/** Maximum INT16 value */
	static const UINT32 MAXINT16;
		/** Maximum INT32 value */
	static const UINT32 MAXINT32;
		/** Maximum INT64 value */
	static const UINT64 MAXINT64;
		/** Minimum INT8 value */
	static const INT32 MININT8;
		/** Minimum INT16 value */
	static const INT32 MININT16;
		/** Minimum INT32 value */
	static const INT32 MININT32;
		/** Minimum INT64 value */
	static const INT64 MININT64;
	
		/** Minimum IEEE4 value */
	static const IEEE4 MINIEEE4;
		/** Maximum IEEE4 value */
	static const IEEE4 MAXIEEE4;
		/** Minimum IEEE8 value */
	static const IEEE8 MINIEEE8;
		/** Maximum IEEE8 value */
	static const IEEE8 MAXIEEE8;

	typedef enum {
			/** s8 integer buffer type */
		BT_INT8		= NCSCT_INT8,
			/** u8 integer buffer type */
		BT_UINT8	= NCSCT_UINT8,
			/** s16 integer buffer type */
		BT_INT16	= NCSCT_INT16,
			/** u16 integer buffer type */
		BT_UINT16	= NCSCT_UINT16,
			/** s32 integer buffer type */
		BT_INT32	= NCSCT_INT32,
			/** u32 integer buffer type */
		BT_UINT32	= NCSCT_UINT32,
			/** s64 integer buffer type */
		BT_INT64	= NCSCT_INT64,
			/** u64 integer buffer type */
		BT_UINT64	= NCSCT_UINT64,
			/** IEEE4 single precision float buffer type */
		BT_IEEE4	= NCSCT_IEEE4,
			/** IEEE4 double precision float buffer type */
		BT_IEEE8	= NCSCT_IEEE8
	} Type;

		/** FLAG: This buffer owns resources */
	static const UINT32 AT_OWNER;	
		/** FLAG: A child node returned the buffer and will free it later */
	static const UINT32 AT_CHILD;	
		/** Buffer is locked and can't be assigned to. */
	static const UINT32 AT_LOCKED;	

		/** Generally either 16 or 32 depending on which CPU feature instruction extensions are present. */
	static int s_nMemoryAlignment;

		/** Buffer type */
	Type	m_eType;
		/** buffer flags */
	UINT32	m_Flags;

		/** Actual buffer pointer */
	void	*m_pBuffer;

		/** Buffer Stride size to next line */
	int		m_nStride;
	UINT32 m_nHeapNumber;
	UINT32 GetHeapNumber() const { return m_nHeapNumber; }
		/** Default constructor, initialises members */
	CBuffer2D();
		/** Copy constructor */
	CBuffer2D( const  CBuffer2D& src );
		/** constructor, specify rect values */
	CBuffer2D(UINT32 nWidth, Type eType, UINT32 nHeapNumber);
		/** constructor, specify rect values */
	CBuffer2D(UINT32 nWidth, UINT32 nHeight, Type eType, UINT32 nHeapNumber);
		/** constructor, specify rect values */
	CBuffer2D(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1, Type eType, UINT32 nHeapNumber);
		/** constructor, specify rect values */
	CBuffer2D(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight, Type eType, UINT32 nHeapNumber);
		/** Virtual destructor */
	virtual ~CBuffer2D();
	
		/** Assignment operator */
	CBuffer2D& operator=( const CBuffer2D& src );

		/** Equality operators */
	bool operator==( const CBuffer2D& s );
	bool operator!=( const CBuffer2D& s );

		/** Alloc a buffer the specified width and type */
	bool Alloc(UINT32 nWidth, Type eType, UINT32 nHeapNumber);
		/** Alloc a buffer the specified width, height and type */
	bool Alloc(UINT32 nWidth, UINT32 nHeight, Type eType, UINT32 nHeapNumber);
		/** Alloc a buffer with the specified TL, BR and type */
	bool Alloc(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1, Type eType, UINT32 nHeapNumber);
		/** Alloc a buffer with the specified TL, width, height and type */
	bool Alloc(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight, Type eType, UINT32 nHeapNumber);
		/** Alloc a buffer with the specified CRect and type */
	bool Alloc(CRect2D &rect, Type eType, UINT32 nHeapNumber);
		/** Free the buffer */
	void Free();

		/** Assign a buffer to this buffer */
	bool Assign(Type eType, void *pPtr, UINT32 nHeapNumber,INT32 nStride = CBuffer2D::MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(UINT32 nWidth, Type eType, void *pPtr, UINT32 nHeapNumber, INT32 nStride = MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(UINT32 nWidth, UINT32 nHeight, Type eType, void *pPtr, UINT32 nHeapNumber, INT32 nStride = MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1, Type eType, void *pPtr, UINT32 nHeapNumber, INT32 nStride = MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight, Type eType, void *pPtr, UINT32 nHeapNumber, INT32 nStride = CBuffer2D::MININT32, bool bFree = true);
	bool Assign(const CRect2D &rect, Type eType, void *pPtr, UINT32 nHeapNumber, INT32 nStride = CBuffer2D::MININT32, bool bFree = true) {
					return(Assign(rect.GetX0(), rect.GetY0(), rect.GetX1(), rect.GetY1(), eType, pPtr,nHeapNumber, nStride, bFree));
				}
		/** Release this buffer */
	void Release();

		/** Set flags */
	void SetFlag(UINT32 Flag) { m_Flags |= Flag; };
		/** Unset flags */
	void UnSetFlags(UINT32 Flags) { m_Flags &= ~Flags; };
		/** Get flags */
	UINT32 GetFlags() { return(m_Flags); };
		/** test flags */
	bool TestFlags(UINT32 Flag) const { return((m_Flags & Flag) ? true : false); };

		/** 
		 * Return the size of a cell in bytes
		 * @param		eType		Type of cell
		 * @return      UINT32		calculated buffer size in bytes.
		 */
	static NCS_INLINE UINT32 Size(Type eType) {
			switch(eType) {
				case BT_INT8:
					return(sizeof(INT8));
				case BT_UINT8:
					return(sizeof(UINT8));
				case BT_INT16:
					return(sizeof(INT16));
				case BT_UINT16:
					return(sizeof(UINT16));
				case BT_INT32:
					return(sizeof(INT32));
				case BT_UINT32:
					return(sizeof(UINT32));
				case BT_INT64:
					return(sizeof(INT64));
				case BT_UINT64:
					return(sizeof(UINT64));
				case BT_IEEE4:
					return(sizeof(IEEE4));
				case BT_IEEE8: 
					return(sizeof(IEEE8));
			}
			return(0);
		}
		/** 
		 * Return the size of a buffer in bytes (default 1 cell).
		 * @param		eType		Type of buffer
		 * @param		nWidth		Number of pixels in buffer.
		 * @return      UINT32		calculated buffer size in bytes.
		 */
	static NCS_INLINE UINT32 Size(Type eType, UINT32 nWidth, UINT32 nHeight = 1) {
		return(NCSCeilDiv(Size(eType) * nWidth, CBuffer2D::s_nMemoryAlignment) * CBuffer2D::s_nMemoryAlignment * nHeight);
	}
		/** Get buffer Stride in cells */

	NCS_INLINE UINT32 GetStride() const { return(m_nStride); };
		/** Get buffer Stride in bytes */
	NCS_INLINE UINT32 GetStrideBytes() const { return(m_nStride * Size(m_eType)); };
		/** get buffer type */
	NCS_INLINE Type GetType() const { return(m_eType); };
		/** get pointer to start of buffer (0,0) */
	NCS_INLINE void *GetPtr() { return(m_pBuffer); };
		/**	get pointer to buffer at (x,y) */
	NCS_INLINE void *GetPtr(INT32 x, INT32 y) const { 
		return((UINT8*)m_pBuffer + (y - GetY0()) * (INT32)GetStrideBytes() + (x - GetX0()) * (INT32)Size(GetType())); };

		/** 
		 * Return the size of the Wavelet Tranform border for the specified pipeline type.
		 * @param		eType		Type of buffer
		 * @return      UINT32		Width/Height of border in CELLS.
		 */
	static NCS_INLINE UINT32 Border(Type eType) {
			switch(eType) {
				case BT_INT8:
				case BT_UINT8:
				case BT_INT16:
				case BT_UINT16:
				case BT_INT32:
				case BT_UINT32:
				case BT_INT64:
				case BT_UINT64:
						return(1);
					//break;
				case BT_IEEE4:
				case BT_IEEE8: 
						return(2);
					//break;
			}
			return(0);
		}

		/** Clear buffer to 0 */
	void Clear();
	static bool Clear(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, Type eType);
	bool Clear(CRect2D &clip);
		/** 
		 * Convert a line to a line of type T.
		 * @param		pDst		Pointer to Destination buffer
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Convert(UINT8 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT8, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(UINT16 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT16, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(UINT32 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT32, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(UINT64 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT64, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT8 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT8, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT16 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT16, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT32 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT32, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT64 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT64, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(IEEE4 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_IEEE4, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(IEEE8 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_IEEE8, nStep, m_pBuffer, (NCSCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}

		/** 
		 * Read buffer to a type T.
		 * @param		pDst		Pointer to Destination buffer
		 * @return      bool		true on succes, else false.
		 */
	template <class T> bool Read(T *pDst, UINT8 nStep = 1, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
			for(UINT32 y = 0; y < GetHeight(); y++) {
				Convert(pDst, nStep, bCanDoScaling, nScaleRatio);
			}
			return(true);
		}

	template <class DT, class ST> bool Read(DT *pDst, UINT8 nStep, ST SrcMin, ST SrcMax, DT DstMin, DT DstMax) const {
			switch(GetType()) {
				case BT_INT8:
					return Read<DT, ST, INT8>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_UINT8:
					return Read<DT, ST, UINT8>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_INT16:
					return Read<DT, ST, INT16>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_UINT16:
					return Read<DT, ST, UINT16>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_INT32:
					return Read<DT, ST, INT32>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_UINT32:
					return Read<DT, ST, UINT32>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_INT64:
					return Read<DT, ST, INT64>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_UINT64:
					return Read<DT, ST, UINT64>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_IEEE4:
					return Read<DT, ST, IEEE4>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_IEEE8: 
					return Read<DT, ST, IEEE8>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				default:
					break;
			}
			return(false);
		}

	NCS_INLINE bool Read(CBuffer2D *pDst, CRect2D &clip) const {
			CBuffer2D tmp;
			CRect2D intersect = clip & *pDst;
			tmp.Assign(intersect,
					   pDst->GetType(), 
					   pDst->GetPtr(intersect.GetX0(), intersect.GetY0()), 
					   pDst->GetHeapNumber(),
					   pDst->GetStride());
			return(Read(&tmp));
		}
		/** Read from the buffer to the destination buffer, copying only the intersecting area */
	NCS_INLINE bool Read(CBuffer2D *pDst) const {
			CRect2D r(*this & *pDst);

			if(pDst->GetType() == GetType()) {

				return(Copy(r.GetWidth(), r.GetHeight(), 
							pDst->GetPtr(r.GetX0(), r.GetY0()),
							pDst->GetStrideBytes(), 
							GetPtr(r.GetX0(), r.GetY0()),
							GetStrideBytes(), 
							m_eType));
			} else {
				for(UINT32 y = 0; y < r.GetHeight(); y++) {
					CBuffer2D tmp;
					tmp.Assign(r.GetX0(), r.GetY0(), r.GetWidth(), (UINT32)1, GetType(), GetPtr(r.GetX0(), r.GetY0()+y),GetHeapNumber(), GetStride());
					switch(pDst->GetType()) {
						case BT_INT8:
								tmp.Convert((INT8*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_UINT8:
								tmp.Convert((UINT8*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_INT16:
								tmp.Convert((INT16*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_UINT16:
								tmp.Convert((UINT16*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_INT32:
								tmp.Convert((INT32*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_UINT32:
								tmp.Convert((UINT32*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_INT64:
								tmp.Convert((INT64*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_UINT64:
								tmp.Convert((UINT64*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_IEEE4:
								tmp.Convert((IEEE4*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
						case BT_IEEE8:
								tmp.Convert((IEEE8*)pDst->GetPtr(r.GetX0(), r.GetY0()+y), 1);
							break;
					}
					tmp.Release();
				}
			}
			return(true);
		}

		/** 
		 * Clip valus in the buffer to the specified range.
		 * @param		nMin		Min value
		 * @param		nMax		Max value
		 * @return      bool		true on success, else false & error set.
		 */
	bool Clip(INT64 nMin, INT64 nMax);
		/** 
		 * Clip valus in a buffer to the specified range.
		 * @param		fMin		Min value
		 * @param		fMax		Max value
		 * @return      bool		true on success, else false & error set.
		 */
	bool Clip(IEEE4 fMin, IEEE4 fMax);
		/** 
		 * Add a constant value to the buffer.
		 * @param		nValue		Constant value to add
		 * @param		nScale		Scale factor, *= NCS2Pow(-nScale)
		 * @return      bool		true on succes, else false.
		 */
	bool Add(const INT64 nValue, INT16 nScale = 0);
		/** 
		 * Add a constant value to the line.
		 * @param		fValue		Constant value to add
		 * @param		nScale		Scale factor, *= fScale
		 * @return      bool		true on succes, else false.
		 */
	bool Add(const IEEE4 fValue, INT16 nScale = 0);
		/** 
		 * Change bit depth (data range) of a buffer
		 * @param		nCurrentBits	Current bit depth
		 * @param		nNewBits		New bit depth
		 * @return      bool		true on succes, else false.
		 */
	bool OffsetShift(const INT64 nValue, UINT8 nCurrentBits, UINT8 nNewBits);
		/**
		 * Multiply the contents of the buffer by an integer.
		 * @param		nValue		Multiplication factor
		 * @return		bool		true on success, else false.
		 */
	bool Multiply(const IEEE4 fValue);
		/**
		 * Multiply the contents of the buffer by an integer.
		 * @param		nValue		Multiplication factor
		 * @return		bool		true on success, else false.
		 */
	bool Multiply(const INT32 nValue);
		/**
		 * Separate the contents of the buffer into low and hi values 
		 * @param		nLowValue	Low Value to set it to if less than Mid Value
		 * @param		nMidValue	MidValue 
		 * @param		nHighValue	High Value to set it to if greater than than Mid Value
		 * @return		bool		true on success, else false.
		 */
	//bool Separate(INT16 nLowValue, INT16 nMidValue, INT16 nHighValue, NCSCellType eSrcType);
	bool Separate(INT64 nLowValue, INT64 nMidValue, INT64 nHighValue, NCSCellType eSrcType);

	/**
	 * \fn	bool CBuffer2D::Set(UINT8 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The UINT8 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(UINT8 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(INT8 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The INT8 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(INT8 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(UINT16 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The UINT16 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(UINT16 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(INT16 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The INT16 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(INT16 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(UINT32 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The UINT32 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(UINT32 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(INT32 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The INT32 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(INT32 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(UINT64 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The UINT64 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(UINT64 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(INT64 nValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	nValue	The INT64 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(INT64 nValue);

	/**
	 * \fn	bool CBuffer2D::Set(IEEE4 fValue);
	 *
	 * \brief	Sets the the buffer to specified value.  The value will be cast into the buffer datatype with no rounding.
	 *
	 * \param	fValue	The IEEE4 to set.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	bool Set(IEEE4 fValue);
	/** 
		 * Copy from one pointer to another (may overlap).
		 * @param		nWidth		Width in pixels.
		 * @param		nHeight		Height in pixels.
		 * @param		pDst		Dest buffer
		 * @param		nDstStep	Dest buffer step size
		 * @param		pSrc		Src buffer
		 * @param		nSrcStep	Src buffer step size
		 * @param		eType		Buffer Type
		 * @return      bool		true on success, else false & error set.
		 */
	static bool Copy(UINT32 nWidth, UINT32 nHeight, void *pDst, INT32 nDstStride, const void *pSrc, INT32 nSrcStride, Type eType);
	static bool CopyFlip(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, const void *pSrc, int nSrcStride, Type eType, bool bVertical);
	static bool CopyMultiply(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, const void *pSrc, int nSrcStride, Type eType, const INT64 nValue);
	static bool CopyShift(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, const void *pSrc, int nSrcStride, Type eType, const int nShift);

	static bool ConvType(void *pDst, Type eDstType, UINT8 nDstStep, void *pSrc, Type eSrcType, UINT8 nSrcStep, UINT32 nWidth, bool bCanDoScaling = false, UINT8 nScaleRatio = 2);
	static bool ConvType(void *pDst, NCSCellType eDstType, UINT8 nDstStep, void *pSrc, NCSCellType eSrcType, UINT8 nSrcStep, UINT32 nWidth, bool bCanDoScaling = false, UINT8 nScaleRatio = 2);
	
	static Type GetTypeFromCellType(NCSCellType eType);
	
	static void sPurgeFreeQueue();

	static void Init();
	static void Fini();
private:
	bool Alloc(UINT32 nHeapNumber);

	template <class DT, class ST, class BT> bool Read(DT *pDst, UINT8 nStep, ST SrcMin, ST SrcMax, DT DstMin, DT DstMax) const {
		DT *pD = pDst;
		IEEE4 nF = DstMax - DstMin;
		nF /= (SrcMax - SrcMin);
		for(INT32 y = GetY0(); y < GetY1(); y++) {
			BT *pS = (BT*)GetPtr(GetX0(), y);
			for(UINT32 x = 0; x < GetWidth(); x++) {
				//*pD = (DT)(((pS[x] - SrcMin) * DstMax) / (SrcMax - SrcMin));
				*pD = MIN(MAX((DT)((pS[x] - SrcMin) * nF), DstMin), DstMax);
				pD += nStep;
			}
		}
		return true;
	}

public:
	NCS::SDK::CAtomicUnsignedInt64 *m_pMemoryCounter;

	static NCS::SDK::CAtomicUnsignedInt64 &GetDecompressionCounter();
	static NCS::SDK::CAtomicUnsignedInt64 &GetCompressionCounter();

		/** Buffer copy US */
	static NCS::SDK::CAtomicInt64			sm_usCopy;
		/** buffer copy bytes */
	static NCS::SDK::CAtomicUnsignedInt64	sm_nCopyBytes;
		/** buffer clear US */
	static NCS::SDK::CAtomicInt64			sm_usClear;
		/** buffer clear bytes */
	static NCS::SDK::CAtomicUnsignedInt64	sm_nClearBytes;
		/** buffer Alloc US */
	static NCS::SDK::CAtomicInt64			sm_usAlloc;
		/** Decompression buffer alloc bytes */
	static NCS::SDK::CAtomicUnsignedInt64	sm_nAllocDecompressionBytes;
	static NCS::SDK::CAtomicUnsignedInt64	sm_nAllocCompressionBytes;
		/** buffer free US */
	static NCS::SDK::CAtomicInt64			sm_usFree;
};

typedef std::vector<CBuffer2D> CBuffer2DVector;

class NCSECW_IMPEX CBuffer2DCache {
public:
	virtual ~CBuffer2DCache() {};
	virtual void SetBuffers(UINT32 nBuffers);
	virtual CBuffer2D *GetBuffer(UINT32 nBuffer);
	virtual CBuffer2D *FindBuffer(UINT32 &nBuffer, CRect2D &r, bool &bFound, CBuffer2D::Type eType, bool bCompressionMemory = false);
private:
	CBuffer2DVector m_Buffers;
};

class NCSECW_IMPEX CBuffer2DQueue: public std::vector<CBuffer2DVector*>, public CEvent, public CMutex {
public:
	CBuffer2DQueue();
	virtual ~CBuffer2DQueue();
	virtual CError Alloc(UINT32 nLength, UINT32 nBuffers);
	virtual CError Free();
	virtual CBuffer2DVector *NextBuffer(int nWaitForMS = -1);
	virtual void PushBuffer(CBuffer2DVector *pBuffer);
	// added to stop infinifte loop, if writer is just cancelled or got error
	void SetCancel(bool bCancel) {m_bCancel = bCancel;};
private:
	// added to stop infinifte loop, if writer is just cancelled or got error
	bool m_bCancel;
};
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // !NCSJPCBuffer2D_H
