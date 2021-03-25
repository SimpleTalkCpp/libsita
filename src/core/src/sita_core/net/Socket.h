#pragma once

#include "SockAddr.h"

namespace sita {

// OOP - BSD socket
class Socket : public NonCopyable {
public:
	using SockLen = SockAddr::SockLen;

#ifdef SITA_OS_WINDOWS
	using socklen_t = int;
	using NativeHandle = SOCKET;

	static const NativeHandle kInvalidHandle = INVALID_SOCKET;
#else
	using NativeHandle = int;
	static const NativeHandle kInvalidHandle = -1;
#endif

	~Socket() { close(); }

	bool isValid() const { return _sock != kInvalidHandle; }

	void createUDP();
	void createTCP();
	void close();

	void setReuseAddr(bool b);
	void bind(const SockAddr& addr);
	void bind(const char* hostname, uint16_t port);

	void listen(int backlog = 64);

	bool connect(const SockAddr& addr);
	bool connect(const char* hostname, uint16_t port);

	bool accept(Socket & acceptedSocket);

	void sendto(const SockAddr& addr, const u8* data, size_t dataSize);
	void sendto_c_str(const SockAddr& addr, const char* data) { sendto(addr, reinterpret_cast<const u8*>(data), strlen(data)); }

	int send(const u8* data, size_t dataSize);
	int send(const Vector<u8> & data)	{ return send(data.data(), data.size()); }
	int send(const String & data)		{ return send(reinterpret_cast<const u8*>(data.data()), data.size()); }

	size_t send_c_str(const char* data)		{ return send(reinterpret_cast<const u8*>(data), strlen(data)); }

	template<size_t N>
	int send(const char (&sz)[N])			{ return N ? send(sz, N-1) : 0; }

	size_t availableBytesToRead();

	int recv(u8* buf, size_t bytesToRecv, int flags);
	int recv(Vector<u8> & buf, size_t bytesToRecv, int flags);
	int recv(String & buf, size_t bytesToRecv, int flags);

	int appendRecv(Vector<u8> & buf, size_t bytesToRecv, int flags);
	int appendRecv(String & buf, size_t bytesToRecv, int flags);

	int recvfrom(SockAddr& addr, Vector<u8> & buf, size_t bytesToRecv);

	void setNonBlocking(bool b);

	SOCKET sock() { return _sock; }

	static	void platformInit();

	int getSockError();

private:
	SOCKET _sock = kInvalidHandle;

	void _setsockopt(int level, int optname, const void* optval, SockLen optlen);

	class PlatformInit {
	public:
		PlatformInit();
		~PlatformInit();	
	};
};

} // namespace