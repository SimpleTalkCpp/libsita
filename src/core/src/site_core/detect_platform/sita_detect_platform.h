#pragma once

/*
Sita_DetectPlatform:
	- headers only
	- detect compiler / OS / CPU
	- may be used by external lib, so please keep it simple
*/

#ifdef __OBJC__
	#define SITA_OBJC		1
#else
	#define SITA_OBJC		0
#endif

//=========== Detect COMPILER ===============
#if defined(__clang__) 
	#define SITA_COMPILER_CLANG	1
	#include "sita_detect_compiler_gcc.h"

#elif defined(__GNUC__)
	#define SITA_COMPILER_GCC		1
	#include "sita_detect_compiler_gcc.h"

#elif defined(_MSC_VER)
	#define SITA_COMPILER_VC		1
	#include "sita_detect_compiler_vc.h"

#endif

#if SITA_COMPILER_VC + SITA_COMPILER_GCC + SITA_COMPILER_CLANG != 1
    #error "Compiler should be specified"
#endif

//======== Detect CPU =============

// check CPU define
#if SITA_CPU_X86_64 + SITA_CPU_X86 + SITA_CPU_POWERPC + SITA_CPU_ARM + SITA_CPU_ARM64 != 1
	#error "CPU should be specified"
#endif

#if SITA_CPU_X86_64
	#define SITA_CPU_LP64				1
	#define SITA_CPU_ENDIAN_LITTLE	1
	#define SITA_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SITA_CPU_X86
	#define SITA_CPU_LP32				1
	#define SITA_CPU_ENDIAN_LITTLE	1
	#define SITA_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SITA_CPU_POWERPC
	#define SITA_CPU_LP32				1
	#define SITA_CPU_ENDIAN_BIG		1
	#define SITA_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SITA_CPU_ARM64
	#define SITA_CPU_LP64				1
	#define SITA_CPU_MISALIGNED_MEMORY_ACCESSES	8
#endif

#if SITA_CPU_ARM
	#define SITA_CPU_LP32				1
	#define SITA_CPU_MISALIGNED_MEMORY_ACCESSES	1
#endif

#if SITA_CPU_LP32 + SITA_CPU_LP64 != 1
	#error "CPU bits should be specified"
#endif

#if SITA_CPU_LP32
	#define SITA_SIZEOF_POINTER		4
#endif

#if SITA_CPU_LP64
	#define SITA_SIZEOF_POINTER		8
#endif

#ifndef SITA_SIZEOF_POINTER
	#error "Unknown SITA_SIZEOF_POINTER"
#endif

#if SITA_CPU_ENDIAN_BIG + SITA_CPU_ENDIAN_LITTLE != 1
	#error "CPU endian should be specified"
#endif

//======== Detect OS ===============

#if SITA_OS_WIN32 + SITA_OS_WIN64 + SITA_OS_WINCE \
	+ SITA_OS_FREEBSD + SITA_OS_LINUX \
	+ SITA_OS_ANDROID \
	+ SITA_OS_MACOSX  + SITA_OS_IOS \
	+ SITA_OS_CYGWIN  + SITA_OS_MINGW != 1
	#error "OS should be specified"
#endif



//====================================

#ifndef UNICODE
	#define UNICODE
#endif

#ifndef _UNICODE
	#define _UNICODE
#endif
