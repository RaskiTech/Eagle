#include "TestingGround.h"
#include "ImGui/imgui.h"

TestingGround::TestingGround() : Layer("TestingGround"), mCameraController(1280.0f / 720.0f, true) {
	mCameraController.SetZoom(2);
}

///////////////////// On Attach //////////////////////
void TestingGround::OnAttach() {
	EAGLE_PROFILE_FUNCTION();
	bg = Texture::Create("Assets/BG.png", false);
	mCaveTiles = Texture::Create("Assets/Atlas.png", false);
	creature = SubTexture::CreateFromIndexes(mCaveTiles, { 1, 0 }, { 16, 16 });

	// Particles
	auto mParticleProps = ParticleSystemProps();
	mParticleProps.zPosition = -0.2f;
	mParticleProps.emitAngleWidthRadiant = 6.282f;
	mParticleProps.minColor = { 0.7f, 0.7f, 0.7f, 1.0f };
	mParticleProps.maxColor = { 0.9f, 0.9f, 0.9f, 1.0f };
	mParticleProps.minSize = { 0.2f, 0.2f };
	mParticleProps.maxSize = { 0.2f, 0.2f };
	mParticleProps.sizeAtEnd = { -0.2, -0.2 };

	mParticleSystem = ParticleSystem(mParticleProps, 10000);
}

void TestingGround::OnDetach() {
	EAGLE_PROFILE_FUNCTION();
}

///////////////////// On Update //////////////////////
void TestingGround::OnUpdate() {
	EAGLE_PROFILE_FUNCTION();

	mCameraController.OnUpdate();

	Renderer::GetStats().ResetStats();

	RenderCommand::SetColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();
	
	// Emit a particle every 10 frames
	static int v = 0;
	if (v > 10) {
		mParticleSystem.Emit({ 0, 0 });
		v = 0;
	}
	v++;

	mParticleSystem.OnRender(mCameraController.GetCamera());

	{
		EAGLE_PROFILE_SCOPE("Rendering loop");


		Renderer::BeginScene(mCameraController.GetCamera());
		Renderer::DrawTextureQuad({ 0, 0, -0.3 }, { 40, 40 }, bg, 100, { 1, 1, 1, 1 });

		float xPos = glm::sin(Time::GetSeconds() * 0.5f) * 4;
		Renderer::DrawTextureQuad({ xPos, 2 }, {1, 1}, creature, 1, glm::vec4(texColor[0], texColor[1], texColor[2], texColor[3]));
		
		for (int i = 0; i < 10; i++) {
			float yPos = glm::sin(Time::GetSeconds() - i);
			Renderer::DrawRotatedColorQuad({ i - 5, yPos, -0.1f }, rotation, { 0.4f, 0.4f }, glm::vec4(color[0], color[1], color[2], color[3]));;
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

	ImGui::Begin("Example window");
	ImGui::Text("Scene editor");
	ImGui::ColorEdit4("Square color", color);
	ImGui::ColorEdit4("Texture color", texColor);
	ImGui::DragFloat("Rotation", &rotation, 0.01f, -10, 10);
	ImGui::End();
}