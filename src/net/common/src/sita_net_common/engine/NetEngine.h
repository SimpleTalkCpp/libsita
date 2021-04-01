#pragma once

#include "NESocket.h"

namespace sita {

class NetEngine : public NonCopyable {
public:

	void listen(const SockAddr& addr);
	void stopListen();

	void update();

	NESocket* connect(const SockAddr& addr);

	virtual void onAccept		(NESocket* s) {}
	virtual void onConnect		(NESocket* s) {}
	virtual void onDisconnect	(NESocket* s) {}

	virtual void onRecvPacket	(NESocket* s, const NEPacketHeader& hdr, BinDeserializer& se) {}
	virtual void onRecv			(NESocket* s);

	template<class Packet>
	void sendPacket(NESocket* s, Packet& pkt);

private:
	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;

	Vector<u8>		_recvPacketBuf;
	Vector<u8>		_sendPacketBuf;
	Vector< UPtr<NESocket> > _sockList;
};

template<class Packet> inline
void NetEngine::sendPacket(NESocket* s, Packet& pkt) {
	if (!s) return;
	_sendPacketBuf.clear();
	BinSerializer se(_sendPacketBuf);
	se.io(pkt);
	s->send(buf);
}

} // namespace