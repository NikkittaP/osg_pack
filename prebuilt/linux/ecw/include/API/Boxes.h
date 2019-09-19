#ifndef NCS_API_BOXES_H
#define NCS_API_BOXES_H

#ifndef SWIG
#include "NCSDefs.h"
#else
#define NCSECW_IMPEX
#endif
#include "NCSError.h"
namespace NCS {
	namespace API {
		class CBox;
		class CBoxesContext;
		/**
		* \class	NCS::API::CBoxes
		*
		* \brief	Box information of ECW or JP2 file.
		* \ingroup	simplifiedclasses
		*
		* \author	Intergraph Perth
		* \date	13/01/2016
		* Closing underlying decoder will make CBoxes invalid. 
		*/
		class NCSECW_IMPEX CBoxes {
			CBoxesContext *m_pContext;
			friend class CDecoderBase;
		public:
			CBoxes();
			virtual ~CBoxes();
			/**
			* \fn	CError CBoxes::GetBox(int number, CBox *pBox) const;
			*
			* \brief	Gets box by number
			* Note: CBox returned will be only valid as long as underlying Decoder is open. 
			* \author	Intergraph Perth
			* \date	13/01/2016
			* \param [in]	number	Box number. Must be smaller than number of boxes returned by CBoxes::GetNumberOfBoxes().
			* \param [in,out] pBox Box that will be returned. 
			* \return	CError representing error condition or success.
			*/
			CError GetBox(int number, CBox *pBox) const;
			/**
			* \fn	int CBoxes::GetNumberOfBoxes() const;
			*
			* \brief	Gets number of boxes
			*
			* \author	Intergraph Perth
			* \date	13/01/2016
			* \param [in]	number	Box number. Must be smaller than number of boxes returned from CBoxes.
			* \param [in,out] pBox Box that will be returned.
			* \return	CError representing error condition or success.
			*/
			int GetNumberOfBoxes() const;
		};
	}
}
#endif