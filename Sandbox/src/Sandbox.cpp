#include <Eagle.h>

#include <glm/glm.hpp>
#include "ImGui/imgui.h"

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

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Test");
		ImGui::End();
	}
};

class Sandbox : public Egl::Application {
public:
	Sandbox() {
		AddLayer(new BaseLayer());
	}

	~Sandbox() {

	}
};

Egl::Application* Egl::CreateApplication() {
	return new Sandbox();
}