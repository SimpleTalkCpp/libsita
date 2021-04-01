#pragma once

#include "NEPacket.h"

namespace sita {

class NetEngine;

class NESocket : public NonCopyable {
public:
	~NESocket() {}

	enum class Status {
		None,
		Listening,
		Connecting,
		Connected,
	};

	void setUserData(void* p) { _userData = p; }
	void* userData() { return _userData; }

	void close();

friend class NetEngine;
protected:

	Socket _sock;
	void* _userData = nullptr;

	Status	_status = Status::None;

	size_t _totalRecvBytes = 0;
	size_t _totalSendBytes = 0;
};


} // namespace