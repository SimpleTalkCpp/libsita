#include <sita/core/base/UnitTest.h>
#include <sita/core/net/Socket.h>

namespace sita { namespace test {

class TestSocket : public UnitTestBase {
public:
	
	void test_resolveIPv4() {
		IPv4 ip;
		ip.resolve("localhost");
		SITA_DUMP_VAR(ip);
		SITA_TEST_CHECK(ip == IPv4(127,0,0,1));
		SITA_TEST_CHECK_SLIENT(ip == IPv4(127,0,0,1));
	}
};

}} // namespace 

void test_Socket() {
	SITA_TEST_CASE(TestSocket, test_resolveIPv4());
}
