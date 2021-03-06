#pragma once

// Disable warninigs that are coused by spdlog 
#pragma warning( push )
#pragma warning( disable : 26451 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 6385 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 6387 )

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

#pragma warning( pop )

namespace Egl {

	class Log
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
#ifdef EAGLE_DIST
	#define LOG_ENG_TRACE(...) 
	#define LOG_ENG_ERROR(...) 
	#define LOG_ENG_WARN(...)  
	#define LOG_ENG_INFO(...)  
	#define LOG_ENG_FATAL(...) 
	#define LOG_ENG(...)  
	
	#define LOG_TRACE(...)
	#define LOG_ERROR(...)
	#define LOG_WARN(...) 
	#define LOG_INFO(...) 
	#define LOG_FATAL(...)
	#define LOG(...)
#else
	#define LOG_ENG_TRACE(...) ::Egl::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define LOG_ENG_ERROR(...) ::Egl::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define LOG_ENG_WARN(...)  ::Egl::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define LOG_ENG_INFO(...)  ::Egl::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define LOG_ENG_FATAL(...) ::Egl::Log::GetEngineLogger()->critical(__VA_ARGS__)
	#define LOG_ENG(...)  LOG_ENG_TRACE(__VA_ARGS__)
	
	// Client log macros
	#define LOG_TRACE(...) ::Egl::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define LOG_ERROR(...) ::Egl::Log::GetClientLogger()->error(__VA_ARGS__)
	#define LOG_WARN(...)  ::Egl::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define LOG_INFO(...)  ::Egl::Log::GetClientLogger()->info(__VA_ARGS__)
	#define LOG_FATAL(...) ::Egl::Log::GetClientLogger()->critical(__VA_ARGS__)
	#define LOG(...)  LOG_TRACE(__VA_ARGS__)
#endif