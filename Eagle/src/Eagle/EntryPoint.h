#pragma once

#ifdef EAGLE_PLATFORM_WINDOWS

extern Egl::Application* Egl::CreateApplication();

int main(int agrc, char** argv) {
	auto app = Egl::CreateApplication();
	Egl::Log::Init();
	LOG_ENG_WARN("Warning");
	LOG_INFO("client {0}", 3);
	app->Run();
	delete app;
}

#endif // EAGLE_PLATFORM_WINDOWS

