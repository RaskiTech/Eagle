#include <EaglePCH.h>
#include "Application.h"
#include "Eagle/Rendering/Texture.h"

#ifdef EAGLE_PLATFORM_WINDOWS

//class LambdaBase {
//public:
//	LambdaBase() {};
//	int variable;
//};
//class LambdaDerived : public LambdaBase {
//public:
//	LambdaDerived() {};
//	int variable;
//};
//class LambdaOther : public LambdaBase {
//public:
//	LambdaOther() {};
//	int variable;
//};
//
//class LambdaTest {
//public:
//	std::function<void()> fn;
//
//	template<typename T>
//	void init() {
//		fn = [&]() { ptr = new T(); };
//	}
//
//	LambdaBase* ptr;
//};

int main(int agrc, char** argv) {
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

#endif // EAGLE_PLATFORM_WINDOWS

