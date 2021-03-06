#pragma once

#include <sita_core/base/sita_base.h>
#include <sita_core/string/Fmt.h>
#include <sita_core/string/String.h>

#define SITA_LOG(...)       do{ sita::g_log.write(sita::Log::Level::Info,    __VA_ARGS__); } while(false)
#define SITA_LOG_WARN(...)  do{ sita::g_log.write(sita::Log::Level::Warning, __VA_ARGS__); } while(false)
#define SITA_LOG_ERROR(...) do{ sita::g_log.write(sita::Log::Level::Error,   __VA_ARGS__); } while(false)

namespace sita {

class Log : public NonCopyable {
public:

	enum class Level {
		Unknown,
		Info,
		Warning,
		Error,
	};

	template<class... Args>
	void write(Level lv, const Args&... args) {
		String s = fmt::format(SITA_FORWARD(args)...);
		onWrite(lv, s);
	}

	void onWrite(Level lv, StrView str);
};

extern Log g_log;

} //namespace


