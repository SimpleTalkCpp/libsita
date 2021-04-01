#pragma once

#include "../base/sita_base.h"

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

namespace sita {

template<class... ARGS> inline
void FmtTo(TempString& outStr, ARGS&&... args) {
	fmt::format_to(std::back_inserter(outStr), SITA_FORWARD(args)...);
}

template<class... ARGS> inline
TempString Fmt(ARGS&&... args) {
	TempString o;
	FmtTo(o, SITA_FORWARD(args)...);
	return o;
}

}


