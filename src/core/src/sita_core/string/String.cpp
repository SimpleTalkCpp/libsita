#include "String.h"

namespace sita {

void StringUtil::appendBinToHex(String& result, const u8* data, size_t dataSize) {	
	const char* hex = "0123456789ABCDEF";
	size_t lineCount = (dataSize + 15) / 16;


	for (size_t line = 0; line < lineCount; line++) {
		for (size_t i = 0; i < 16; i++) {
			size_t j = line * 16;

			if (j < dataSize) {
				u8 v = data[j];
				result.push_back(hex[v >> 4]);
				result.push_back(hex[v & 0xF]);
			} else {
				result.push_back(' ');
				result.push_back(' ');
			}
			result.push_back(' ');
		}
	}
}


} // namespace
