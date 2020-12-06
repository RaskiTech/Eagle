#include <Eagle.h>

class BaseLayer : public Egl::Layer {
public:
	BaseLayer() : Layer("BaseLayer", false) {}

	void OnUpdate() override {
		LOG_INFO("Base layer update");
	}

	void OnEvent(Egl::Event& event) override {
		LOG_INFO("{0}", event);
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