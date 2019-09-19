/******************************************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE: Config.h
** CREATED: <20/08/2013>
** PURPOSE: Config header file of ECW JPEG 2000 SDK
**
******************************************************************************/

#pragma once

#ifndef SWIG
#include "NCSDefs.h"
#else
#define NCSECW_IMPEX
#endif

namespace NCS {
namespace API {

/**
* \class	NCS::API::CMemoryCounters
*
* \brief	A class to query the SDK's memory counters.
* \ingroup	simplifiedclasses
*
* \author	Intergraph Perth
* \date	21/02/2018
*/
class NCSECW_IMPEX CMemoryCounters {
public:
	CMemoryCounters();
	CMemoryCounters(const CMemoryCounters &s);
	virtual ~CMemoryCounters();

#ifndef SWIG
	const CMemoryCounters &operator=(const CMemoryCounters &s);
#endif

	/**
	* \fn	virtual int CMemoryCounters::GetAllBytes();
	*
	* \brief	Gets the sum of all bytes counted by the SDK.  
	* 
	* This includes all buffers and class structures in caches but not transient bytes or stack memory
	*
	* \author	Intergraph Perth
	* \date	21/02/2018
	*
	* \return	Total counted memory in bytes.
	*/
	UINT64 GetAllBytes();

	/**
	* \fn	virtual int CMemoryCounters::GetDecoderBufferBytes();
	*
	* \brief	Gets the total bytes for all decoder buffers.
	*
	* \author	Intergraph Perth
	* \date	21/02/2018
	*
	* \return	Value in bytes.
	*/
	UINT64 GetDecoderBufferBytes();

	/**
	* \fn	virtual int CMemoryCounters::GetEncoderBufferBytes();
	*
	* \brief	Gets the total bytes for all encoder buffers.
	*
	* \author	Intergraph Perth
	* \date	21/02/2018
	*
	* \return	Value in bytes.
	*/
	UINT64 GetEncoderBufferBytes();

	/**
	* \fn	virtual int CMemoryCounters::GetECWStructureBytes();
	*
	* \brief	Gets the total bytes for all ECW class structures.
	*
	* \author	Intergraph Perth
	* \date	21/02/2018
	*
	* \return	Value in bytes.
	*/
	UINT64 GetECWStructureBytes();

	/**
	* \fn	virtual int CMemoryCounters::GetJP2StructureBytes();
	*
	* \brief	Gets the total bytes for all JP2 class structures.
	*
	* \author	Intergraph Perth
	* \date	21/02/2018
	*
	* \return	Value in bytes.
	*/
	UINT64 GetJP2StructureBytes();
};

}
}