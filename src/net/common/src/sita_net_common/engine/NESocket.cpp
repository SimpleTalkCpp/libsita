#include "NESocket.h"

namespace sita {

void NESocket::close() {
	_status = Status::None;
	_sock.close();
}

} // namespace