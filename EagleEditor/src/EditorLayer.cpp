#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"
#include "ImGui/imgui.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Events/MouseEvent.h"

#include "Eagle/Core/Input.h"
#include "Eagle/Core/Keycodes.h"

namespace Egl {

	EditorLayer::EditorLayer() : Layer("EditorLayer") {

	}

	///////////////////// On Attach //////////////////////
	void EditorLayer::OnAttach() {
		EAGLE_PROFILE_FUNCTION();
		FrameBufferDefenition defenition;
		defenition.width = 1280;
		defenition.height = 720;
		mFrameBuffer = FrameBuffer::Create(defenition);

#pragma region ECS 
		mScene = CreateRef<Scene>();

		for (int i = 0; i < 8; i++) {
			mPlayer = mScene->AddEntity("Cube");
			mPlayer.AddComponent<SpriteComponent>(glm::vec4(0.2f, 0.3f, 0.4f, 1.0f));
			mPlayer.GetComponent<TransformComponent>().position = { i * 1.5f - 5, 2, 0 };
			mPlayer.GetComponent<TransformComponent>().rotation = (float)i;
		}

		mCamera = mScene->AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>().camera.SetBounds(7);
		mScene->SetPrimaryCamera(mCamera);

		// Camera controller
		class CameraController : public Script {
		public:
			void OnUpdate() {
				auto& transform = GetComponent<TransformComponent>();
				float zoomSize = GetComponent<CameraComponent>().camera.GetCameraSize();
				float speed = 5;

				if (Input::IsKeyPressed(EGL_KEY_A)) transform.position.x -= speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_D)) transform.position.x += speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_S)) transform.position.y -= speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_W)) transform.position.y += speed * Time::GetFrameDelta() * zoomSize * 0.2f;
			}
		};
		mCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		Entity particle = mScene->AddEntity("ParticleSystem");
		ParticleSystemProps props;
		props.minSize = { 0.2f, 0.2f };
		props.minSize = { 0.4f, 0.4f };
		props.maxColor = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);
		props.minColor = glm::vec4(0.2f, 0.8f, 0.3f, 1.0f);
		props.sizeAtEnd = { 0, 0 };
		particle.AddComponent<ParticleSystemComponent>(props);
#pragma endregion

		mHierarchyPanel.SetContext(mScene);
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
		{
			mFrameBuffer->Resize((uint32_t)mScenePanelSize.x, (uint32_t)mScenePanelSize.y);
			mScene->SetViewportAspectRatio(mScenePanelSize.x / mScenePanelSize.y);
		}

		mFrameBuffer->Bind();

		Renderer::GetStats().ResetStats();

		mScene->OnUpdate();

		mFrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender() {
		EAGLE_PROFILE_FUNCTION();
		auto& style = ImGui::GetStyle();
		float windowMinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f; // 319.0f;
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		style.WindowMinSize.x = windowMinSize;

		///////// Renderer stats //////////
		ImGui::Begin("Renderer Stats");
		ImGui::Text("Drawcalls: %d", Renderer::GetStats().GetDrawCallCount());
		ImGui::Text("Quads: %d", Renderer::GetStats().GetQuadCount());
		ImGui::Text("Vertices: %d", Renderer::GetStats().GetVertexCount());
		ImGui::Text("Indeces: %d", Renderer::GetStats().GetIndexCount());
		ImGui::End();

		///////// Scene //////////
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		// The actual resizing happens in onUpdate. This is just a "notification". Currently could as well be here.
		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (mScenePanelSize != *((glm::vec2*) & scenePanelSize))
			mScenePanelSize = { scenePanelSize.x, scenePanelSize.y };

		mScenePanelFocused = ImGui::IsWindowFocused();
		mScenePanelHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->LetEventsThrough(mScenePanelFocused && mScenePanelHovered);

		uint32_t textureID = mFrameBuffer->GetColorAttachementsRendererID();
		ImGui::Image((void*)(intptr_t)textureID, scenePanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		mHierarchyPanel.OnImGuiRender();

		//// Restart scene button ////
		ImGui::Begin("Restart Scene");
		if (ImGui::Button("Reset scene", ImVec2(ImGui::GetWindowWidth(), 0))) {
			// TODO: Handle scene Reset
		}
		ImGui::End();
	}

	void EditorLayer::OnEvent(Egl::Event& event) {
		EAGLE_PROFILE_FUNCTION();
	}
}