/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSArray.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Memory handling routines.
** EDITS:
 *******************************************************/

#ifndef NCSARRAY_H
#define NCSARRAY_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef NCSMALLOC_H
#include "NCSMalloc.h"
#endif

/*
** Set to cause arrays to grow/shrink by a granular factor instead of for each element.
*/
#define NCS_ARRAY_GRANULAR

#ifdef NCS_ARRAY_GRANULAR

/*
** Granular array factor (nelements to grow/shrink)
*/
#define NCS_ARRAY_GRANULARITY 16
		/*
		((void *)pArray) = (void *)NCSRealloc((void *)(pArray),	\
			((nElements) + NCS_ARRAY_GRANULARITY) *		\
			sizeof(*(pArray)),				\
			FALSE);						\
		*/

#define NCSArrayInsertElementType(T, pArray, nElements, nIndex, pElement)	\
	if(nElements % NCS_ARRAY_GRANULARITY == 0) {			\
		void *pData = (void *)NCSRealloc((void *)(pArray),	\
			((nElements) + NCS_ARRAY_GRANULARITY) *		\
			sizeof(*(pArray)),				\
			FALSE);						\
		memcpy(&(pArray), &pData, sizeof(pArray));	\
	}								\
	if((nIndex) < (nElements)) {					\
		memmove(&((pArray)[(nIndex) + 1]),			\
			&((pArray)[(nIndex)]),				\
			((nElements) - (nIndex)) * sizeof(*(pArray)));	\
	}								\
	if(pElement) {							\
		/*(pArray)[(nIndex)] = *(pElement);*/			\
		memcpy(&((pArray)[(nIndex)]), (pElement), sizeof(*(pElement))); \
	} else { 							\
		memset(&((pArray)[(nIndex)]), 0, sizeof(*pArray));	\
	}								\
	(nElements) += 1

#define NCSArrayAppendElementType(T, pArray, nElements, pElement)		\
	NCSArrayInsertElementType(T, pArray, nElements, nElements, pElement)

#define NCSArrayRemoveElementType(T, pArray, nElements, nIndex)		\
	if((nIndex) < (nElements) - 1) {				\
		memmove(&((pArray)[(nIndex)]),				\
			&((pArray)[(nIndex) + 1]),			\
			((nElements) - (nIndex) - 1) * sizeof(*(pArray)));	\
	}								\
	(nElements) -= 1;						\
	if(nElements % NCS_ARRAY_GRANULARITY == 0) {			\
		if((nElements) > 0) {					\
			void *pData = (void *)NCSRealloc((pArray),	\
				(nElements) * sizeof(*(pArray)),	\
				FALSE);					\
			memcpy(&(pArray), &pData, sizeof(pArray));	\
		} else {						\
			NCSFree((pArray));				\
			/*pArray = (void*)NULL;*/				\
			memset(&(pArray), 0, sizeof(pArray));	\
		}							\
	}

#else /* NCS_ARRAY_GRANULAR */

#define NCSArrayInsertElementType(T, pArray, nElements, nIndex, pElement)	\
				(pArray) = (T*)NCSRealloc((pArray),										\
									  (nElements + 1) * sizeof(*(pArray)),			\
									  TRUE);										\
				if((nIndex) < (nElements)) {										\
					memmove(&((pArray)[(nIndex) + 1]),								\
							&((pArray)[(nIndex)]),									\
							((nElements) - (nIndex)) * sizeof(*(pArray)));			\
				}																	\
				if(pElement) {														\
					(pArray)[(nIndex)] = *(pElement);								\
				}																	\
				(nElements) += 1

#define NCSArrayAppendElementType(T, pArray, nElements, pElement)							\
				NCSArrayInsertElementType(T, pArray, nElements, nElements, pElement)

#define NCSArrayRemoveElementType(T, pArray, nElements, nIndex)							\
				if((nIndex) < (nElements) - 1) {									\
					memmove(&((pArray)[(nIndex)]),									\
						    &((pArray)[(nIndex) + 1]),								\
							((nElements) - (nIndex) - 1) * sizeof(*(pArray)));		\
				}																	\
				(nElements) -= 1;													\
				if((nElements) > 0) {												\
					(pArray) = (T*)NCSRealloc((pArray),									\
										  (nElements) * sizeof(*(pArray)),			\
										  FALSE);									\
				} else {															\
					NCSFree((pArray));												\
					(pArray) = (T*)NULL;											\
				}

#endif /* NCS_ARRAY_GRANULAR */

#define NCSArrayInsertElement(pArray, nElements, nIndex, pElement) NCSArrayInsertElementType(void, pArray, nElements, nIndex, pElement)
#define NCSArrayAppendElement(pArray, nElements, pElement) NCSArrayAppendElementType(void, pArray, nElements, pElement)
#define NCSArrayRemoveElement(pArray, nElements, nIndex) NCSArrayRemoveElementType(void, pArray, nElements, nIndex)

#ifdef __cplusplus
}
#endif

#endif /* NCSARRAY_H */
