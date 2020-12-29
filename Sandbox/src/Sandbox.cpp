#include <Eagle.h>
#include <Eagle/Core/EntryPoint.h>

#include "ImGui/imgui.h"
#include "TestingGround.h"

using namespace Egl;

class Timer {
public:
	Timer(const char* name) : mName(name), mStopped(false) {
		mStartPoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		if (!mStopped)
			Stop();
	}
	void Stop() {
		mStopped = true;

		auto endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartPoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
		float duration = (end - start) * 0.001f;

	}


private:
	const char* mName;
	bool mStopped;
	std::chrono::time_point<std::chrono::steady_clock> mStartPoint;
};

class Sandbox : public Egl::Application {
public:
	Sandbox() {
		AddLayer(new TestingGround());
	}

	~Sandbox() {

	}
};

Egl::Application* Egl::CreateApplication() {
	return new Sandbox();
}

class BaseLayer : public Layer {
public:
	BaseLayer() : Layer("BaseLayer", true) 
	{
		{
			float vertices[6] = {
				-0.8f, -0.8f,
				 0.8f, -0.8f,
				 0.0f,    0.6f
			};
			Ref<VertexBuffer> buffer;
			buffer.reset(new VertexBuffer(vertices, sizeof(vertices)));
		}
	}

	void OnUpdate() override {

	}

	void OnEvent(Egl::Event& event) override {

	}
};