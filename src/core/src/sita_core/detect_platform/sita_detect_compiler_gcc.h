#pragma once

#if ! (SITA_COMPILER_CLANG | SITA_COMPILER_GCC)
	#error
#endif

#if __cplusplus >= 199711L
	#define SITA_CPLUSPLUS_03	1
#endif

#if __cplusplus >= 201103L 
	#define SITA_CPLUSPLUS_11	1
#endif

#if __cplusplus >= 201402L
	#define SITA_CPLUSPLUS_14	1
#endif

#if __cplusplus >= 201703L
	#define SITA_CPLUSPLUS_17	1
#endif

#if __cplusplus >= 202002L
	#define SITA_CPLUSPLUS_20	1
#endif



#if SITA_COMPILER_CLANG
	#define SITA_TYPE_HAS_SIZE_T		0  //linux 
	#define SITA_TYPE_HAS_SSIZE_T		0  //linux 

	#define SITA_OPTIMIZE_OFF			_Pragma("clang optimize off")
#else
	#define SITA_TYPE_HAS_SIZE_T		0  //linux 
	#define SITA_TYPE_HAS_SSIZE_T		0  //linux 

	#define SITA_OPTIMIZE_OFF			_Pragma("GCC optimize(\"O0\")")
#endif

#if __GNUC__ >= 4
	#define	SITA_DLL_EXPORT			__attribute__ ((visibility ("default")))
	#define SITA_DLL_IMPORT			// __attribute__ ((visibility ("hidden")))
#else
	#define	SITA_DLL_EXPORT
	#define SITA_DLL_IMPORT
#endif


// #define nullptr	NULL

#define	SITA_FUNC_NAME_SZ				__FUNCTION__
#define SITA_FUNC_FULLNAME_SZ			__PRETTY_FUNCTION__

#define SITA_DEPRECATED( f )		f __attribute__( (deprecated) )

#define SITA_COMPILER_VER __cplusplus

#if SITA_CPLUSPLUS_17
	#define SITA_FALLTHROUGH		// [[fallthrough]]
	#define SITA_NODISCARD		[[nodiscard]]
#else
	#define SITA_FALLTHROUGH
	#define SITA_NODISCARD
#endif

//#define SITA_ALIGN(N)				__attribute__((aligned(N)))
//#define SITA_ALIGN(N)				alignas(N) //c++11

#if SITA_GEN_CONFIG_Debug
	#define	SITA_INLINE      inline
#else
	#define	SITA_INLINE		inline //__attribute__( (always_inline) )
#endif

//#define SITA_THREAD_LOCAL	__thread
#define SITA_THREAD_LOCAL thread_local //C++11


#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64) || defined(__amd64__)
	#define SITA_CPU_X86_64      1

#elif defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
	#define SITA_CPU_X86         1

#elif defined(__POWERPC__)
	#define SITA_CPU_POWERPC     1

#elif defined(__aarch64__)
	#define SITA_CPU_ARM64 1

	#if defined(__ARMEL__) || defined(__AARCH64EL__)
		#define SITA_CPU_ENDIAN_LITTLE	1
	#elif defined(__ARMEB__) || defined(__AARCH64EB__)
		#define SITA_CPU_ENDIAN_BIG		1
	#else
		#error unknown ARM CPU endian
	#endif

#elif defined(__arm__)
	#define SITA_CPU_ARM 1

	#if defined(__ARMEL__) || defined(__AARCH64EL__)
		#define SITA_CPU_ENDIAN_LITTLE	1
	#elif defined(__ARMEB__) || defined(__AARCH64EB__)
		#define SITA_CPU_ENDIAN_BIG		1
	#else
		#error unknown ARM CPU endian
	#endif
#endif

#if	defined(__ARM_NEON__) || defined(__ARM_NEON)
	#define SITA_CPU_FEATURE_ARM_NEON
#endif

#if __SSE__
	#define SITA_CPU_FEATURE_SSE1			1
#endif

#if __SSE2__
	#define SITA_CPU_FEATURE_SSE2			1
	#include <xmmintrin.h>
#endif

#if __SSE3__
	#define SITA_CPU_FEATURE_SSE3			1
#endif

#if __SSSE3__
	#define SITA_CPU_FEATURE_SSSE3		1
#endif

#if __SSE4_1__
	#define SITA_CPU_FEATURE_SSE4_1		1
#endif

#if __SSE4_2__
	#define SITA_CPU_FEATURE_SSE4_2		1
#endif

//os

#if _WIN64
	#define SITA_OS_WIN64     1
	#define SITA_OS_WINDOWS	1

#elif _WIN32
	#define SITA_OS_WIN32     1
	#define SITA_OS_WINDOWS	1

#elif __ANDROID__
	#define SITA_OS_ANDROID	1

#elif __linux
	#define SITA_OS_LINUX        1

#elif __FreeBSD__
	#define SITA_OS_FREEBSD		1

#elif __APPLE__ && __MACH__
	#include <TargetConditionals.h>
	#if (TARGET_OF_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define SITA_OS_IOS		1
	#else
		#define SITA_OS_MACOSX	1
	#endif

#elif __sun
	#define SITA_OS_SOLARIS		1

#elif __CYGWIN__
    #define SITA_OS_CYGWIN        1

#elif __MINGW32__
	#define SITA_OS_MINGW			1
#endif

