#include "EditorLayer.h"
#include "ImGui/imgui.h"

namespace Egl {

	EditorLayer::EditorLayer() : Layer("EditorLayer"), mCameraController(1280.0f / 720.0f, true) {
		mCameraController.SetZoom(2);
	}

	///////////////////// On Attach //////////////////////
	void EditorLayer::OnAttach() {
		EAGLE_PROFILE_FUNCTION();
		bg = Texture::Create("Assets/BG.png", false);
		mCaveTiles = Texture::Create("Assets/Atlas.png", false);
		creature = SubTexture::CreateFromIndexes(mCaveTiles, { 1, 0 }, { 16, 16 });

		FrameBufferDefenition defenition;
		defenition.width = 1280;
		defenition.height = 720;
		mFrameBuffer = FrameBuffer::Create(defenition);

		// Particles
		mParticleProps = ParticleSystemProps();
		mParticleProps.zPosition = -0.2f;
		mParticleProps.emitAngleWidthRadiant = 6.282f;
		mParticleProps.minColor = { 0.7f, 0.7f, 0.7f, 1.0f };
		mParticleProps.maxColor = { 0.9f, 0.9f, 0.9f, 1.0f };
		mParticleProps.minSize = { 0.2f, 0.2f };
		mParticleProps.maxSize = { 0.2f, 0.2f };
		mParticleProps.sizeOverLifetime = { -0.2, -0.2 };

		mParticleSystem = ParticleSystem(mParticleProps, 10000);
	}

	void EditorLayer::OnDetach() {
		EAGLE_PROFILE_FUNCTION();
	}

	///////////////////// On Update //////////////////////
	void EditorLayer::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();

		if (mScenePanelFocused)
			mCameraController.OnUpdate();

		Renderer::GetStats().ResetStats();

		mFrameBuffer->Bind();

		RenderCommand::SetColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		{
			EAGLE_PROFILE_SCOPE("Rendering loop");

			static int v = 0;
			if (v > 10) {
				mParticleSystem.Emit({ 0, 0 });
				v = 0;
			}
			v++;
			mParticleSystem.OnRender(mCameraController.GetCamera());

			Renderer::BeginScene(mCameraController.GetCamera());
			Renderer::DrawTextureQuad({ 0, 0, -0.3 }, { 40, 40 }, bg, 100, { 1, 1, 1, 1 });

			float xPos = glm::sin(Time::GetSeconds() * 0.5f) * 4;
			Renderer::DrawTextureQuad({ xPos, 2 }, { 1, 1 }, creature, 1, glm::vec4(texColor[0], texColor[1], texColor[2], texColor[3]));

			for (int i = 0; i < 10; i++) {
				float yPos = glm::sin(Time::GetSeconds() - i);
				Renderer::DrawRotatedColorQuad({ i - 5, yPos, -0.1f }, rotation, { 0.4f, 0.4f }, glm::vec4(color[0], color[1], color[2], color[3]));;
			}

			Renderer::EndScene();
			mFrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnEvent(Egl::Event& event) {
		EAGLE_PROFILE_FUNCTION();
		mCameraController.OnEvent(event);
	}

	void EditorLayer::OnImGuiRender() {
		EAGLE_PROFILE_FUNCTION();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::Begin("Renderer Stats");
		ImGui::Text("Drawcalls: %d", Renderer::GetStats().GetDrawCallCount());
		ImGui::Text("Quads: %d", Renderer::GetStats().GetQuadCount());
		ImGui::Text("Vertices: %d", Renderer::GetStats().GetVertexCount());
		ImGui::Text("Indeces: %d", Renderer::GetStats().GetIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (mScenePanelSize != *((glm::vec2*)&scenePanelSize)) {
			mScenePanelSize = { scenePanelSize.x, scenePanelSize.y };
			mFrameBuffer->Resize((uint32_t)scenePanelSize.x, (uint32_t)scenePanelSize.y);
			mCameraController.Resize(scenePanelSize.x, scenePanelSize.y);
		}

		mScenePanelFocused = ImGui::IsWindowFocused();
		mScenePanelHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->LetEventsThrough(mScenePanelFocused && mScenePanelHovered);

		uint32_t textureID = mFrameBuffer->GetColorAttachementsRendererID();
		ImGui::Image((void*)(intptr_t)textureID, scenePanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

	}

}