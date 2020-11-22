#pragma once

#ifdef EAGLE_PLATFORM_WINDOWS

extern Egl::Application* Egl::CreateApplication();

int main(int agrc, char** argv) {
	auto app = Egl::CreateApplication();
	app->Run();
	delete app;
}

#endif // EAGLE_PLATFORM_WINDOWS

