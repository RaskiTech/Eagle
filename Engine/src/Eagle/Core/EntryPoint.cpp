#include <EaglePCH.h>
#include "Application.h"
#include "Eagle/Rendering/Texture.h"

#include "EagleBuildSettings.h"

inline void EagleMain() {
	Egl::Log::Init();
	LOG_ENG_INFO("Engine starting...");

	EAGLE_PROFILE_BEGIN("Engine Startup", "Eagle-Profile-Startup.json");
	auto app = new Egl::Application();
	EAGLE_PROFILE_END();

	LOG_ENG_INFO("Inizialization complete");

	EAGLE_PROFILE_BEGIN("Engine Runtime", "Eagle-Profile-Runtime.json");
	app->Run();
	EAGLE_PROFILE_END();

	EAGLE_PROFILE_BEGIN("Engine Shutdown", "Eagle-Profile-Shutdown.json");
	delete app;
	EAGLE_PROFILE_END();
}


#ifdef EAGLE_PLATFORM_WINDOWS

int main(int agrc, char** argv) {
	#if !EAGLE_SHOW_CONSOLE
	  FreeConsole();
	#endif

	EagleMain();
}

#endif

