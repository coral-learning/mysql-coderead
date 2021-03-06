/* Copyright (c) 2003-2006 MySQL AB
   Use is subject to license terms

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA */

#ifndef ERRORREPORTER_H
#define ERRORREPORTER_H

#include <ndb_global.h>
#include <ndbd_exit_codes.h>

#include "TimeModule.hpp"
#include <Emulator.hpp>

class ErrorReporter
{
public:
  static void setErrorHandlerShutdownType(NdbShutdownType nst = NST_ErrorHandler);
  static void handleAssert(const char* message, 
			   const char* file, 
			   int line, int ec = NDBD_EXIT_PRGERR)  __attribute__((__noreturn__));
  
  static void handleError(int faultID, 
			  const char* problemData,
                          const char* objRef,
			  enum NdbShutdownType = NST_ErrorHandler);
  
  static void handleWarning(int faultID, 
			    const char* problemData,
                            const char* objRef);
  
  static void formatMessage(int faultID, 
			    const char* problemData,
                            const char* objRef, 
			    const char* theNameOfTheTraceFile,
			    char* messptr);

  static int get_trace_no();
  
  static const char* formatTimeStampString();
  
private:
  static enum NdbShutdownType s_errorHandlerShutdownType;
};

#endif
