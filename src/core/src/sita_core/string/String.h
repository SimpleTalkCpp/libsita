#pragma once

#include "../base/sita_base.h"
#include "Fmt.h"

namespace sita {

using String = std::string;
using StrView = std::string_view;

class StringUtil {
public:
	static void binToHex(String& result, const Vector<u8>& data) {
		binToHex(result, data.data(), data.size());
	}

	static void binToHex(String& result, const u8* data, size_t dataSize) {
		result.clear();
		appendBinToHex(result, data, dataSize);
	}	

	static void appendBinToHex(String& result, const u8* data, size_t dataSize);
};


} // namespace