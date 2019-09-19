#pragma once

#include "NCSErrors.h"

typedef void *pNCSnetClient;
typedef void NCSnetPacketRecvCB(void *, INT32, pNCSnetClient, void *, NCSError eError);	 /**[06]**/	// pPacket, nLength, pClient, pUserData, eError
typedef void NCSnetPacketSentCB(NCSError, void *, void *);					// eError, pPacket, pUserData

typedef int(*NCScnetWriteCallback)(void *szCurrent,
	unsigned long nSize,
	void *szAllData,
	void *pUserData);