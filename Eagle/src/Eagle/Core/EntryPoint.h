#pragma once
#include "Eagle/Rendering/Texture.h"

#ifdef EAGLE_PLATFORM_WINDOWS

extern Egl::Application* Egl::CreateApplication();

int main(int agrc, char** argv) {
	Egl::Log::Init();

	EAGLE_PROFILE_BEGIN("Engine Startup", "Eagle-Profile-Startup.json");
	auto app = Egl::CreateApplication();
	EAGLE_PROFILE_END();

	EAGLE_PROFILE_BEGIN("Engine Runtime", "Eagle-Profile-Runtime.json");
	app->Run();
	EAGLE_PROFILE_END();

	EAGLE_PROFILE_BEGIN("Engine Shutdown", "Eagle-Profile-Shutdown.json");
	delete app;
	EAGLE_PROFILE_END();
}

#endif // EAGLE_PLATFORM_WINDOWS

