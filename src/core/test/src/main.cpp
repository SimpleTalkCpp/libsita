#include <sita_core/base/UnitTest.h>
#include <sita_core/log/Log.h>

// SITA_OPTIMIZE_OFF

#define	RUN_TEST( fn )	\
	SITA_LOG("\n===== " #fn " ================================\n"); \
	void fn(); \
	fn(); \
//----

namespace sita {

void run_temp_test() {
	RUN_TEST(test_Fmt);
}

void run_all_test() {
// base
	RUN_TEST(test_BinSerializer);

// string
	RUN_TEST(test_Fmt);

// net
	RUN_TEST(test_Socket);
}

int test_main() {
#if 0
	run_temp_test();
#else
	run_all_test();
#endif
	// TODO: check memory leak

	SITA_LOG("\n\n==== Program Ended ==== \n");
	return 0;
}

} // namespace

int main() {
	return sita::test_main();
}
