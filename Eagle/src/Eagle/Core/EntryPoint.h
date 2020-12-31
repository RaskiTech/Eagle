#pragma once
#include "Eagle/Rendering/Texture.h"

#ifdef EAGLE_PLATFORM_WINDOWS

extern Egl::Application* Egl::CreateApplication();

int main(int agrc, char** argv) {
	EAGLE_PROFILE_BEGIN("Engine Startup", "Eagle-Profile-Startup.json");
	Egl::Log::Init();
	auto app = Egl::CreateApplication();
	EAGLE_PROFILE_END();

	EAGLE_PROFILE_BEGIN("Engine Runtime", "Eagle-Profile-Runtime.json");
	app->Run();
	EAGLE_PROFILE_END();

	EAGLE_PROFILE_BEGIN("Engine Shutdown", "Eagle-Profile-Shutdown.json");
	delete app;
	EAGLE_PROFILE_END();

	//while (true);
}

#endif // EAGLE_PLATFORM_WINDOWS

