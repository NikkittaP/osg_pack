/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	NCSECWCompatibility.h
** CREATED:	01/03/2013
** PURPOSE:	Public client backwards compatibility defines
**
********************************************************/

#ifndef NCS_ECW_COMPATIBILITY_H
#define NCS_ECW_COMPATIBILITY_H

// NCSECWClient.h
#define NCSecwInit NCSInit
#define NCSecwShutdown NCSShutdown
#define NCSecwSetIOCallbacks NCSSetIOCallbacks
#define NCSecwNetBreakdownUrl NCSNetBreakdownUrl
#define NCSecwNetBreakdownUrlW NCSNetBreakdownUrlW
#define NCScbmOpenFileView NCSOpenFileView
#define NCScbmOpenFileViewW NCSOpenFileViewW
#define NCScbmCloseFileView NCSCloseFileView
#define NCScbmCloseFileViewEx NCSCloseFileViewEx
#define NCSGetSetViewInfo NCSGetViewInfo
#define NCScbmGetViewFileInfo NCSGetViewFileInfo
#define NCScbmGetViewFileInfoEx NCSGetViewFileInfo
#define NCScbmGetViewInfo NCSGetViewInfo
#define NCScbmSetFileView NCSSetFileView
#define NCScbmSetFileViewEx NCSSetFileViewEx
#define NCScbmReadViewLineBIL NCSReadViewLineBIL
#define NCScbmReadViewLineBILEx NCSReadViewLineBILEx
#define NCScbmReadViewLineRGB NCSReadViewLineRGB
#define NCScbmReadViewLineBGR NCSReadViewLineBGR
#define NCScbmReadViewLineRGBA NCSReadViewLineRGBA
#define NCScbmReadViewLineBGRA NCSReadViewLineBGRA
#define NCScbmReadViewFake NCSReadViewFake
#define NCSecwSetConfig NCSSetConfig
#define NCSecwGetConfig NCSGetConfig
#define NCScbmGetFileType NCSGetFileType
#define NCScbmGetFileMimeType NCSGetFileMimeType
#define NCSInitFileInfoEx NCSInitFileInfo
#define NCSFreeFileInfoEx NCSFreeFileInfo
#define NCSCopyFileInfoEx NCSCopyFileInfo
#define NCSEcwInitStatistics NCSInitStatistics
#define NCSEcwInitStatisticsDefault NCSInitStatisticsDefault
#define NCSEcwInitStatisticsHistograms NCSInitStatisticsHistograms
#define NCSEcwFreeStatistics NCSFreeStatistics
#define NCSEcwCopyStatistics NCSCopyStatistics
#define NCSEcwInitMetaData NCSInitMetaData
#define NCSEcwCopyMetaData NCSCopyMetaData
#define NCSEcwFreeMetaData NCSFreeMetaData
#define NCScbmGetViewRPCData NCSGetViewRPCData
#define NCSEcwCopyRPCData NCSCopyRPCData
#define NCSEcwFreeRPCData NCSFreeRPCData
#define NCSEcwGetAllGeotiffKeyEntries NCSGetAllGeotiffKeyEntries
#define NCSEcwGetGeotiffKey NCSGetGeotiffKey
#define NCScbmGetViewStatistics NCSGetViewStatistics
#define NCSEcwGetGeotiffTag NCSGetGeotiffTag
#define NCSEcwGetGeotiffKey NCSGetGeotiffKey
#define NCSFileViewInfoEx NCSFileInfo
#define NCSFileViewInfo NCSFileInfo
#define NCSEcwReadStatus NCSReadStatus
#define CellSizeUnits NCSCellSizeUnitType
#define NCSEcwConfigType NCSConfigType
#define GeodataUsage NCSGeodataUsage
#define NCSFileViewFileInfo NCSFileInfo
#define NCSFileViewFileInfoEx NCSFileInfo
#define NCSFileViewSetInfo NCSSetViewInfo

#define NCSECW_READ_OK NCS_READ_OK
#define NCSECW_READ_FAILED NCS_READ_FAILED
#define NCSECW_READ_CANCELLED NCS_READ_CANCELLED

// NCSTypes.h
#define NCSEcwCellType NCSCellType

// NCSECWHeaderEditor.h
#define NCSEcwEditReadInfo NCSReadEditInfo
#define NCSEcwEditWriteInfo NCSWriteEditInfo
#define NCSEcwEditFreeInfo NCSFreeEditInfo
#define NCSEcwEditCopyInfo NCSCopyEditInfo

#define NCSEditReadInfo NCSReadEditInfo
#define NCSEditWriteInfo NCSWriteEditInfo
#define NCSEditFreeInfo NCSFreeEditInfo
#define NCSEditCopyInfo NCSCopyEditInfo


