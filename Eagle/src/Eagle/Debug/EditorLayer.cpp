#include <EaglePCH.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Dependencies/ImGui.h>
#include "EditorLayer.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Core/Events/MouseEvent.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Core/Time.h"

#include "Eagle/Core/Input.h"
#include "Eagle/Core/Keycodes.h"

#include "Eagle/Core/Application.h"
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

		mHierarchyPanel.SetContext(Application::Get().GetGameLayer()->GetActiveScene());
	}

	void EditorLayer::OnDetach() {
		EAGLE_PROFILE_FUNCTION();
	}

	void EditorLayer::PreUpdate() {
		EAGLE_PROFILE_FUNCTION();
		Renderer::GetStats().ResetStats();

		FrameBufferDefenition def = mFrameBuffer->GetDefenition();
		const glm::vec2& size = Application::Get().GetViewportSize();
		if (size.x > 0.0f && size.y > 0.0f && (size.x != def.width || size.y != def.height)) {
			mFrameBuffer->Resize((uint32_t)size.x, (uint32_t)size.y);
			Application::Get().GetGameLayer()->GetActiveScene()->SetViewportAspectRatio(size.x / size.y);
		}

		mFrameBuffer->Bind();
	}
	void EditorLayer::PostUpdate() {
		EAGLE_PROFILE_FUNCTION();
		mFrameBuffer->Unbind();
	}

	///////////////////// On Update //////////////////////
	void EditorLayer::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();
	}

	

	void EditorLayer::OnImGuiRender() {

		EAGLE_PROFILE_FUNCTION();
		auto& style = ImGui::GetStyle();
		float originalWindowMinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f; // 319.0f;

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		style.WindowMinSize.x = originalWindowMinSize;

		///////// Renderer stats //////////
		ImGui::Begin("Game Stats");

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[0];

		ImGui::PushFont(boldFont);
		ImGui::Text("Framerate");
		ImGui::PopFont();

		// Change the framerate stats only sometimes
		static float time = -0.1f;
		static uint32_t fps;
		static float ms;

		time -= Time::GetFrameDelta();
		if (time < 0) {
			fps = (uint32_t)(1 / Time::GetFrameDelta());
			ms = Time::GetFrameDeltaMilliseconds();
			time = 0.1f;
		}
		ImGui::Text("  FPS: %d", fps);
		ImGui::Text("  Ms per frame: %.1f", ms);

		ImGui::Spacing();
		ImGui::PushFont(boldFont);
		ImGui::Text("Renderer");
		ImGui::PopFont();

		ImGui::Text("  Drawcalls: %d", Renderer::GetStats().GetDrawCallCount());
		ImGui::Text("  Quads: %d", Renderer::GetStats().GetQuadCount());
		ImGui::Text("  Vertices: %d", Renderer::GetStats().GetVertexCount());
		ImGui::Text("  Indeces: %d", Renderer::GetStats().GetIndexCount());


		ImGui::End();

		///////// Scene //////////
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		// The actual resizing happens in onUpdate. This is just a "notification"
		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (Application::Get().GetViewportSize() != *((glm::vec2*)&scenePanelSize)) {
			Application::Get().SetScenePanelSize({ scenePanelSize.x, scenePanelSize.y });
			Application::Get().SetViewportSize({ scenePanelSize.x, scenePanelSize.y });
		}
		ImVec2 vMin = ImGui::GetWindowContentRegionMin(); // If the top bar isn't hidden
		ImVec2 winPos = ImGui::GetWindowPos();
		Application::Get().SetSceneScreenOffset({ vMin.x + winPos.x - Application::Get().GetWindow().GetPositionX(), vMin.y + winPos.y - Application::Get().GetWindow().GetPositionY() });

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
			Application::Get().GetGameLayer()->ResetApplication();
			mHierarchyPanel.SetContext(Application::Get().GetGameLayer()->GetActiveScene());
			mHierarchyPanel.ResetSelection();
			Application::Get().GetGameLayer()->GetActiveScene()->SetViewportAspectRatio(Application::Get().GetViewportSize().x / Application::Get().GetViewportSize().y);
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Egl::Event& event) {
		EAGLE_PROFILE_FUNCTION();
	}
}