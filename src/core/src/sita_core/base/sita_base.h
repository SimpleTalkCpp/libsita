#pragma once

#include "../detect_platform/sita_detect_platform.h"

#if SITA_OS_WINDOWS
	#define NOMINMAX 1

	#include <WinSock2.h> // WinSock2.h must include before windows.h to avoid winsock1 define
	#include <ws2tcpip.h> // struct sockaddr_in6
	#pragma comment(lib, "Ws2_32.lib")

	#include <Windows.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <netinet/in.h> // struct sockaddr_in
#endif

#include <cassert>
#include <exception>
#include <iostream>
#include <cstdlib>
#include <cstdint>

#include <EASTL/vector.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/string.h>
#include <EASTL/fixed_string.h>
#include <EASTL/string_view.h>
#include <EASTL/span.h>

#include <EASTL/map.h>
#include <EASTL/hash_map.h>
#include <EASTL/string_map.h>

#include <EASTL/unique_ptr.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/weak_ptr.h>

#include "sita_macro.h"

//==== EASTL ====

#if !EASTL_DLL // If building a regular library and not building EASTL as a DLL...
	// It is expected that the application define the following
	// versions of operator new for the application. Either that or the
	// user needs to override the implementation of the allocator class.
	inline void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line) { return malloc(size); }
	inline void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
		#if SITA_OS_WINDOWS
			return _aligned_malloc(size, alignment);
		#else
			return std::aligned_alloc(alignment, size);
		#endif
	}
#endif

//===============

namespace sita {

template<class T> inline constexpr typename std::underlying_type<T>::type         enumInt   (      T  value) { return       static_cast<      typename std::underlying_type<T>::type >( value); }
template<class T> inline constexpr typename std::underlying_type<T>::type       & enumIntRef(      T& value) { return *reinterpret_cast<      typename std::underlying_type<T>::type*>(&value); }
template<class T> inline constexpr typename std::underlying_type<T>::type const & enumIntRef(const T& value) { return *reinterpret_cast<const typename std::underlying_type<T>::type*>(&value); }

template<class T> inline bool constexpr enumHas		(const T& a, const T& b) { return static_cast<T>(enumInt(a) & enumInt(b)) != static_cast<T>(0); }

template<class T> SITA_INLINE T* constCast(const T* v) { return const_cast<T*>(v); }
template<class T> SITA_INLINE T& constCast(const T& v) { return const_cast<T&>(v); }

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// using f16 = half;
using f32 = float;
using f64 = double;
using f128 = long double;

template<class T> using UPtr = eastl::unique_ptr<T>;
template<class T> using SPtr = eastl::shared_ptr<T>;
template<class T> using WPtr = eastl::weak_ptr<T>;

template<class T> using Span   = eastl::span<T>;
template<class T, size_t N, bool bEnableOverflow = true> using Vector_ = eastl::fixed_vector<T, N, bEnableOverflow>;

template<class T> using Vector = eastl::vector<T>;

template<class T> using StrViewT = eastl::basic_string_view<T>;
using StrView = StrViewT<char>;

template<class T, size_t N, bool bEnableOverflow = true> // using FixedStringT = eastl::fixed_string<T, N, bEnableOverflow>;
class StringT : public eastl::fixed_string<T, N, bEnableOverflow> {
	using Base = eastl::fixed_string<T, N, bEnableOverflow>;
public:
	StringT() = default;
	StringT(StrViewT<T> view) : Base(view.data(), view.size()) {}
	StringT(StringT && str) : Base(std::move(str)) {}

	template<class R> void operator=(R && r) { Base::operator=(SITA_FORWARD(r)); }
};

using String = eastl::string;
template<size_t N, bool bEnableOverflow = true> using String_ = StringT<char, N, bEnableOverflow>;
using TempString = String_<220>;

//! Source Location
class SrcLoc {
public:
	SrcLoc() = default;
	SrcLoc(const char* file_, int line_, const char* func_) 
		: file(file_)
		, line(line_) 
		, func(func_)
	{}

	const char* file = "";
	const char* func;
	int line = 0;
};

class NonCopyable {
public:
	NonCopyable() = default;

private:
	NonCopyable(NonCopyable &&) {}

	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

} // namespace
