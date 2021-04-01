#pragma once

#include "../base/sita_base.h"
#include "Fmt.h"

namespace sita {

class StringUtil {
public:
	static void binToHex(String& result, Span<u8> data) {
		result.clear();
		appendBinToHex(result, data.data(), data.size());
	}

	static void appendBinToHex(String& result, const u8* data, size_t dataSize);
};

} // namespace

inline
std::ostream& operator<<(std::ostream& s, const sita::StrView& v) {
	s.write(v.data(), v.size());
	return s;
}

template<>
struct fmt::formatter<sita::String> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const sita::String& v, fmt::format_context& ctx) {
		std::string_view view(v.data(), v.size());
		return fmt::format_to(ctx.out(), "{}", view);
	}
};
