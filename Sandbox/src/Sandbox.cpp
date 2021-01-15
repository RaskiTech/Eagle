#include <Eagle.h>

#include <Dependencies/ImGui.h>
#include "TestingGround.h"

using namespace Egl;

class Sandbox : public Egl::Application {
public:
	Sandbox() {
		EAGLE_PROFILE_FUNCTION();
		//AddLayer(new TestingGround());
	}

	~Sandbox() {

	}
};

//Egl::Application* Egl::CreateApplication() {
//	return new Sandbox();
//}
