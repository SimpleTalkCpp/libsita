#include "NetEngine.h"

namespace sita {

void NetEngine::update(int timeoutMilliseconds) {
	const auto totalSockCount = _listenSockets.size() + _sockets.size();
	_pollfds.clear();
	_pollfds.reserve(totalSockCount);

	_pollSockets.clear();
	_pollSockets.reserve(totalSockCount);
	
	_addToPoll(_listenSockets);
	_addToPoll(_sockets);

	if (_pollfds.size() <= 0)
		return;

	int pollRet = Poll(_pollfds, timeoutMilliseconds);
	if (pollRet < 0) {
		SITA_ASSERT(false);
		return;
	}

	{
		SITA_ASSERT(_pollSockets.size() == _pollfds.size());

		auto* pf    = _pollfds.begin();

		for (auto* s : _pollSockets) {
			if (pf->revents && s) {
				try {
					_updateSocket(s, PollFlags(pf->revents));
				} catch (const Error& err) {
					_onError(s, err);
				} catch (...) {
					throw;
				}
			}
			pf++;
		}
	}
}

void NetEngine::_addToPoll(Vector<UPtr<NESocket>>& list) {
	auto it = list.begin();

	while (it != list.end()) {
		auto& s = *it;

		if (!s || !s->_sock.isValid() || s->_status == NESocket::Status::None) {
			it = list.erase_unsorted(it); // destroy closed sockets;
			continue;
		}

		PollFlags ev = PollFlags::None;
		switch (s->_status) {
			case NESocket::Status::Listening:	{ ev = PollFlags::In;    } break;
			case NESocket::Status::Connecting:	{ ev = PollFlags::InOut; } break;
			case NESocket::Status::Connected:	{ ev = PollFlags::InOut; } break;
		}

		_pollSockets.emplace_back(s.get());

		auto& pfd = _pollfds.emplace_back();
		pfd.fd = s->_sock.nativeHandle();
		pfd.events = enumInt(ev);
		pfd.revents = 0;

		++it;
	}	
}

void NetEngine::_updateSocket(NESocket* s, PollFlags pf) {
	switch (s->_status) {
		case NESocket::Status::Listening: {
			if (enumHas(pf, PollFlags::In)) {
				auto& newSock = _sockets.emplace_back(new NESocket());
				if (!s->_sock.accept(newSock->_sock)) {
					newSock->close();
					return;
				}

				newSock->_sock.setNonBlocking(true);
				newSock->_status = NESocket::Status::Connected;
				_onAccept(newSock.get());
				return;
			}
		} break;

		case NESocket::Status::Connecting: {
			if (enumHas(pf, PollFlags::Hup | PollFlags::Error)) {
				_onError(s, SITA_ERROR("error connect"));
				return;
			}
		} break;

		case NESocket::Status::Connected: {
			if (enumHas(pf, PollFlags::Hup | PollFlags::Error)) {
				_onDisconnect(s);
				return;
			}

			if (enumHas(pf, PollFlags::In)) {
				if (s->_sock.availableBytesToRead() == 0) {
					_onDisconnect(s);
					return;
				}

				try {
					onRecv(s);
				} catch (...) {
					s->close();
				}
			}

			if (enumHas(pf, PollFlags::Out)) {
				// ready to send
			}			
		} break;
	}
}

void NetEngine::listen(const SockAddr& addr, int backlog) {
	auto& s = _listenSockets.emplace_back(new NESocket());
	try {
		s->_sock.createTCP();
		s->_sock.setNonBlocking(true);
		s->_sock.bind(addr);
		s->_sock.listen(backlog);
		s->_status = NESocket::Status::Listening;
	} catch (...) {
		s->close();
		throw;
	}
}

void NetEngine::listen(StrView hostname, u16 port, int backlog) {
	SockAddr addr;
	addr.resolve(hostname, port);
	SITA_LOG("Listen {}", addr);
	listen(addr, backlog);
}

void NetEngine::stopListen() {
	SITA_LOG("Stop Listen");
	
	for (auto& s : _listenSockets) {
		if (s) s->close();
	}
}

NESocket* NetEngine::connect(const SockAddr& addr) {
	auto& c = _sockets.emplace_back(new NESocket());
	c->_sock.createTCP();
	c->_sock.setNonBlocking(true);
	c->_status = NESocket::Status::Connecting;

	try {
		if (c->_sock.connect(addr)) {
			c->_status = NESocket::Status::Connected;
			onConnect(c.get());
		}
	} catch(...) {
		c->close();
	}
	return c.get();
}

NESocket* NetEngine::connect(StrView hostname, u16 port) {
	SockAddr addr;
	addr.resolve(hostname, port);
	return connect(addr);
}

void NetEngine::disconnectAll() {
	for (auto& s : _sockets) {
		if (s) s->close();
	}
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