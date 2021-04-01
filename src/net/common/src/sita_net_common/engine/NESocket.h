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

	u64 _totalRecvBytes = 0;
	u64 _totalSendBytes = 0;
};


} // namespace