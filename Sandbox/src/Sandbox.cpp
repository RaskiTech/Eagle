#include <Eagle.h>

class Sandbox : public Egl::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Egl::Application* Egl::CreateApplication() {
	return new Sandbox();
}