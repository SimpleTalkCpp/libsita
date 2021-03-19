#pragma once

#include "sita_base.h"
#include "Error.h"
#include "ByteOrder.h"

namespace sita {

class BinSerializer : public NonCopyable {
public:
	BinSerializer(Vector<u8>& buf) {
	}

private:
	Vector<u8>* _buf = nullptr;
};

} // namespace