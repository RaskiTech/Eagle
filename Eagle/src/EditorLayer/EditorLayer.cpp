#include <EaglePCH.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include "EditorLayer.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Events/MouseEvent.h"
#include "Eagle/Rendering/Renderer.h"

#include "Eagle/Core/Input.h"
#include "Eagle/Core/Keycodes.h"

namespace Egl {

	EditorLayer::EditorLayer() : Layer("EditorLayer") {

	}

	///////////////////// On Attach //////////////////////
	void EditorLayer::OnAttach() {
		EAGLE_PROFILE_FUNCTION();
		mHierarchyPanel.SetContext(Application::Get().GetGameLayer()->GetActiveScene());
		mHierarchyPanel.ResetSelection();
	}

	void EditorLayer::OnDetach() {
		EAGLE_PROFILE_FUNCTION();
	}

	void EditorLayer::PreUpdate()
	{
		Application::Get().GetGameLayer()->GetFrameBuffer()->Bind();
	}

	///////////////////// On Update //////////////////////
	void EditorLayer::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();

		// Handle resize
		//mScenePanelSize = { 1600, 900 };
		FrameBufferDefenition def = Application::Get().GetGameLayer()->GetFrameBuffer()->GetDefenition();
		if ( mScenePanelSize.x > 0.0f && mScenePanelSize.y > 0.0f 
			&& (mScenePanelSize.x != def.width || mScenePanelSize.y != def.height))
		{
			Application::Get().GetGameLayer()->GetFrameBuffer()->Resize((uint32_t)mScenePanelSize.x, (uint32_t)mScenePanelSize.y);
			Application::Get().GetGameLayer()->GetActiveScene()->SetViewportAspectRatio(mScenePanelSize.x / mScenePanelSize.y);
		}

		Renderer::GetStats().ResetStats();
	}

	void EditorLayer::PostUpdate()
	{
		Application::Get().GetGameLayer()->GetFrameBuffer()->Unbind();
	}

	void EditorLayer::OnImGuiRender() {
		EAGLE_PROFILE_FUNCTION();
		auto& style = ImGui::GetStyle();
		float originalWindowMinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f; // 319.0f;
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		style.WindowMinSize.x = originalWindowMinSize;

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

		// The actual resizing happens in onUpdate. This is just a "notification"
		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (mScenePanelSize != *((glm::vec2*) & scenePanelSize))
			mScenePanelSize = { scenePanelSize.x, scenePanelSize.y };

		mScenePanelFocused = ImGui::IsWindowFocused();
		mScenePanelHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->LetEventsThrough(mScenePanelFocused && mScenePanelHovered);

		uint32_t textureID = Application::Get().GetGameLayer()->GetFrameBuffer()->GetColorAttachementsRendererID();
		ImGui::Image((void*)(intptr_t)textureID, scenePanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		mHierarchyPanel.OnImGuiRender();

		//// Restart scene button ////
		ImGui::Begin("Restart Scene");
		if (ImGui::Button("Reset scene", ImVec2(ImGui::GetWindowWidth(), 0)))
			Application::Get().GetGameLayer()->ResetApplication();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Egl::Event& event) {
		EAGLE_PROFILE_FUNCTION();
	}
}