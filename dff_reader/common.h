#ifndef COMMON_H
#define COMMON_H
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

/*
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef unsigned long	QWORD;
*/

typedef unsigned __int8		BYTE;
typedef unsigned __int16	WORD;
typedef unsigned __int32	DWORD;
typedef unsigned __int64	QWORD;

typedef unsigned int		uint;
typedef unsigned long		ulong;

// TODO: reference additional headers your program requires here
#endif