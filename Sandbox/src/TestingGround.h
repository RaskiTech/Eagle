#pragma once
#include "Eagle.h"

using namespace Egl;

class TestingGround : public Egl::Layer {
public:
	TestingGround();
	~TestingGround() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate() override;
	virtual void OnEvent(Egl::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	CameraController mCameraController;

	//Temp
	float color[4] = { 0.8f, 0.2f, 0.3f, 1.0f };
	float texColor[4] = { 1, 1, 1, 1 };
	Ref<Texture> texture;
	Ref<Texture> bg;
	Ref<VertexArray> mVertexArray;
	Ref<Shader> mShader;
};