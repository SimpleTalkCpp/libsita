#include <sita_core/base/UnitTest.h>
#include <sita_core/base/BinSerializer.h>
#include <sita_core/base/BinDeserializer.h>

namespace sita { namespace test {

class Test_BinSerializer : public UnitTestBase {
public:

	template<class T>
	void test_fixed(u64 n, u64 step) {
		Vector<u8> buf;
		{
			BinSerializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t = static_cast<T>(i);
				se.io_fixed(t);
			}
		}

//		SITA_DUMP_HEX(buf);

		{
			BinDeserializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t;
				se.io_fixed(t);
				SITA_TEST_CHECK_SLIENT(t == static_cast<T>(i));
			}
		}
	}

	template<class T>
	void test_vary(u64 n, u64 step) {
		Vector<u8> buf;
		{
			BinSerializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t = static_cast<T>(i);
				se.io_vary(t);
			}
		}

//		SITA_DUMP_HEX(buf);

		{
			BinDeserializer se(buf);
			for (u64 i = 0; i < n; i+=step) {
				T t;
				se.io_vary(t);
				SITA_TEST_CHECK_SLIENT(t == static_cast<T>(i));
			}
		}
	}

};

}} // namespace 

void test_BinSerializer() {
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_fixed<u64>(0xFFFFFFFFFFFFFFFFULL, 1));

	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_fixed<i64>(0xFFFFFFFFFFFFFFFFULL, 1));

//----------
	SITA_TEST_CASE(Test_BinSerializer, test_vary<u8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<u16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<u32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_vary<u64>(0xFFFFFFFFFFFFFFFFULL, 1));

	SITA_TEST_CASE(Test_BinSerializer, test_vary<i8 >(0x100ULL,              1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<i16>(0x10000ULL,            1));
	SITA_TEST_CASE(Test_BinSerializer, test_vary<i32>(0x100000000ULL,        0xF1));
//	SITA_TEST_CASE(Test_BinSerializer, test_vary<i64>(0xFFFFFFFFFFFFFFFFULL, 1));


}
