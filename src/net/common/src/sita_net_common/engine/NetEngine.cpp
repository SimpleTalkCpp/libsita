#include "NetEngine.h"

namespace sita {

void NetEngine::onRecv(NESocket* s) {
	if (!s) return;

	NEPacketHeader hdr;

	const size_t _maxPacketPerFrame = 100;

	_recvPacketBuf.clear();
	_recvPacketBuf.resize(_maxPacketSize);

	for (size_t i = 0; i < _maxPacketPerFrame; i++) {
		auto n = s->_sock.availableBytesToRead();
		if (n < sizeof(hdr.len)) {
			break;
		}

		int ret = s->_sock.recv(_recvPacketBuf, sizeof(hdr.len));
		if (ret <= 0) {
			// error or disconnected
		}

		

	}

}

} // namespace