#include "EaglePCH.h"

namespace Egl {

	std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		EAGLE_PROFILE_FUNCTION();
		spdlog::set_pattern("[%n - %T] %^%v%$");
		s_EngineLogger = spdlog::stdout_color_mt("Eagle");
		s_EngineLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}