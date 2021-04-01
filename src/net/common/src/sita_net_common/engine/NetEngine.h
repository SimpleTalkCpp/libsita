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

	virtual void onRecvPacket	(NESocket* s, const NEPacketHeader& hdr, const u8* data) {}
	virtual void onRecv			(NESocket* s);

	template<class Packet>
	void sendPacket(NESocket* s, Packet& pkt) {
		_sendPacketBuf.clear();
		BinSerializer se(_sendPacketBuf);
		se.io(pkt);
		send(s. buf.data(), buf.size());
	}

	void send(NESocket* s, const u8* data, size_t dataSize);

private:
	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;

	Vector<u8>		_recvPacketBuf;
	Vector<u8>		_sendPacketBuf;
	Vector< UPtr<NESocket> > _sockList;
};

} // namespace