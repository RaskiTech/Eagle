#pragma once
#include "Eagle.h"
#include "Panels/HierarchyPanel.h"

namespace Egl {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:

		Ref<Texture> mCaveTiles, bg;
		Ref<SubTexture> creature;
		Ref<FrameBuffer> mFrameBuffer;
		Ref<VertexArray> mVertexArray;
		Ref<Shader> mShader;

		ParticleSystemProps mParticleProps;
		ParticleSystem mParticleSystem;

		// ECS fields
		Ref<Scene> mScene;
		Entity mPlayer, mCamera;

		// Actual Editor fields
		glm::vec2 mScenePanelSize = { 0.0f, 0.0f };
		bool mScenePanelFocused = false, mScenePanelHovered = false;
		HierarchyPanel mHierarchyPanel;
	};
}