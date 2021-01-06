#include <glm/gtc/matrix_transform.hpp>
#include "EditorLayer.h"
#include "ImGui/imgui.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "glm/gtc/type_ptr.hpp"

namespace Egl {

	EditorLayer::EditorLayer() : Layer("EditorLayer") {//, mCameraController(1280.0f / 720.0f, true) {
		//mCameraController.SetZoom(2);
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

		// TODO: Refactor particleSystem

		// Particles
		//mParticleProps = ParticleSystemProps();
		//mParticleProps.zPosition = -0.2f;
		//mParticleProps.emitAngleWidthRadiant = 6.282f;
		//mParticleProps.minColor = { 0.7f, 0.7f, 0.7f, 1.0f };
		//mParticleProps.maxColor = { 0.9f, 0.9f, 0.9f, 1.0f };
		//mParticleProps.minSize = { 0.2f, 0.2f };
		//mParticleProps.maxSize = { 0.2f, 0.2f };
		//mParticleProps.sizeOverLifetime = { -0.2, -0.2 };
		//
		//mParticleSystem = ParticleSystem(mParticleProps, 10000);


		mScene = CreateRef<Scene>();
		mPlayer = mScene->AddEntity("Player");
		mPlayer.AddComponent<SpriteComponent>(glm::vec4(0.2f, 0.3f, 0.4f, 1.0f));
		mCamera = mScene->AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f));
		mScene->SetPrimaryCamera(mCamera);

		mSecondCamera = mScene->AddEntity("SecondCamera");
		mSecondCamera.AddComponent<CameraComponent>(glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f));

	}

	void EditorLayer::OnDetach() {
		EAGLE_PROFILE_FUNCTION();
	}

	///////////////////// On Update //////////////////////
	void EditorLayer::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();

		if (mScenePanelFocused) {
			// What happens only when window is focused
			//mCameraController.OnUpdate();
		}

		Renderer::GetStats().ResetStats();

		mFrameBuffer->Bind();

		// Random note: 
		// Input needs to probably go through some kind of check to make sure the game window is focused. idk what to do about input polling

		// --- Rendering --- // 

		RenderCommand::SetColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		mScene->OnUpdate();

		mFrameBuffer->Unbind();
	}

	void EditorLayer::OnEvent(Egl::Event& event) {
		EAGLE_PROFILE_FUNCTION();
		//mCameraController.OnEvent(event);
	}

	void EditorLayer::OnImGuiRender() {
		EAGLE_PROFILE_FUNCTION();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::Begin("Renderer Stats");
		ImGui::Text("Drawcalls: %d", Renderer::GetStats().GetDrawCallCount());
		ImGui::Text("Quads: %d", Renderer::GetStats().GetQuadCount());
		ImGui::Text("Vertices: %d", Renderer::GetStats().GetVertexCount());
		ImGui::Text("Indeces: %d", Renderer::GetStats().GetIndexCount());

		ImGui::DragFloat3("CameraTransform", glm::value_ptr(mSecondCamera.GetComponent<TransformComponent>().transform[3]));
		if (ImGui::Checkbox("Camera 1", &use1camera)) {
			//LOG("Shoulf be false: {0}", mCamera == mSecondCamera);
			//Entity e = Entity( (entt::entity)mCamera.GetID(), &*mScene );
			//LOG("Should be true {0}", mCamera == e);
			LOG("{0}, {1}", mScene->GetPrimaryCamera().ToString(), mCamera.ToString());
			auto& cameraToUse = mScene->GetPrimaryCamera() == mCamera ? mSecondCamera : mCamera;
			LOG("PromaryCamera now: {0}, Is it {1}: {2}, result was:  {3}", mScene->GetPrimaryCamera().ToString(), mCamera.ToString(), mScene->GetPrimaryCamera() == mCamera, cameraToUse.ToString());
			mScene->SetPrimaryCamera(cameraToUse);
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (mScenePanelSize != *((glm::vec2*)&scenePanelSize)) {
			mScenePanelSize = { scenePanelSize.x, scenePanelSize.y };
			mFrameBuffer->Resize((uint32_t)scenePanelSize.x, (uint32_t)scenePanelSize.y);
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