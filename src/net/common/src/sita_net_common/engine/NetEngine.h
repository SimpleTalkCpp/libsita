#pragma once

#include "NESocket.h"

namespace sita {

class NetEngine : public NonCopyable {
public:

	void listen(const SockAddr& addr);
	void stopListen();

	NESocket* connect(const SockAddr& addr);

	virtual void onAccept		(NESocket* s) {}
	virtual void onConnect		(NESocket* s) {}

	virtual void onRecvPacket	(NESocket* s, const NEPacketHeader& hdr, const u8* data);
	virtual void onRecv			(NESocket* s);

private:
	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;

	Vector<u8>		_recvPacketBuf;
	Vector<NESocket*> _sockList;
};

} // namespace