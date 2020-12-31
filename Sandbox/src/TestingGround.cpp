#include "TestingGround.h"
#include "ImGui/imgui.h"

using namespace Egl;

TestingGround::TestingGround() : Layer("TestingGround"), mCameraController(1280.0f / 720.0f) {

}

void TestingGround::OnAttach() {
	EAGLE_PROFILE_FUNCTION();
	texture = Texture::Create("Assets/Water.png", false);
	bg = Texture::Create("Assets/BG.png", false);
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
		Renderer::DrawTextureQuad({ 0, 0, -0.3 }, { 40, 40 }, bg, 100);
		Renderer::DrawTextureQuad({ 0, 0, -0.1f }, {1, 1}, texture, glm::vec4(texColor[0], texColor[1], texColor[2], texColor[3]));
		for (int i = 0; i < 10; i++)
			Renderer::DrawRotatedColorQuad({ i - 5, 0 }, 0.45f, { 0.4f, 0.4f }, glm::vec4(color[0], color[1], color[2], color[3]));

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
	ImGui::ColorEdit4("Texture color", texColor);
	ImGui::End();
}