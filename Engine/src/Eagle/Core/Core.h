#pragma once
#include <Eagle/../../../Application/EagleBuildSettings.h>
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


#if !EAGLE_PLATFORM_WINDOWS
	#error Eagle currently only supports windows
#endif



///////////////////////////////////////////////
// What is defined in certain configurations //
///////////////////////////////////////////////

#ifdef EAGLE_DEBUG
	#define EAGLE_ENABLE_ASSERTS 1
	#define EAGLE_ENABLE_WARNINGS 1
#endif


//////////////////////////////////////
// Handle configuration definitions //
//////////////////////////////////////

#if EAGLE_ENABLE_ASSERTS
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


#if EAGLE_ENABLE_WARNINGS
	#define EAGLE_WARNING(x, ...) { if (!(x)) { LOG_WARN("Warning:", __VA_ARGS__); } }
	#define EAGLE_ENG_WARNING(x, ...) { if (!(x)) { LOG_ENG_WARN("Warning:", __VA_ARGS__); } }
#else
	#define EAGLE_WARNING(x, ...)
	#define EAGLE_ENG_WARNING(x, ...)
#endif

#if EAGLE_EDITOR
	// Statements that only execute when the editor is present
	#define EAGLE_EDITOR_ONLY(x) {x;}
#else
	#define EAGLE_EDITOR_ONLY(x)
#endif


#if EAGLE_ENABLE_IMGUI_ON_STANDALONE
	#define EAGLE_ENABLE_IMGUI 1
#else
	#if EAGLE_EDITOR
		#define EAGLE_ENABLE_IMGUI 1
	#endif
#endif




#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIT(x) (1<<x)

namespace Egl {

	// Mostly used for rendering stuff. Could be replaced with the Assets class, but do we want the possibility that the user uses them?
	// Maybe yes for shaders, or maybe replace all with just shared pointers (what they really are anyways)
	// Maybe yes because we want to track memory

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