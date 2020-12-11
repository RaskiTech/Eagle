#include <Eagle.h>

#include <glm/glm.hpp>

int foo()
{
	glm::vec4 Position = glm::vec4(glm::vec3(0.0), 1.0);
	glm::mat4 Model = glm::mat4(1.0);
	Model[4] = glm::vec4(1.0, 1.0, 0.0, 1.0);
	glm::vec4 Transformed = Model * Position;
	return 0;
}

class BaseLayer : public Egl::Layer {
public:
	BaseLayer() : Layer("BaseLayer", true) {}

	void OnUpdate() override {
		if (Egl::Input::IsKeyPressed(EGL_KEY_TAB))
			LOG("Tab is pressed");
	}

	void OnEvent(Egl::Event& event) override {
		//if (event.GetEventType() == Egl::EventType::KeyPressed)
		//	LOG("Key pressed: {0}", event.GetName());
		//LOG_INFO("Event occured: {0}", event);
	}
};

class Sandbox : public Egl::Application {
public:
	Sandbox() {
		AddLayer(new BaseLayer());
		AddOverlay(new Egl::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Egl::Application* Egl::CreateApplication() {
	return new Sandbox();
}