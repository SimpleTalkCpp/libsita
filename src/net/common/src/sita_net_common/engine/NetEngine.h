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

	virtual void onRecvPacket	(NESocket* s, const NEPacketHeader& hdr, Span<u8> data) {}
	virtual void onRecv			(NESocket* s);

	virtual void onError		(NESocket* s, const Error& error) {}

	template<class Packet>
	void sendPacket(NESocket* s, Packet& pkt);
	void disconnectAll();

	size_t listenCount		() const { return _listenSockets.size(); }
	size_t connectedCount	() const { return _sockets.size(); }

	struct BytesStatis {
		u64 lastUpdate = 0;
		u64 lastSecond = 0;
		u64 lastSecondCounting = 0;
		u64 total = 0;

		void add(int n) {
			lastUpdate += n;
			total += n;
			lastSecondCounting += n;
		}

		void _calcLastSecond(double deltaTime) {
			lastSecond = static_cast<u64>(lastSecondCounting / deltaTime);
			lastSecondCounting = 0;
		}
	};

	const BytesStatis& recvStatis() const { return _recvStatis; }
	const BytesStatis& sendStatis() const { return _sendStatis; }

private:
	void _addToPoll(Vector<UPtr<NESocket>>& list);
	void _updateSocket(NESocket* s, PollFlags pf);

	void _onAccept		(NESocket* s)	{ onAccept(s); }
	void _onConnect		(NESocket* s)	{ onConnect(s); }
	void _onDisconnect	(NESocket* s)	{ s->close(); onDisconnect(s); }
	void _onError		(NESocket* s, const Error& error) { s->close(); onError(s, error); }

	void _addRecvStatis	(NESocket* s, int n);
	void _addSendStatis	(NESocket* s, int n);

	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;

	NEPacketLen _lenKey = 0x47A1;

	Vector<UPtr<NESocket>>	_listenSockets;
	Vector<UPtr<NESocket>>	_sockets;

	Vector< NESocket* >		_pollSockets;
	Vector< PollFd >		_pollfds;

	Vector<u8>		_recvPacketBuf;
	Vector<u8>		_sendPacketBuf;

	BytesStatis		_recvStatis;
	BytesStatis		_sendStatis;

	using Clock = std::chrono::system_clock;
	Clock::time_point _statisLastSecondStart;
};

template<class Packet> inline
void NetEngine::sendPacket(NESocket* s, Packet& pkt) {
	if (!s) return;
	try {
		pkt.writeToBuffer(_sendPacketBuf, _lenKey);
		int ret = s->_sock.send(_sendPacketBuf);
		if (ret >= 0) {
			_addSendStatis(s, ret);
		}
	} catch (...) {
		_onError(s, SITA_ERROR("sendPacket"));
		throw;
	}
}

SITA_INLINE 
void NetEngine::_addRecvStatis(NESocket* s, int n) {
	s->_totalRecvBytes += n;
	_recvStatis.add(n);
}

SITA_INLINE 
void NetEngine::_addSendStatis(NESocket* s, int n) {
	s->_totalSendBytes += n;
	_sendStatis.add(n);
}


} // namespace