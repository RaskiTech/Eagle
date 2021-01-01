#include "TestingGround.h"
#include "ImGui/imgui.h"

using namespace Egl;

TestingGround::TestingGround() : Layer("TestingGround"), mCameraController(1280.0f / 720.0f, true) {
	mCameraController.SetZoom(2);
}

void TestingGround::OnAttach() {
	EAGLE_PROFILE_FUNCTION();
	texture = Texture::Create("Assets/Water.png", false);
	bg = Texture::Create("Assets/BG.png", false);

	mParticleProps = ParticleSystemProps();
	mParticleProps.acceleration = 0.7f;
	mParticleProps.minVelocity = 2;
	mParticleProps.maxVelocity = 3;

	mParticleProps.zPosition = 0.9f;
	mParticleProps.emitAngleWidthRadiant = 6.2f;
	mParticleProps.emitDirOffset = 0;
	mParticleProps.maxLifetime = 5;
	mParticleProps.minLifeTime = 3;
	mParticleProps.maxColor = glm::vec4(0.2f, 0.3f, 0.8f, 0.1f);
	mParticleProps.minColor = glm::vec4(0.8f, 0.2f, 0.3f, 0.1f);
	mParticleProps.minSize = glm::vec2(0.02f);
	mParticleProps.maxSize = glm::vec2(0.2f);
	mParticleProps.rotationOverLifetime = 3;

	mParticleSystem = ParticleSystem(mParticleProps, 10000);
}

void TestingGround::OnDetach() {
	EAGLE_PROFILE_FUNCTION();
}

void TestingGround::OnUpdate() {
	EAGLE_PROFILE_FUNCTION();

	mParticleProps.maxColor = { particleMaxColor[0], particleMaxColor[1], particleMaxColor[2], particleMaxColor[3]};
	mParticleProps.minColor = { particleMinColor[0], particleMinColor[1], particleMinColor[2], particleMinColor[3]};
	mParticleProps.minSize =  { particleMinSize[0], particleMinSize[1] };
	mParticleProps.maxSize =  { particleMaxSize[0], particleMaxSize[1] };
	mParticleProps.sizeOverLifetime = { particleSizeAtEnd[0], particleSizeAtEnd[1] };
	mParticleSystem.SetProps(mParticleProps);

	mCameraController.OnUpdate();

	Renderer::GetStats().ResetStats();

	RenderCommand::SetColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	mParticleSystem.Emit({ 0, 0 });

	{
		EAGLE_PROFILE_SCOPE("Rendering loop");

		mParticleSystem.OnRender(mCameraController.GetCamera());

		Renderer::BeginScene(mCameraController.GetCamera());
		Renderer::DrawTextureQuad({ 0, 0, -0.3 }, { 40, 40 }, bg, 100);
		
		float xPos = glm::sin(Time::GetSeconds() * 0.3f) * 4;
		float textureRotation = Time::GetSeconds() * 0.2f;
		Renderer::DrawRotatedTextureQuad({ xPos, 0, -0.1f }, textureRotation, {1, 1}, texture, glm::vec4(texColor[0], texColor[1], texColor[2], texColor[3]));
		
		for (int i = 0; i < 10; i++) {
			float yPos = glm::sin(Time::GetSeconds() - i);
			Renderer::DrawRotatedColorQuad({ i - 5, yPos }, rotation, { 0.4f, 0.4f }, glm::vec4(color[0], color[1], color[2], color[3]));;
		}
		
		Renderer::EndScene();

	}

	LOG("Milliseconds: {0}", (float)Time::GetFrameDeltaMilliseconds());
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
	ImGui::DragFloat("Rotation", &rotation, 0.01f, -10, 10);
	ImGui::Spacing();

	ImGui::Text("Renderer Stats");
	ImGui::Text("Drawcalls: %d", Renderer::GetStats().GetDrawCallCount());
	ImGui::Text("Quads: %d", Renderer::GetStats().GetQuadCount());
	ImGui::Text("Vertices: %d", Renderer::GetStats().GetVertexCount());
	ImGui::Text("Indeces: %d", Renderer::GetStats().GetIndexCount());
	ImGui::Spacing();

	ImGui::Text("ParticleSystem");
	ImGui::DragFloat("zPosition", &mParticleProps.zPosition, 0.01f);
	ImGui::DragFloat("emitAngleWidth", &mParticleProps.emitAngleWidthRadiant, 0.01f, 0, 3.141f*2);
	ImGui::DragFloat("Emit angle offset", &mParticleProps.emitDirOffset, 0.01f, -3.141f, 3.141f);
	ImGui::DragFloat("Acceleration", &mParticleProps.acceleration, 0.01f, 0, 5);
	ImGui::DragFloat("Max lifetime", &mParticleProps.maxLifetime, 0.01f, 0, 10);
	ImGui::DragFloat("Min lifetime", &mParticleProps.minLifeTime, 0.01f, 0, 10);
	ImGui::DragFloat4("maxColor", particleMinColor, 0.01f, 0, 1);
	ImGui::DragFloat4("minColor", particleMaxColor, 0.01f, 0, 1);
	ImGui::DragFloat4("Color over lifetime", particleColorLifetime, 0.01f, 0, 1);
	ImGui::DragFloat2("min velocity", &mParticleProps.minVelocity, 0.01f, 0, 1);
	ImGui::DragFloat2("max velocity", &mParticleProps.maxVelocity, 0.01f, 0, 1);
	ImGui::DragFloat2("minSize", particleMinSize, 0.01f, 0, 1);
	ImGui::DragFloat2("maxSize", particleMaxSize, 0.01f, 0, 1);
	
	ImGui::DragFloat("Rotation in lifetime", &mParticleProps.rotationOverLifetime, 0.02f, -10, 10);

	ImGui::End();
}