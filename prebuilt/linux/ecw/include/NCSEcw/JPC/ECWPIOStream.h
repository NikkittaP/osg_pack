/********************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE:     ECWPIOStream.h
 ** CREATED:  05/12/2002 3:27:34 PM
 ** AUTHOR:   James Xiao
 ** PURPOSE:  JP2 ECWP IO Stream
 ** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCECWPIOSTREAM_H
#define NCSJPCECWPIOSTREAM_H
#if !defined(ECWP2_DISABLED)
#include "NCSEcw/SDK/ECWPIOStream.h"
#endif
namespace NCS {
    namespace JP2 {
        class CFile;
    }
#if !defined(ECWP2_DISABLED)
    namespace JPC {
        
        /**
         * CECWPIOStream class - 64bit ECWP IO "Stream" class.
         * This class is the IO mechanism used to access JP2 files via ECWP.
         * << and >> Operators are not implemented as we can't use exception handling for portability reasons.
         *
         * @author       Simon Cope
         * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $
         */
        class NCSECW_IMPEX CECWPIOStream: public SDK::CECWPIOStream {
        public:
            /** Constructor, initialises members */
            CECWPIOStream(JP2::CFile *pFile, const CString &URLPath, const CString &HttpHeaders);
            
        protected:
        };
        
    }//JPC
#else
#error This file should not be included if ECWP2_DISABLED
#endif //!defined(ECWJP2_DISABLED)
} // NCS

#endif