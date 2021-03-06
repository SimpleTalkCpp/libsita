#pragma once

#include "../detect_platform/sita_detect_platform.h"

#if SITA_OS_WINDOWS
	#include <WinSock2.h> // WinSock2.h must include before windows.h to avoid winsock1 define
	#include <ws2tcpip.h> // struct sockaddr_in6
	#pragma comment(lib, "Ws2_32.lib")
#endif

#include <cassert>
#include <exception>
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>

#include "sita_macro.h"

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

template<class T> inline constexpr typename std::underlying_type<T>::type         enum_int    (      T  value) { return       static_cast<      typename std::underlying_type<T>::type >( value); }
template<class T> inline constexpr typename std::underlying_type<T>::type       & enum_int_ref(      T& value) { return *reinterpret_cast<      typename std::underlying_type<T>::type*>(&value); }
template<class T> inline constexpr typename std::underlying_type<T>::type const & enum_int_ref(const T& value) { return *reinterpret_cast<const typename std::underlying_type<T>::type*>(&value); }

namespace sita {

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
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

} // namespace