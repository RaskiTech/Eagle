#pragma once

#ifdef EAGLE_PLATFORM_WINDOWS

extern Egl::Application* Egl::CreateApplication();

int main(int agrc, char** argv) {
	Egl::Log::Init(); // THIS NEEDS TO BE BEFORE CREATEAPP
	LOG_ENG_INFO("ENGINE STARTING...");
	auto app = Egl::CreateApplication();
	LOG_INFO("Client initialized.");
	app->Run();
	delete app;
}

#endif // EAGLE_PLATFORM_WINDOWS

