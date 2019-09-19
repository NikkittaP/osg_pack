#ifndef NCS_API_BOX_H
#define NCS_API_BOX_H

#ifndef SWIG
#include "NCSDefs.h"
#else
#define NCSECW_IMPEX
#endif
#include "NCSError.h"

namespace NCS {
	namespace API {
		class CBoxContext;
		/**
		* \class	NCS::API::CBox
		*
		* \brief	Box information of ECW or JP2 file.
		* \ingroup	simplifiedclasses
		*
		* \author	Intergraph Perth
		* \date	12/01/2016
		* Closing underlying decoder will make CBox invalid.
		*/
		class NCSECW_IMPEX CBox {
			friend class CBoxes;
			CBoxContext *m_pContext;
		public:
			CBox();
			virtual ~CBox();
			/**
			* \fn	UINT32 CBox::GetId() const;
			*
			* \brief	Gets id of this box
			* \author	Intergraph Perth
			* \date	13/01/2016
			* \return	Id of the box
			*/
			UINT32 GetId() const;
			/**
			* \fn	UINT64 CBox::GetLength() const;
			*
			* \brief	Gets id as string
			* \author	Intergraph Perth
			* \date	13/01/2016
			* \param [in,out] pBuffer  string representation of id will be copied here. Buffer must be at least 5 bytes.
			*/
			UINT64 GetLength() const ;
			/**
			* \fn	CError CBox::GetData(void *pBuffer) const;
			*
			* \brief	Gets actuall data of the box
			* \author	Intergraph Perth
			* \date	13/01/2016
			* \param [in,out] pBuffer  buffer that will be returned.
			* \return	Id of the box
			*/
			CError GetData(void *pBuffer) const;
		};
	}
}
#endif