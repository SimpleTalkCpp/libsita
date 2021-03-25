#pragma once

#include <sita_core/base/sita_base.h>
#include <sita_core/log/Log.h>

#define	SITA_TEST_CASE(TestClass, TestFunc) \
	do{ \
		SITA_LOG("\n[--TEST_CASE--] " #TestClass "." #TestFunc "\n"); \
		sita::test::TestClass testObj; \
		testObj.TestFunc; \
	}while(false)
//----

#define SITA_TEST_CHECK(a)			SITA_TEST_CHECK_IMPL(SITA_SRC_LOC, true,  bool(a), #a)
#define SITA_TEST_CHECK_SLIENT(a)	SITA_TEST_CHECK_IMPL(SITA_SRC_LOC, false, bool(a), #a)

namespace sita {

bool SITA_TEST_CHECK_IMPL(const SrcLoc& loc, bool verbose, bool success, const char* msg);

class UnitTestBase : public NonCopyable {
public:
};

}