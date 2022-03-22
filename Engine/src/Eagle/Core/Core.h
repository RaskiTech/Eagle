#pragma once
#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		// Currently this is also defined in predefinded macros 
		//#define EAGLE_PLATFORM_WINDOWS
	#else
		#error Eagle doesn't support x86
	#endif
#else
	#error Currently Eagle supports only windows
#endif


#ifndef EAGLE_PLATFORM_WINDOWS
	#error Eagle currently only supports windows
#endif

#define BIT(x) (1<<x)

#if defined EAGLE_DEBUG
	#define EAGLE_ENABLE_ASSERTS
	#define EAGLE_ENABLE_WARNINGS
#endif

#ifdef EAGLE_ENABLE_ASSERTS
	#define EAGLE_ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Assertion Failed:", __VA_ARGS__); __debugbreak(); } }
	#define EAGLE_ENG_ASSERT(x, ...) { if (!(x)) { LOG_ENG_ERROR("Assertion Failed:", __VA_ARGS__); __debugbreak(); } }

	//#define EAGLE_ASSERT(x) { if (!(x)) { __debugbreak(); }}
	//#define EAGLE_ENG_ASSERT(x) { if (!(x)) { __debugbreak(); }}
#else
	#define EAGLE_ASSERT(x, ...)
	#define EAGLE_ENG_ASSERT(x, ...)

	//#define EAGLE_ASSERT(x)
	//#define EAGLE_ENG_ASSERT(x)
#endif

#ifdef EAGLE_ENABLE_WARNINGS
	#define EAGLE_WARNING(x, ...) { if (!(x)) { LOG_WARN("Warning:", __VA_ARGS__); } }
	#define EAGLE_ENG_WARNING(x, ...) { if (!(x)) { LOG_ENG_WARN("Warning:", __VA_ARGS__); } }
#else
	#define EAGLE_WARNING(x, ...)
	#define EAGLE_ENG_WARNING(x, ...)
#endif


#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Egl {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}