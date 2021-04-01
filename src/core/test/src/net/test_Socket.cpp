#include <sita_core/base/UnitTest.h>
#include <sita_core/net/Socket.h>

namespace sita {

class Test_Socket : public UnitTestBase {
public:
	
	void test_resolveIPv4() {
		Vector<int> a;


		IPv4 ip;
		ip.resolve("localhost");
		SITA_DUMP_VAR(ip);
		SITA_TEST_CHECK(ip == IPv4(127,0,0,1));
		SITA_TEST_CHECK_SLIENT(ip == IPv4(127,0,0,1));
	}
};

} // namespace 

void test_Socket() {
	using namespace sita;
	SITA_TEST_CASE(Test_Socket, test_resolveIPv4());
}
