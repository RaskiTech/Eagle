#pragma once
#include <Dependencies/Entt.h>

#define LOGR_INCLUDE_GLM_TYPES
#include <LogR.h>

///////////////////////////////////////
// Custom types for the Eagle Engine //
///////////////////////////////////////

template<> inline void LogR::Logger::TypeToString(std::ostream& os, entt::entity& type) {
	os << (uint32_t)type;
}

///////////////////////////////////////

namespace Egl {
	class Log {
	private:
		static LogR::Logger engineLogger;
		static LogR::Logger clientLogger;
	public:
		static void Init();

		inline static LogR::Logger& GetEngineLogger() { return engineLogger; }
		inline static LogR::Logger& GetClientLogger() { return clientLogger; }
	};
}

// Engine log macros
#ifdef EAGLE_DIST
#define LOG_ENG_TRACE(...) 
#define LOG_ENG_ERROR(...) 
#define LOG_ENG_WARN(...)  
#define LOG_ENG_INFO(...)  
#define LOG_ENG(...)  

#define LOG_TRACE(...)
#define LOG_ERROR(...)
#define LOG_WARN(...) 
#define LOG_INFO(...) 
#define LOG(...)
#else
#define LOG_ENG_TRACE(...) ::Egl::Log::GetEngineLogger().Log(__VA_ARGS__)
#define LOG_ENG_ERROR(...) ::Egl::Log::GetEngineLogger().Error(__VA_ARGS__)
#define LOG_ENG_WARN(...)  ::Egl::Log::GetEngineLogger().Warning(__VA_ARGS__)
#define LOG_ENG_INFO(...)  ::Egl::Log::GetEngineLogger().Success(__VA_ARGS__)
#define LOG_ENG(...)  LOG_ENG_TRACE(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...) ::Egl::Log::GetClientLogger().Log(__VA_ARGS__)
#define LOG_ERROR(...) ::Egl::Log::GetClientLogger().Error(__VA_ARGS__)
#define LOG_WARN(...)  ::Egl::Log::GetClientLogger().Warning(__VA_ARGS__)
#define LOG_INFO(...)  ::Egl::Log::GetClientLogger().Success(__VA_ARGS__)
#define LOG(...)  LOG_TRACE(__VA_ARGS__)
#endif