#pragma once

#include "SockAddr.h"

namespace sita {

class Socket : public NonCopyable {
public:



	static	void platformInit();
private:
	class PlatformInit {
	public:
		PlatformInit();
		~PlatformInit();
	};
};

} // namespace