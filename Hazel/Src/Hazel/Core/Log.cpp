#include "hzpch.h"
#include "Log.h"

#include "Hazel\ImGui\ImGuiConsoleSink.h"

#include "spdlog\sinks\stdout_color_sinks.h"

namespace Hazel {

	std::shared_ptr<spdlog::logger> Log::ms_coreLogger;
	std::shared_ptr<spdlog::logger> Log::ms_clientLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()); /* VS debug console */
		sinks.emplace_back(std::make_shared<ImGuiConsoleSink_mt>(true));  /* ImGuiConsole */

		// ms_coreLogger = spdlog::stdout_color_mt("HAZEL");
		// ms_clientLogger = spdlog::stdout_color_mt("APP");
		ms_coreLogger = std::make_shared<spdlog::logger>("HAZEL", begin(sinks), end(sinks));
		spdlog::register_logger(ms_coreLogger);
		ms_clientLogger = std::make_shared<spdlog::logger>("APP", begin(sinks), end(sinks));
		spdlog::register_logger(ms_clientLogger);

		spdlog::set_pattern("%^[%T] %n: %v%$");
		ms_coreLogger->set_level(spdlog::level::trace);
		ms_clientLogger->set_level(spdlog::level::trace);
	}

}