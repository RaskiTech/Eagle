#pragma once
#include "Eagle/Rendering/Camera.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Events/ApplicationEvent.h"
#include "Eagle/Events/MouseEvent.h"

namespace Egl {
	class CameraController {
	public:
		CameraController(float aspectRatio, bool rotation = false);
		CameraController(float aspectRatio, float moveSpeed, bool rotation = false, float rotationSpeed = 70);
		void OnUpdate();
		void OnEvent(Event& event);
		Camera& GetCamera() { return mCamera; }
		const Camera& GetCamera() const { return mCamera; }

		float GetMoveSpeed() const { return mMoveSpeed; }
		float GetRotationSpeed() const { return mRotationSpeed; }
		void SetRotationSpeed(float speed) { mRotationSpeed = speed; }
		void SetZoom(float zoom);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnApplicationRezised(WindowResizeEvent& event);

		float mAspectRatio;
		float mZoomLevel = 1;
		Camera mCamera;
		bool mShouldRotate;

		glm::vec3 mCameraPosition = { 0.0f, 0.0f, 0.0f };
		float mCameraRotation = 0.0f;
		float mMoveSpeed = 2;
		float mRotationSpeed = 70;
	};
}