#include <sita_core/base/UnitTest.h>
#include <sita_core/base/BinSerializer.h>
#include <sita_core/base/BinDeserializer.h>

namespace sita { namespace test {

class Test_BinSerializer : public UnitTestBase {
public:

	void test1() {
		Vector<u8> buf;
		BinSerializer se(buf);

		for (size_t i = 0; i < 16; i++) {
			u32 t = 0x12345678;
			se.io(t);
		}

		SITA_DUMP_HEX(buf);

	}
};

}} // namespace 

void test_BinSerializer() {
	SITA_TEST_CASE(Test_BinSerializer, test1());
}
