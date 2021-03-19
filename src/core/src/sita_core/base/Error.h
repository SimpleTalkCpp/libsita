#pragma once

#include "../string/String.h"
#include "../string/Fmt.h"

#define SITA_ERROR(...) Error(SITA_SRC_LOC, fmt::format(__VA_ARGS__));

namespace sita {

class IError : public std::exception {
public:

};

class Error : public IError {
public:
//	Error() = default;
	Error(const SrcLoc& loc, const String& msg);

private:
	SrcLoc _loc;
	String _msg;
};


} // namespace