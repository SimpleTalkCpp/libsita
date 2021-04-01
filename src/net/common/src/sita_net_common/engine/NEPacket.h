#pragma once

#include "../sita_net_common-config.h"

namespace sita {

using NEPacketCmd = u32;
using NEPacketLen = u16;

class NEPacketHeader {
public:
	using Len = NEPacketLen;
	using Cmd = NEPacketCmd;

	Len len = 0;
	Cmd cmd = 0;

	NEPacketHeader() = default;
	NEPacketHeader(Cmd cmd_) : cmd(cmd_) {}

	template<class SE>
	void io(SE& se) {
		se.io_fixed(len);
		se.io(cmd);
	}
};

class NEPacket : public NonCopyable {
public:
	using Header = NEPacketHeader;

	NEPacket(NEPacketCmd cmd) : _header(cmd) {}

	void writeToBuffer (Vector<u8>&    buf, NEPacketLen lenKey);
	void readFromBuffer(Span<const u8> buf);

	virtual void onWrite(BinSerializer&   se) = 0;
	virtual void onRead (BinDeserializer& se) = 0;

private:
	Header _header;
};

} // namespace