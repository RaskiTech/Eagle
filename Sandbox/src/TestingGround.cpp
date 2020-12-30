#include "TestingGround.h"
#include "ImGui/imgui.h"

using namespace Egl;

TestingGround::TestingGround() : Layer("TestingGround"), mCameraController(1280.0f / 720.0f, true) {

}

void TestingGround::OnAttach() {
	EAGLE_PROFILE_FUNCTION();
	texture = Texture::Create("Assets/Water.png", false);
}

void TestingGround::OnDetach() {
	EAGLE_PROFILE_FUNCTION();
}

void TestingGround::OnUpdate() {
	EAGLE_PROFILE_FUNCTION();

	mCameraController.OnUpdate();

	RenderCommand::SetColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	{
		EAGLE_PROFILE_SCOPE("Rendering loop");
		Renderer::BeginScene(mCameraController.GetCamera());
		Renderer::DrawQuad({ 0, 0, -0.1f }, 0, {0.4, 1}, texture);
		for (int i = 0; i < 10; i++) {
			Renderer::DrawQuad({ i, 0 }, (float)i * 10, { 0.5f, i * 0.2f }, glm::vec4(color[0], color[1], color[2], color[3]));
		}
		Renderer::EndScene();
	}
}

void TestingGround::OnEvent(Egl::Event& event) {
	EAGLE_PROFILE_FUNCTION();
	mCameraController.OnEvent(event);
}

void TestingGround::OnImGuiRender() {
	EAGLE_PROFILE_FUNCTION();
	ImGui::Begin("Color editor window");
	ImGui::ColorEdit4("Square color", color);
	ImGui::End();
}