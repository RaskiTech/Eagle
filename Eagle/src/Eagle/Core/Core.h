#pragma once
#include <memory>

#ifdef EAGLE_PLATFORM_WINDOWS

#else
	#error Eagle currently only supports windows
#endif

#define BIT(x) (1<<x)

#ifdef EAGLE_DEBUG
	#define EAGLE_ENABLE_ASSERTS
#endif

#ifdef EAGLE_ENABLE_ASSERTS
	#define EAGLE_ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EAGLE_ENG_ASSERT(x, ...) { if (!(x)) { LOG_ENG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EAGLE_ASSERT(x, ...)
	#define EAGLE_ENG_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Egl {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}