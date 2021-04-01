#pragma once

#include "../sita_net_common-config.h"

namespace sita {

class NEPacketHeader {
public:
	using Len = u16;
	using Cmd = u32;

	Len len = 0;
	Cmd cmd = 0;

	template<class SE>
	void io(SE& se) {
		se.io_fixed(len);
		se.io(cmd);
	}
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