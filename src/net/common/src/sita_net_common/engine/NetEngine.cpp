#include "NetEngine.h"

namespace sita {

void NetEngine::update() {
	//for (; all sockets;) {
	//	
	//	onRecv(sock);
	//}
}

void NetEngine::onRecv(NESocket* s) {
	if (!s) return;

	NEPacketHeader hdr;

	const size_t _maxPacketPerFrame = 100;

	_recvPacketBuf.clear();
	_recvPacketBuf.resize(_maxPacketSize);

	for (size_t i = 0; i < _maxPacketPerFrame; i++) {
		auto bytesToRead = s->_sock.availableBytesToRead();
		if (bytesToRead < sizeof(hdr.len)) {
			break;
		}

		int ret = s->_sock.recv(_recvPacketBuf, sizeof(hdr.len), MSG_PEEK);
		if (ret <= sizeof(hdr.len)) {
			// error or disconnected
		}

		{
			BinDeserializer se(_recvPacketBuf);
			se.io_fixed(hdr.len);
		}
		
		if (bytesToRead < hdr.len)
			break;

		ret = s->_sock.recv(_recvPacketBuf, hdr.len, 0);
		if (ret <= hdr.len) {
			// error or disconnected
		}

		{
			BinDeserializer se(_recvPacketBuf);
			se.io(hdr);
		}
		{
			BinDeserializer se(_recvPacketBuf);
			onRecvPacket(s, hdr, se);
		}
	}

}

} // namespace