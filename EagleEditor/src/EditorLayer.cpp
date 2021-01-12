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

// This wouldn't be inside Eagle, but probably somewhere defined.
#include "Eagle/Scripting/ExampleScript.h"

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

		// Scripting
		mActiveScene = ApplicationStartup();

		mHierarchyPanel.SetContext(mActiveScene);
		mHierarchyPanel.ResetSelection();

		mActiveScene->SceneBegin();

	}

	void EditorLayer::OnDetach() {
		EAGLE_PROFILE_FUNCTION();
		mActiveScene->SceneEnd();
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
			mActiveScene->SetViewportAspectRatio(mScenePanelSize.x / mScenePanelSize.y);
		}

		mFrameBuffer->Bind();

		Renderer::GetStats().ResetStats();

		mActiveScene->OnUpdate();

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
		if (ImGui::Button("Reset scene", ImVec2(ImGui::GetWindowWidth(), 0)))
			ResetApplication();

		ImGui::End();
	}

	// Mainly the reset button
	void EditorLayer::ResetApplication() {
		OnDetach();
		OnAttach();
	}

	void EditorLayer::OnEvent(Egl::Event& event) {
		EAGLE_PROFILE_FUNCTION();
	}
}