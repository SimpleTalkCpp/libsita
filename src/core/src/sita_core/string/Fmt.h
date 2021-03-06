#pragma once

#include <fmt/format.h>

#if SITA_COMPILER_VC
	#if _DEBUG
		#pragma comment(lib, "fmtd.lib")
	#else
		#pragma comment(lib, "fmt.lib")
	#endif
#endif

#define SITA_FORMATTER(T) \
	template<> \
	struct fmt::formatter<T> { \
		auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		auto format(const T& v, fmt::format_context& ctx) { return v.onFormat(ctx); } \
	};
//------



