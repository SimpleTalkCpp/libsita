#include "Socket.h"

namespace sita {

Socket::PlatformInit::PlatformInit() {
#if SITA_OS_WINDOWS
	WSADATA	wsad;
	if (WSAStartup(WINSOCK_VERSION, &wsad) != 0) {
		throw SITA_ERROR("WSAStartup");
	}
#endif
}

Socket::PlatformInit::~PlatformInit() {
#if SITA_OS_WINDOWS
	if (WSACleanup() != 0) {
		SITA_ASSERT(false);
	}
#endif
}

void Socket::platformInit() {
	static PlatformInit s;
}

} // namespace