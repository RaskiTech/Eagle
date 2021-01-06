#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"
#include "ImGui/imgui.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/ECS/Script.h"

#include "Eagle/Core/Input.h"
#include "Eagle/Core/Keycodes.h"

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

#pragma region Testing ECS 
		mCamera = mScene->AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>();
		mScene->SetPrimaryCamera(mCamera);

		mSecondCamera = mScene->AddEntity("SecondCamera");
		mSecondCamera.AddComponent<CameraComponent>();
		mSecondCamera.GetComponent<CameraComponent>().camera.SetBounds(5, -1, 1);

		// Camera controller
		class CameraController : public Script {
		public:
			void OnCreate() {
				
			}
			void OnDestroy() {

			}
			void OnUpdate() {
				auto& transform = GetComponent<TransformComponent>().transform;
				float speed = 5;

				if (Input::IsKeyPressed(EGL_KEY_A)) transform[3][0] -= speed * Time::GetFrameDelta();
				if (Input::IsKeyPressed(EGL_KEY_D)) transform[3][0] += speed * Time::GetFrameDelta();
				if (Input::IsKeyPressed(EGL_KEY_S)) transform[3][1] -= speed * Time::GetFrameDelta();
				if (Input::IsKeyPressed(EGL_KEY_W)) transform[3][1] += speed * Time::GetFrameDelta();
			}
		};

		mCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#pragma endregion
	}

	void EditorLayer::OnDetach() {
		EAGLE_PROFILE_FUNCTION();
	}

	///////////////////// On Update //////////////////////
	void EditorLayer::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();

		// Handle resize
		FrameBufferDefenition def = mFrameBuffer->GetDefenition();
		if ( mScenePanelSize.x > 0.0f && mScenePanelSize.y > 0.0f 
			&& (mScenePanelSize.x != def.width || mScenePanelSize.y != def.height))
		//if (mScenePanelSize.x != def.width || mScenePanelSize.y != def.height)
		{
			mFrameBuffer->Resize((uint32_t)mScenePanelSize.x, (uint32_t)mScenePanelSize.y);
			mScene->SetViewportAspectRatio(mScenePanelSize.x / mScenePanelSize.y);
		}

		//if (mScenePanelFocused) {
		//	// What happens only when window is focused
		//	//mCameraController.OnUpdate();
		//}

		Renderer::GetStats().ResetStats();
		mFrameBuffer->Bind();

		RenderCommand::SetColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		mScene->OnUpdate();

		mFrameBuffer->Unbind();

		// Random note: 
		// Input needs to probably go through some kind of check to make sure the game window is focused. idk what to do about input polling
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
		ImGui::End();
		
		ImGui::Begin("Components window");
		ImGui::DragFloat3("CameraTransform", glm::value_ptr(mSecondCamera.GetComponent<TransformComponent>().transform[3]));
		if (ImGui::Checkbox("Camera 1", &use1camera))
			mScene->SetPrimaryCamera(mScene->GetPrimaryCamera() == mCamera ? mSecondCamera : mCamera);
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		// The actual resizing happens in onUpdate. This is just a "notification". Currently could as well be here.
		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (mScenePanelSize != *((glm::vec2*)&scenePanelSize))
			mScenePanelSize = { scenePanelSize.x, scenePanelSize.y };

		mScenePanelFocused = ImGui::IsWindowFocused();
		mScenePanelHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->LetEventsThrough(mScenePanelFocused && mScenePanelHovered);

		uint32_t textureID = mFrameBuffer->GetColorAttachementsRendererID();
		ImGui::Image((void*)(intptr_t)textureID, scenePanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

	}

}