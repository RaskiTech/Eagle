#include <EaglePCH.h>
#include <Dependencies/GLM.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/Entt.h>
#include "EditorLayer.h"
#include "Eagle/Core/Application.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Core/Events/MouseEvent.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Core/Core.h"

#include "Eagle/Core/Input.h"
#include "Eagle/Core/Keycodes.h"

namespace Egl {

	EditorLayer::EditorLayer() {}

	///////////////////// On Attach //////////////////////
	void EditorLayer::OnAttach() {
		EAGLE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnDetach() {
		EAGLE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate() {
		Renderer::GetStats().ResetStats();
	}

	void EditorLayer::DrawSelectedEntityOutline(const CameraComponent& camera) {
		const glm::vec4 outlineColor = { 1.0f, 1.0f, 0.0f, 1.0f }; // Violet1
		const float width = 5 // <-- Width multiplier
			/ Application::Get().GetSceneWindowSize().x * camera.camera.GetSize() * camera.camera.GetAspectRatio();

		entt::entity e = mHierarchyPanel.GetSelectedEntity();
		glm::vec2 pos;
		glm::vec2 radius;

		if (e == entt::null)
			return;

		Scene* scene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
		if (scene->mRegistry.has<Transform>(e)) {
			Transform& t = scene->mRegistry.get<Transform>(e);
			pos = t.GetPosition();
			radius = { t.GetScale().x / 2, t.GetScale().y / 2 };
		}
		else {
			UITransform& t = scene->mRegistry.get<UITransform>(e);
			pos = t.GetWorldPosition();
			radius = { t.GetWorldScale().x / 2, t.GetWorldScale().y / 2 };
		}

		

		Renderer::DrawColorQuad(std::numeric_limits<uint16_t>::max(), glm::vec2{ pos.x + radius.x + width / 2, pos.y }, { width, radius.y * 2 + width * 2 }, outlineColor);
		Renderer::DrawColorQuad(std::numeric_limits<uint16_t>::max(), glm::vec2{ pos.x - radius.x - width / 2, pos.y }, { width, radius.y * 2 + width * 2 }, outlineColor);
		Renderer::DrawColorQuad(std::numeric_limits<uint16_t>::max(), glm::vec2{ pos.x, pos.y + radius.y + width / 2 }, { radius.x * 2 + width * 2, width }, outlineColor);
		Renderer::DrawColorQuad(std::numeric_limits<uint16_t>::max(), glm::vec2{ pos.x, pos.y - radius.y - width / 2 }, { radius.x * 2 + width * 2, width }, outlineColor);
	}

	void EditorLayer::OnSceneDelete() {
		mHierarchyPanel.ResetSelection();
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

		if (EAGLE_PROFILE)
			ImGui::TextWrapped("     Note: Profiling is enabled which noticeably hinders performance.");

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

		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (Application::Get().GetSceneWindowSize() != *((glm::vec2*)&scenePanelSize))
			Application::Get().SetSceneWindowSize({ scenePanelSize.x, scenePanelSize.y });

		ImVec2 vMin = ImGui::GetWindowContentRegionMin(); // If the top bar isn't hidden
		ImVec2 winPos = ImGui::GetWindowPos();
		Application::Get().SetSceneScreenOffset({ vMin.x + winPos.x - Application::Get().GetWindow().GetPositionX(), 
			vMin.y + winPos.y - Application::Get().GetWindow().GetPositionY() });

		mScenePanelFocused = ImGui::IsWindowFocused();
		mScenePanelHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->LetEventsThrough(mScenePanelFocused && mScenePanelHovered);
		uint32_t textureID = Application::Get().GetGameLayer()->ReadFramebuffer()->GetColorAttachementsRendererID();
		ImGui::Image((void*)(intptr_t)textureID, scenePanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		mAssetPanel.OnImGuiRender();
		mHierarchyPanel.OnImGuiRender();

		//// Restart scene button ////
		ImGui::Begin("Restart Application");
		if (ImGui::Button("Reload application", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 40))) {
			Application::Get().GetGameLayer()->ResetApplication();
			mHierarchyPanel.ResetSelection();
			Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene())->ChangeCameraAspectRatios(Application::Get().GetSceneWindowSize().x / Application::Get().GetSceneWindowSize().y);
		}

		ImGui::End();
	}
}