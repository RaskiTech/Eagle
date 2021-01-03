#pragma once
#include "Eagle.h"

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

		// Temp, game fields
		CameraController mCameraController;
		float color[4] = { 0.8f, 0.2f, 0.3f, 1.0f };
		float texColor[4] = { 1, 1, 1, 1 };
		float rotation = 0;

		Ref<Texture> mCaveTiles, bg;
		Ref<SubTexture> creature;
		Ref<FrameBuffer> mFrameBuffer;
		Ref<VertexArray> mVertexArray;
		Ref<Shader> mShader;

		ParticleSystemProps mParticleProps;
		ParticleSystem mParticleSystem;

		// Editor fields
		glm::vec2 mScenePanelSize = { 0.0f, 0.0f };
		bool mScenePanelFocused = false, mScenePanelHovered = false;
	};
}