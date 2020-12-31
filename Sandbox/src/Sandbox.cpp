#include <Eagle.h>
#include <Eagle/Core/EntryPoint.h>

#include "ImGui/imgui.h"
#include "TestingGround.h"

using namespace Egl;

class Sandbox : public Egl::Application {
public:
	Sandbox() {
		EAGLE_PROFILE_FUNCTION();
		AddLayer(new TestingGround());
	}

	~Sandbox() {

	}
};

Egl::Application* Egl::CreateApplication() {
	return new Sandbox();
}
