#pragma once

#include "NESocket.h"

namespace sita {

class NetEngine : public NonCopyable {
public:
	virtual ~NetEngine() {}

	void listen(const SockAddr& addr, int backlog = 64);
	void listen(StrView hostname, u16 port, int backlog = 64);

	void stopListen();

	void update(int timeoutMilliseconds = 0);

	NESocket* connect(const SockAddr& addr);
	NESocket* connect(StrView hostname, u16 port);

	virtual void onAccept		(NESocket* s) {}
	virtual void onConnect		(NESocket* s) {}
	virtual void onDisconnect	(NESocket* s) {}

	virtual void onRecvPacket	(NESocket* s, const NEPacketHeader& hdr, BinDeserializer& se) {}
	virtual void onRecv			(NESocket* s);

	virtual void onError		(NESocket* s, const Error& error) {}

	template<class Packet>
	void sendPacket(NESocket* s, Packet& pkt);
	void disconnectAll();

	size_t listenCount		() const { return _listenSockets.size(); }
	size_t connectedCount	() const { return _sockets.size(); }

private:
	void _addToPoll(Vector<UPtr<NESocket>>& list);
	void _updateSocket(NESocket* s, PollFlags pf);

	void _onAccept		(NESocket* s)	{ onAccept(s); }
	void _onConnect		(NESocket* s)	{ onConnect(s); }
	void _onDisconnect	(NESocket* s)	{ s->close(); onDisconnect(s); }
	void _onError		(NESocket* s, const Error& error) { s->close(); onError(s, error); }

	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;

	Vector<UPtr<NESocket>>	_listenSockets;
	Vector<UPtr<NESocket>>	_sockets;

	Vector< NESocket* >		_pollSockets;
	Vector< PollFd >		_pollfds;

	Vector<u8>		_recvPacketBuf;
	Vector<u8>		_sendPacketBuf;
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