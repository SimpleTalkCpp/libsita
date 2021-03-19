#include "Error.h"

namespace sita {

Error::Error(const SrcLoc& loc, const String& msg) 
: _loc(loc), _msg(msg) 
{} 

} // namespace
