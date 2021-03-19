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

int Socket::getSockError() {
#if SITA_OS_WINDOWS
	int ret = WSAGetLastError();
#else
	int ret = errno;
#endif
	return ret;
}

void Socket::createUDP() {
	close();
	platformInit();

	_sock = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (_sock == kInvalidHandle) {
		throw SITA_ERROR("createUDP");
	}
}

void Socket::createTCP() {
	close();
	platformInit();

	_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == kInvalidHandle) {
		throw SITA_ERROR("createTCP");
	}
}

void Socket::setReuseAddr(bool b) {
	int a = b ? 1 : 0;
#if !SITA_OS_WINDOWS
	_setsockopt(SOL_SOCKET, SO_REUSEPORT, &a, sizeof(a));
#endif
	_setsockopt(SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a));
}

void Socket::_setsockopt(int level, int optname, const void* optval, SockLen optlen) {
	if (!isValid()) return;

	int ret = ::setsockopt(_sock, level, optname, (const char*)optval, optlen );
	if (ret != 0)
		throw SITA_ERROR("setsockopt");
}

void Socket::bind(const char* hostname, uint16_t port) {
	SockAddr addr;
	addr.resolve(hostname, port);
	return bind(addr);
}

void Socket::bind(const SockAddr& addr) {
	if (!isValid()) return;

	int ret = ::bind(_sock, &addr._addr, sizeof(addr._addr));
	if (ret != 0) {
		throw SITA_ERROR("bind");
	}
}

void Socket::listen(int backlog) {
	if (!isValid()) return;

	int ret = ::listen(_sock, backlog);
	if (ret != 0) {
		throw SITA_ERROR("listen");
	}
}

bool Socket::connect(const SockAddr& addr) {
	if (!isValid()) return false;

	int ret = ::connect(_sock, &addr._addr, sizeof(addr._addr));
	if (ret < 0) {
#ifdef SITA_OS_WINDOWS
		int e = WSAGetLastError();
		if (e == WSAEWOULDBLOCK) // connect in non-blocking mode
			return false;
#else
		int e = errno;
		if (e == EINPROGRESS) // connect in non-blocking mode
			return false;
#endif
		throw SITA_ERROR("connect");
	}
	return true;
}

bool Socket::connect(const char* hostname, uint16_t port) {
	SockAddr addr;
	addr.resolve(hostname, port);
	return connect(addr);
}

bool Socket::accept(Socket & acceptedSocket) {
	acceptedSocket.close();

	auto c = ::accept(_sock, nullptr, nullptr);
	if (c == kInvalidHandle)
		return false;

	acceptedSocket._sock = c;
	return true;
}

void Socket::sendto(const SockAddr& addr, const u8* data, size_t dataSize) {
	if (dataSize > static_cast<size_t>(std::numeric_limits<int>::max()))
		throw SITA_ERROR("send dataSize is too big");

	int ret =::sendto(_sock, reinterpret_cast<const char*>(data), static_cast<int>(dataSize), 0, &addr._addr, sizeof(addr._addr));
	if (ret < 0) {
		throw SITA_ERROR("send");
	}
}

int Socket::send(const u8* data, size_t dataSize) {
	if (!isValid()) return 0;

	if (dataSize > static_cast<size_t>(std::numeric_limits<int>::max()))
		throw SITA_ERROR("send dataSize is too big");

	int ret =::send(_sock, reinterpret_cast<const char*>(data), static_cast<int>(dataSize), 0);
	return ret;
}

size_t Socket::availableBytesToRead() {
	if (!isValid()) return 0;

#ifdef _WIN32
	u_long n = 0;
	if (0 != ::ioctlsocket(_sock, FIONREAD, &n))
		throw SITA_ERROR("availableBytesToRead");
	return static_cast<size_t>(n);
#else
	int n = 0;
	if (0 != ::ioctl(_sock, FIONREAD, &n))
		throw SITA_ERROR("availableBytesToRead");
	return static_cast<size_t>(n);
#endif
}

int Socket::recvfrom(SockAddr& addr, Vector<u8> & buf, size_t bytesToRecv) {
	buf.clear();
	if (bytesToRecv > static_cast<size_t>(std::numeric_limits<int>::max()))
		throw SITA_ERROR("recv bytesToRecv is too big");

	buf.resize(bytesToRecv);
	socklen_t addrLen = sizeof(addr._addr);	
	int ret = ::recvfrom(_sock, reinterpret_cast<char*>(buf.data()), static_cast<int>(bytesToRecv), 0, &addr._addr, &addrLen);
	return ret;
}

void Socket::setNonBlocking(bool b)
{
#ifdef _WIN32
	u_long v = b ? 1 : 0;
	if (0 != ::ioctlsocket(_sock, FIONBIO, &v))
		throw SITA_ERROR("setNonBlocking");
#else
	long v = b ? 1 : 0;
	if (0 != ::ioctl(_sock, FIONBIO, &v))
		throw SITA_ERROR("setNonBlocking");
#endif
}

int Socket::recv(Vector<u8> & buf, size_t bytesToRecv) {
	buf.clear();
	return appendRecv(buf, bytesToRecv);
}

int Socket::recv(String & buf, size_t bytesToRecv) {
	buf.clear();
	return appendRecv(buf, bytesToRecv);
}

int Socket::appendRecv(Vector<u8> & buf, size_t bytesToRecv) {
	if (bytesToRecv > static_cast<size_t>(std::numeric_limits<int>::max()))
		throw SITA_ERROR("recv bytesToRecv is too big");

	auto oldSize = buf.size();
	auto newSize = oldSize + bytesToRecv;
	if (newSize < oldSize)
		throw SITA_ERROR("vector size > size_t");

	buf.resize(newSize);
	int ret = recv(&buf[oldSize], bytesToRecv);
	if (ret <= 0) {
		buf.resize(oldSize);
		return ret;
	}

	buf.resize(oldSize + ret);
	return ret;
}

int Socket::appendRecv(String & buf, size_t bytesToRecv) {
	if (bytesToRecv > static_cast<size_t>(std::numeric_limits<int>::max()))
		throw SITA_ERROR("recv bytesToRecv is too big");

	auto oldSize = buf.size();
	auto newSize = oldSize + bytesToRecv;
	if (newSize < oldSize)
		throw SITA_ERROR("vector size > size_t");

	buf.resize(newSize);
	int ret = recv(reinterpret_cast<u8*>(&buf[oldSize]), bytesToRecv);
	if (ret <= 0) {
		buf.resize(oldSize);
		return ret;
	}

	buf.resize(oldSize + ret);
	return ret;
}

int Socket::recv(u8* buf, size_t bytesToRecv) {
	if (!isValid()) return 0;

	if (bytesToRecv > static_cast<size_t>(std::numeric_limits<int>::max()))
		throw SITA_ERROR("recv bytesToRecv is too big");
	int ret = ::recv(_sock, reinterpret_cast<char*>(buf), static_cast<int>(bytesToRecv), 0);
	return ret;
}

void Socket::close() {
	if (_sock != kInvalidHandle) {
		#ifdef _WIN32
			::closesocket(_sock);
		#else
			::close(_sock);
		#endif
		_sock = kInvalidHandle;
	}
}

} // namespace