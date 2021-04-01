#include <sita_core/base/UnitTest.h>
#include <sita_core/string/Fmt.h>

namespace sita {

class Test_Fmt : public UnitTestBase {
public:	
	void test_string() {
		String s = "hello";
		SITA_DUMP_VAR(s);
	}
};

} // namespace 

void test_Fmt() {
	using namespace sita;
	SITA_TEST_CASE(Test_Fmt, test_string());
}
