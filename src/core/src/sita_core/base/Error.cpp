#include "Error.h"

namespace sita {

Error::Error(const SrcLoc& loc, StrView msg) 
: _loc(loc), _msg(msg)
{} 

} // namespace
