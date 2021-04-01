#include "NEPacket.h"

namespace sita {

void NEPacket::writeToBuffer(Vector<u8>& buf, NEPacketLen lenKey) {
	buf.clear();
	BinSerializer se(buf);
	se.io(_header);
	onWrite(se);

	auto len = static_cast<NEPacketLen>(buf.size()) ^ lenKey;
	*reinterpret_cast<NEPacketLen*>(buf.data()) = LittleEndian::FromHost::get(len);
//	SITA_DUMP_HEX(buf);
}

void NEPacket::readFromBuffer(Span<const u8> buf) {
	BinDeserializer se(buf);
	auto cmd = _header.cmd;
	se.io(_header);
	onRead(se);

	if (cmd != _header.cmd) {
		_header.cmd = cmd;
		throw SITA_ERROR("invalid packet cmd");
	}
}

} // namespace