#define NCSEcwEditSetFileMetaData NCSEditSetFileMetaData
#define NCSEcwEditSetStatistics NCSEditSetStatistics
#define NCSEcwEditSetRPCData NCSEditSetRPCData
#define NCSEcwEditOpen NCSEditOpen
#define NCSEcwEditSetGeotiffKey NCSEditSetGeotiffKey
#define NCSEcwEditSetGeotiffTag NCSEditSetGeotiffTag
#define NCSEcwEditFlushAll NCSEditFlushAll
#define NCSEcwEditClose NCSEditClose
#define NCSEcwEditInfo NCSEditInfo

// NCSECWCompressClient.h
#define NCSEcwCompressFreeClient NCSCompressFreeClient
#define NCSEcwCompressClose NCSCompressClose
#define NCSEcwCompress NCSCompress
#define NCSEcwCompressSetStatistics NCSCompressSetStatistics
#define NCSEcwCompressSetRPCData NCSCompressSetRPCData
#define NCSEcwCompressOpen NCSCompressOpen
#define NCSEcwCompressSetOEMKey NCSCompressSetOEMKey
#define NCSEcwCompressAllocClient NCSCompressAllocClient
#define NCSEcwCompressSetGeotiffKey NCSCompressSetGeotiffKey
#define NCSEcwCompressSetGeotiffTag NCSCompressSetGeotiffTag
#define NCSEcwCompressClient NCSCompressClient

#define CompressFormat NCSCompressFormat
#define COMPRESS_NONE NCS_COMPRESS_NONE
#define COMPRESS_UINT8 NCS_COMPRESS_UINT8
#define COMPRESS_YUV NCS_COMPRESS_YUV
#define COMPRESS_MULTI NCS_COMPRESS_MULTI
#define COMPRESS_RGB NCS_COMPRESS_RGB

#define CompressHint NCSCompressHint
#define COMPRESS_HINT_NONE NCS_COMPRESS_HINT_NONE
#define COMPRESS_HINT_FAST NCS_COMPRESS_HINT_FAST
#define COMPRESS_HINT_BEST NCS_COMPRESS_HINT_BEST
#define COMPRESS_HINT_INTERNET NCS_COMPRESS_HINT_INTERNET

// NCSEcw/ECW/ECW.h
#define BlockingFormat NCSBlockingFormat
#define BLOCKING_LEVEL NCS_BLOCKING_LEVEL

#define EncodeFormat NCSEncodeFormat
#define ENCODE_INVALID NCS_ENCODE_INVALID
#define ENCODE_RAW NCS_ENCODE_RAW
#define ENCODE_HUFFMAN NCS_ENCODE_HUFFMAN
#define ENCODE_RANGE NCS_ENCODE_RANGE
#define ENCODE_RANGE8 NCS_ENCODE_RANGE8
#define ENCODE_ZEROS NCS_ENCODE_ZEROS
#define ENCODE_RUN_ZERO NCS_ENCODE_RUN_ZERO
#define ENCODE_RANGE16 NCS_ENCODE_RANGE16
#define ENCODE_HUFFMAN16 NCS_ENCODE_HUFFMAN16
#define ENCODE_HUFFMAN8to8 NCS_ENCODE_HUFFMAN8to8
#define ENCODE_RUN_ZERO_COUNT_LOC_SHIFT NCS_ENCODE_RUN_ZERO_COUNT_LOC_SHIFT
#define ENCODE_HUFFMAN8to8_COUNT_LOC_SHIFT NCS_ENCODE_HUFFMAN8to8_COUNT_LOC_SHIFT
#define ENCODE_HUFFMAN16to8_COUNT_LOC_SHIFT NCS_ENCODE_HUFFMAN16to8_COUNT_LOC_SHIFT
#define ENCODE_RUN_NONDIFF_COUNT_LOC_SHIFT NCS_ENCODE_RUN_NONDIFF_COUNT_LOC_SHIFT
#define ENCODE_HUFFMAN8to8_NONDIFF_COUNT_LOC_SHIFT NCS_ENCODE_HUFFMAN8to8_NONDIFF_COUNT_LOC_SHIFT
#define ENCODE_HUFFMAN16to8_NONDIFF_COUNT_LOC_SHIFT NCS_ENCODE_HUFFMAN16to8_NONDIFF_COUNT_LOC_SHIFT

// NCSLog.h
#define OFF_LEVEL NCS_OFF_LEVEL
#define FATAL_LEVEL NCS_FATAL_LEVEL
#define ERROR_LEVEL NCS_ERROR_LEVEL
#define WARN_LEVEL NCS_WARN_LEVEL
#define INFO_LEVEL NCS_INFO_LEVEL
#define DEBUG_LEVEL NCS_DEBUG_LEVEL
#define TRACE_LEVEL NCS_TRACE_LEVEL
#define ALL_LEVEL NCS_ALL_LEVEL
#define NOT_SET_LEVEL NCS_NOT_SET_LEVEL
#define ENCSLogLevel NCSLogLevel

#endif // NCS_ECW_COMPATIBILITY_H