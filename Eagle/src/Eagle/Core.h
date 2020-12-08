#pragma once

#ifdef EAGLE_PLATFORM_WINDOWS
	#ifdef EAGLE_BUILD_DLL
		#define EAGLE_API __declspec(dllexport)
	#else	
		#define EAGLE_API __declspec(dllimport)
	#endif
#else
	#error Eagle only supports windows
#endif // EGL_PLATFORM_WINDOWS

#define BIT(x) (1<<x)

#ifdef EAGLE_DEBUG
	#define EAGLE_ENABLE_ASSERTS
#endif

#ifdef EAGLE_ENABLE_ASSERTS
	#define EAGLE_ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EAGLE_CORE_ASSERT(x, ...) { if (!(x)) { LOG_ENG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EAGLE_ASSERT(x, ...)
	#define EAGLE_CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)