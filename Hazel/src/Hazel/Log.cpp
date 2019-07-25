#include "hzpch.h"
#include "Log.h"

#include "spdlog\sinks\stdout_color_sinks.h"

namespace Hazel {

	std::shared_ptr<spdlog::logger> Log::ms_coreLogger;
	std::shared_ptr<spdlog::logger> Log::ms_clientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		ms_coreLogger = spdlog::stdout_color_mt("HAZEL");
		ms_coreLogger->set_level(spdlog::level::trace);
		  
		ms_clientLogger = spdlog::stdout_color_mt("APP");
		ms_clientLogger->set_level(spdlog::level::trace);
	}

}