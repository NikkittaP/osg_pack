/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: Error.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Error header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_ERROR_TYPE_H
#define NCS_API_ERROR_TYPE_H

/** 
 *	@enum NCSError
 *	@ingroup enums
 *	An enumerated type specifying all the errors associated with the SDK.  Each error
 *	code is associated with a string of explanatory text. 
 *  Note: add the new errors to the end of enum but before NCS_MAX_ERROR_NUMBER
 */
typedef enum {
	/* NCS Raster Errors */
	NCS_SUCCESS					= 0,		/**< No error */
	NCS_QUEUE_NODE_CREATE_FAILED,			/**< Queue node creation failed */
	NCS_FILE_OPEN_FAILED,					/**< File open failed */
	NCS_FILE_LIMIT_REACHED,					/**< The server's licensed file limit has been reached */
	NCS_FILE_SIZE_LIMIT_REACHED,			/**< The requested file is larger than is permitted by the license on this server/client */
	NCS_FILE_NO_MEMORY,						/**< Not enough memory for new file */
	NCS_CLIENT_LIMIT_REACHED,				/**< The server's licensed client limit has been reached */
	NCS_DUPLICATE_OPEN,						/**< Detected duplicate open from net layer */
	NCS_PACKET_REQUEST_NYI,					/**< Packet request type not yet implemented */
	NCS_PACKET_TYPE_ILLEGAL,				/**< Packet type is illegal */
	NCS_DESTROY_CLIENT_DANGLING_REQUESTS,	/**< Client closed while requests outstanding */

	/* NCS Network Errors */
	NCS_UNKNOWN_CLIENT_UID,					/**< Client UID unknown */
	NCS_COULDNT_CREATE_CLIENT,				/**< Could not create new client */
	NCS_NET_COULDNT_RESOLVE_HOST,			/**< Could not resolve the internet address */
	NCS_NET_COULDNT_CONNECT,				/**< Could not connect to host */
	NCS_NET_RECV_TIMEOUT,					/**< Receive timeout */
	NCS_NET_HEADER_SEND_FAILURE,			/**< Error sending header */
	NCS_NET_HEADER_RECV_FAILURE,			/**< Error receiving header */
	NCS_NET_PACKET_SEND_FAILURE,			/**< Error sending packet */
	NCS_NET_PACKET_RECV_FAILURE,			/**< Error receiving packet */
	NCS_NET_401_UNAUTHORISED,				/**< 401 Unauthorised: SDK doesn't do authentication so this suggests a misconfigured server */			
	NCS_NET_403_FORBIDDEN,					/**< 403 Forbidden: could be a 403.9 from IIS or PWS meaning that the maximum simultaneous request limit has been reached */
	NCS_NET_404_NOT_FOUND,					/**< 404 Not Found: this error suggests that the server software has not been installed or a resource was not found */
	NCS_NET_407_PROXYAUTH,					/**< 407 Proxy Authentication: the SDK doesn't do proxy authentication yet either, so this also suggests misconfiguration */
	NCS_NET_UNEXPECTED_RESPONSE,			/**< Unexpected HTTP response could not be handled */
	NCS_NET_BAD_RESPONSE,					/**< HTTP response received outside specification */
	NCS_NET_ALREADY_CONNECTED,				/**< Already connected */
	NCS_INVALID_CONNECTION,					/**< Connection is invalid */
	NCS_WINSOCK_FAILURE,					/**< A Windows sockets failure occurred */

	/* NCS Symbol Errors */
	NCS_SYMBOL_ERROR,			/**< Symbology error */
	NCS_OPEN_DB_ERROR,			/**< Could not open database */
	NCS_DB_QUERY_FAILED,		/**< Could not execute the requested query on database */
	NCS_DB_SQL_ERROR,			/**< SQL statement could not be executed */
	NCS_GET_LAYER_FAILED,		/**< Open symbol layer failed */
	NCS_DB_NOT_OPEN,			/**< The database is not open */
	NCS_QT_TYPE_UNSUPPORTED,	/**< This type of quadtree is not supported */

	/* Preference errors */
	NCS_PREF_INVALID_USER_KEY,		/**< Invalid local user key name specified */
	NCS_PREF_INVALID_MACHINE_KEY,	/**< Invalid local machine key name specified */
	NCS_REGKEY_OPENEX_FAILED,		/**< Failed to open registry key */
	NCS_REGQUERY_VALUE_FAILED,		/**< Registry query failed */
	NCS_INVALID_REG_TYPE,			/**< Type mismatch in registry variable */

	/* Misc Errors */
	NCS_INVALID_ARGUMENTS,		/**< Invalid arguments passed to function */
	NCS_ECW_ERROR,				/**< ECW error */
	/* unspecified, but coming out of ecw */
	NCS_SERVER_ERROR,			/**< Server error */
	/* unspecified server error */
	NCS_UNKNOWN_ERROR,			/**< Unknown error */
	NCS_EXTENT_ERROR,			/**< Extent conversion failed */
	NCS_COULDNT_ALLOC_MEMORY,	/**< Could not allocate enough memory */
	NCS_INVALID_PARAMETER,		/**< An invalid parameter was used */

	/* Compression Errors */
	NCS_FILEIO_ERROR,						/**< Error reading or writing file */
	NCS_COULDNT_OPEN_COMPRESSION,			/**< Compression task could not be initialised */
	NCS_COULDNT_PERFORM_COMPRESSION,		/**< Compression task could not be processed */
	NCS_GENERATED_TOO_MANY_OUTPUT_LINES,	/**< Trying to generate too many output lines */
	NCS_USER_CANCELLED_COMPRESSION,			/**< Compression task was cancelled by client application */
	NCS_COULDNT_READ_INPUT_LINE,			/**< Could not read line from input data */
	NCS_INPUT_SIZE_EXCEEDED,				/**< Input image size was exceeded for this version of the SDK */
	NCS_MISMATCH_PARAMETER_COMPRESSION,		/**< Parameter not matched for corresponding compreesion version of the SDK */

	/* Decompression Errors */
	NCS_REGION_OUTSIDE_FILE,	/**< Specified image region is outside image extents */
	NCS_NO_SUPERSAMPLE,			/**< Supersampling is not supported by the SDK functions */
	NCS_ZERO_SIZE,				/**< Specified image region has a zero width or height */
	NCS_TOO_MANY_BANDS,			/**< More bands specified than exist in the input file */
	NCS_INVALID_BAND_NR,		/**< An invalid band number has been specified */

	/* NEW Compression Error */
	NCS_INPUT_SIZE_TOO_SMALL,	/**< Input image size is too small to compress - for ECW compression there is a minimum output file size */

	/* NEW Network error */
	NCS_INCOMPATIBLE_PROTOCOL_VERSION,	/**< The ECWP client version is incompatible with this server */
	NCS_WININET_FAILURE,				/**< Windows Internet Client error */
	NCS_COULDNT_LOAD_WININET,			/**< wininet.dll could not be loaded - usually indicates Internet Explorer should be upgraded */

	/* NCSFile && NCSRenderer class errors */
	NCS_FILE_INVALID_SETVIEW,			/**< The parameters specified for setting a file view were invalid, or the view was not set */
	NCS_FILE_NOT_OPEN,					/**< No file is open */
	
	/* NEW JNI Java Errors */
	NCS_JNI_REFRESH_NOT_IMPLEMENTED,	/**< Class does not implement ECWProgressiveDisplay interface */
	/* A class is trying to use RefreshUpdate() method, but has not implemented ECWProgressiveDisplay*/

	/* NEW Coordinate Errors*/
	NCS_INCOMPATIBLE_COORDINATE_SYSTEMS,	/**< Incompatible coordinate systems */
	NCS_INCOMPATIBLE_COORDINATE_DATUM,		/**< Incompatible coordinate datum types */
	NCS_INCOMPATIBLE_COORDINATE_PROJECTION,	/**< Incompatible coordinate projection types */
	NCS_INCOMPATIBLE_COORDINATE_UNITS,		/**< Incompatible coordinate units types */
	NCS_COORDINATE_CANNOT_BE_TRANSFORMED,	/**< Non-linear coordinate systems not supported */
	NCS_GDT_ERROR,							/**< Error involving the GDT database */
	
	/* NEW NCScnet error */
	NCS_NET_PACKET_RECV_ZERO_LENGTH,	/**< Zero length packet received */
	/**[01]**/
	
	/* Macintosh SDK specific errors */
	NCS_UNSUPPORTEDLANGUAGE,			/**< Must use Japanese version of the ECW SDK */
	/**[02]**/

	/* Loss of connection */
	NCS_CONNECTION_LOST,				/**< Connection to server was lost */
	/**[03]**/

	NCS_COORD_CONVERT_ERROR,			/**< NCSGDT coordinate conversion failed */

	/* Metabase Stuff */
	NCS_METABASE_OPEN_FAILED,			/**< Failed to open metabase */
	/**[04]**/
	NCS_METABASE_GET_FAILED,			/**< Failed to get value from metabase */
	/**[04]**/
	NCS_NET_HEADER_SEND_TIMEOUT,		/**< Timeout sending header */
	/**[05]**/
	
	NCS_JNI_ERROR,						/**< Java JNI error */
	/**[06]**/

	NCS_DB_INVALID_NAME,				/**< No data source passed */
	/**[07]**/
	NCS_SYMBOL_COULDNT_RESOLVE_HOST,	/**< Could not resolve address of Symbol Server Extension */
	/**[07]**/
	
	NCS_INVALID_ERROR_ENUM,				/**< The value of an NCSError error number was invalid! */
	/**[08]**/
	
	/* NCSFileIO errors [10] */
	NCS_FILE_EOF,						/**< End of file reached */
	NCS_FILE_NOT_FOUND,					/**< File not found */
	NCS_FILE_INVALID,					/**< File was invalid or corrupt */
	NCS_FILE_SEEK_ERROR,				/**< Attempted to read, write or seek past file limits */
	NCS_FILE_NO_PERMISSIONS,			/**< Permissions not available to access file */
	NCS_FILE_OPEN_ERROR,				/**< Error opengin file */
	NCS_FILE_CLOSE_ERROR,				/**< Error closing file */
	NCS_FILE_IO_ERROR,					/**< Miscellaneous error involving file input or output */

	NCS_SET_EXTENTS_ERROR,				/**< Illegal or invalid world coordinates supplied */
	/**[09]**/ 

	NCS_FILE_PROJECTION_MISMATCH,		/**< Image projection does not match that of the controlling layer */
	/** 1.65 gdt errors [15]**/
	NCS_GDT_UNKNOWN_PROJECTION,		/**< Unknown map projection */
	NCS_GDT_UNKNOWN_DATUM,			/**< Unknown geodetic datum */
	NCS_GDT_USER_SERVER_FAILED,		/**< User specified Geographic Projection Database data server failed */
	NCS_GDT_REMOTE_PATH_DISABLED,	/**< Remote Geographic Projection Database file downloading has been disabled and no local GDT data is available */
	NCS_GDT_BAD_TRANSFORM_MODE,		/**< Invalid mode of transform */
	NCS_GDT_TRANSFORM_OUT_OF_BOUNDS,/**< Coordinate to be transformed is out of bounds */
	NCS_LAYER_DUPLICATE_LAYER_NAME,	/**< A layer already exists with the specified name */
	/**[17]**/
	NCS_LAYER_INVALID_PARAMETER,	/**< The specified layer does not contain the specified parameter */
	/**[18]**/
	NCS_PIPE_CREATE_FAILED,			/**< Failed to create pipe */
	/**[19]**/

	/* Directory creation errors */
	NCS_FILE_MKDIR_EXISTS,			/**< Directory to be created already exists */ /*[20]*/
	NCS_FILE_MKDIR_PATH_NOT_FOUND,	/**< The path specified for directory creation does not exist */ /*[20]*/
	NCS_ECW_READ_CANCELLED,			/**< File read was cancelled */

	/* JP2 georeferencing errors */
	NCS_JP2_GEODATA_READ_ERROR,		/**< Error reading geodata from a JPEG 2000 file */ /*[21]*/
	NCS_JP2_GEODATA_WRITE_ERROR,    /**< Error writing geodata to a JPEG 2000 file */	/*[21]*/
	NCS_JP2_GEODATA_NOT_GEOREFERENCED,		/**< JPEG 2000 file not georeferenced */			/*[21]*/

	NCS_PROGRESSIVE_VIEW_TOO_LARGE,			/**< Progressive views are limited to 6000x6000 in size */ /*[23]*/

	NCS_GDT_SET_DATUM_SHIFTS_ERROR,			/**< Failed to set datum shifts*/
	NCS_GDT_GET_DATUM_SHIFTS_ERROR,			/**< Failed to get datum shifts*/

	
	NCS_PROJECTION_STRING_TOO_LONG,			/**< Projection string is too big. */
	NCS_OTDF_FILE_VERSION_NOT_SUPPORTED,	/**< "Unsupported OTDF verion number" */

	NCS_ECWP_POLLING,						/**< ECWP Connection is using polling mode */

	NCS_INSUFFICIENT_PRIVILEGE,				/**< User has insufficient privilege for some special file operations.*/
	NCS_INSUFFICENT_FILESYSTEM_SPACE,		/**< Insufficient filesystem space */

	NCS_INVALID_WMS_SERVICE,				/**< Specified WMS Service is invalid or unable to be found */
	NCS_NON_MATCHED_TEMPORAL_EXTENT,		/**< Temporal extents for dataset was not match in request */

	NCS_UNSUPPORTED_FILE_TYPE_OR_VERSION,	/**< Unsupported file type or version */
    NCS_NO_VALUE,                           /**< Value is null */
	// Insert new errors before here!
	NCS_MAX_ERROR_NUMBER			/**< The maximum error value in this enumerated type - should not itself be reported, must always be defined last */ /*[08]*/
} NCSError;

#endif // NCS_API_ERROR_TYPE_H