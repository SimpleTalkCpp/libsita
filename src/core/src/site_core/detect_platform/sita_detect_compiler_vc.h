#pragma once

#pragma warning(disable: 4668) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'

#if !SITA_COMPILER_VC	
	#error
#endif

#if _MSVC_LANG >= 199711L
	#define SITA_CPLUSPLUS_03	1
#endif

#if _MSVC_LANG >= 201103L
	#define SITA_CPLUSPLUS_11	1
#endif

#if _MSVC_LANG >= 201402L
	#define SITA_CPLUSPLUS_14	1
#endif

#if _MSVC_LANG >= 201703L
	#define SITA_CPLUSPLUS_17	1
#endif

#if _MSVC_LANG >= 202002L
	#define SITA_CPLUSPLUS_20	1
#endif

#define SITA_TYPE_HAS_SIZE_T		0
#define SITA_TYPE_HAS_SSIZE_T		0
#define SITA_TYPE_HAS_LONGLONG		0

#define SITA_DLL_EXPORT			_declspec(dllexport)
#define SITA_DLL_IMPORT			_declspec(dllimport)

#define	SITA_FUNC_NAME_SZ			__FUNCTION__
#define SITA_FUNC_FULLNAME_SZ		__FUNCSIG__

#define SITA_DEPRECATED			__declspec(deprecated)

#define SITA_COMPILER_VER _MSVC_LANG

#if SITA_CPLUSPLUS_17
	#define SITA_FALLTHROUGH	//	[[fallthrough]]
	#define SITA_NODISCARD		[[nodiscard]]
#else
	#define SITA_FALLTHROUGH
	#define SITA_NODISCARD
#endif

//#define SITA_ALIGN(N)				__declspec(align(N)) 
//#define SITA_ALIGN(N)				alignas(N) //c++11

#define SITA_OPTIMIZE_OFF			__pragma(optimize("", off))

#if 0 // SITA_GEN_CONFIG_Debug
	#define	SITA_INLINE		inline
#else
	#define	SITA_INLINE		__forceinline
#endif

#define SITA_THREAD_LOCAL			__declspec( thread )
//#define SITA_THREAD_LOCAL thread_local //C++11

#if _MSC_VER < 1600
	#define	nullptr	NULL
#endif

//cpu
#if _M_X64
	#define SITA_CPU_X86_64	1
	#define SITA_CPU_FEATURE_SSE1		1
	#define SITA_CPU_FEATURE_SSE2		1
	#define SITA_CPU_FEATURE_SSE3		1

#elif _M_PPC
	#define SITA_CPU_PowerPC     1
#else
	#define SITA_CPU_x86         1
#endif

//os

#if _WIN64
	#define SITA_OS_WIN64     1
	#define SITA_OS_WINDOWS	1
#elif _WIN32
	#define SITA_OS_WIN32     1
	#define SITA_OS_WINDOWS	1
#elif _WINCE
	#define SITA_OS_WinCE     1
	#define SITA_OS_WINDOWS	1
#endif
