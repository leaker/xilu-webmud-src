// Copyright (C) 1999-2000 Open Source Telecom Corporation.
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
// As a special exception to the GNU General Public License, permission is 
// granted for additional uses of the text contained in its release 
// of Common C++.
// 
// The exception is that, if you link the Common C++ library with other
// files to produce an executable, this does not by itself cause the
// resulting executable to be covered by the GNU General Public License.
// Your use of that executable is in no way restricted on account of
// linking the Common C++ library code into it.
// 
// This exception does not however invalidate any other reasons why
// the executable file might be covered by the GNU General Public License.
// 
// This exception applies only to the code released under the 
// name Common C++.  If you copy code from other releases into a copy of
// Common C++, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
// 
// If you write modifications of your own for Common C++, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.  

#ifdef __WIN32__ 

#ifndef	__CCXX_CONFIG_H__
#define	__CCXX_CONFIG_H__

#ifndef	__CCXX_WIN32
#define	__CCXX_WIN32
#ifdef __WIN32__
#include <windows.h>
#endif //__WIN32__
#endif

#if defined(__MINGW32__) || defined(__CYGWIN32__)
#undef __EXPORT
#undef __stdcall
#define __EXPORT
#define __stdcall
typedef	char int8;
typedef short int16;
typedef long int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;
typedef DWORD size_t;
#else
#define	__EXPORT	__declspec(dllexport)
typedef	__int8	int8;
typedef	__int16 int16;
typedef	__int32 int32;
typedef	__int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
#endif

#endif
#else //__WIN32__
#include "config_unix.h"
#endif //__WIN32__ 


