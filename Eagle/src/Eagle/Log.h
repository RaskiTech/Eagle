#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Egl {

	class EAGLE_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() {
			return s_EngineLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}
	};
}

// Engine log macros
// ... takes any number of arguments and __VA_ARGS__ pushes those arguments foward	
#define LOG_ENG_TRACE(...) ::Egl::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENG_ERROR(...) ::Egl::Log::GetEngineLogger()->error(__VA_ARGS__)
#define LOG_ENG_WARN(...)  ::Egl::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENG_INFO(...)  ::Egl::Log::GetEngineLogger()->info(__VA_ARGS__)
#define LOG_ENG_FATAL(...) ::Egl::Log::GetEngineLogger()->fatal(__VA_ARGS__)
#define LOG_ENG(...)  LOG_ENG_TRACE(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...) ::Egl::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_ERROR(...) ::Egl::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)  ::Egl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)  ::Egl::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_FATAL(...) ::Egl::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define LOG(...)  LOG_TRACE(__VA_ARGS__)