#pragma once

#include "../sita_net_common-pch.h"

namespace sita {

class NEPacketHeader {
public:
	using Len = u16;
	using Cmd = u32;


	Len len = 0;
	Cmd cmd = 0;
};

class NEPacket {
public:
	using Header = NEPacketHeader;

	void writeBuffer(const Vector<char>& buf);
	void readBuffer (Vector<char>& buf);

private:
	Header _header;
};

} // namespace