/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	ecwp3packets.h
** CREATED:	01/03/2012
** PURPOSE:	Packet declarations for ECWP3
**
********************************************************/

#ifndef NCS_ECWP3_PACKETS_H
#define NCS_ECWP3_PACKETS_H

#define ECWP3_PACKET_PREAMBLE 0x6F6D

#define ECWP3_BASE_PACKET_SIZE 8
#define ECWP3_ERROR_PACKET_SIZE 5
#define ECWP3_GET_PACKET_SIZE 8
#define ECWP3_OPEN_PACKET_SIZE 25
#define ECWP3_RECONNECT_PACKET_SIZE 12
#define ECWP3_CLOSE_PACKET_SIZE 1

typedef enum {
		ECWP3_CMD_OPEN = 0,
		ECWP3_CMD_GET = 1,
		ECWP3_CMD_RECONNECT = 2,
		ECWP3_CMD_CLOSE = 3
} NCSecwp3ServerCommand;

typedef enum {
	ECWP3_FILETYPE_ECW = 0,
	ECWP3_FILETYPE_ECW_WITH_OPACITY = 1,
	ECWP3_FILETYPE_JP2 = 2,
	ECWP3_FILETYPE_UNKNOWN
} NCSecwp3FileType;

typedef enum {
	ECWP3_OPEN_RESPONSE = 0,
	ECWP3_RECONNECT_RESPONSE = 1,
	ECWP3_CLOSE_RESPONSE = 2,
	ECWP3_GET_RESPONSE = 3,
	ECWP3_ERROR_RESPONSE = 4
} NCSecwp3PacketType;

typedef enum {
	ECWP3_ERROR_NOSESSION = 0,
	ECWP3_ERROR_CANTOPENFILE = 1,
	ECWP3_ERROR_INVALIDBLOCK = 2,
	ECWP3_ERROR_MALFORMEDREQUEST = 3,
	ECWP3_ERROR_INVALIDCLIENTVER = 4,
	ECWP3_ERROR_SECURITY = 5
} NCSecwp3ErrorType;

struct NCSecwp3EuclideonHeader {
	UINT8 nVersion;
	INT64 nFileSize;
};

static const char*  NCSecwp3ErrorTypeDescriptions[] = {
	"No session",
	"Unable to open file",
	"Invalid block",
	"Malformed request",
	"Client version",
	"Security error"
};

#define ECWP3_STATELESS_CLIENT_ID 0xffffffffffffffff

#endif //NCS_ECWP3_PACKETS_H