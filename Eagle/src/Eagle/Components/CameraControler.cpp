#include <EaglePCH.h>
#include "CameraController.h"
#include "Eagle/Core/Input.h"
#include "Eagle/Core/Keycodes.h"

namespace Egl {
	CameraController::CameraController(float aspectRatio, bool rotate) 
		: mAspectRatio(aspectRatio), mCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel),
		mShouldRotate(rotate) {}
	CameraController::CameraController(float aspectRatio, float moveSpeed, bool rotate, float rotationSpeed)
		: mAspectRatio(aspectRatio), mCamera(-mAspectRatio * mZoomLevel, mAspectRatio* mZoomLevel, -mZoomLevel, mZoomLevel), 
		mShouldRotate(rotate), mMoveSpeed(moveSpeed), mRotationSpeed(rotationSpeed) {}

	void CameraController::OnUpdate() {
		if (Input::IsKeyPressed(EGL_KEY_A)) mCameraPosition -= glm::vec3(mMoveSpeed, 0, 0) * Time::GetDeltaTime();
		if (Input::IsKeyPressed(EGL_KEY_D)) mCameraPosition += glm::vec3(mMoveSpeed, 0, 0) * Time::GetDeltaTime();
		if (Input::IsKeyPressed(EGL_KEY_W)) mCameraPosition += glm::vec3(0, mMoveSpeed, 0) * Time::GetDeltaTime();
		if (Input::IsKeyPressed(EGL_KEY_S)) mCameraPosition -= glm::vec3(0, mMoveSpeed, 0) * Time::GetDeltaTime();

		if (mShouldRotate) {
			if (Input::IsKeyPressed(EGL_KEY_Q)) mCameraRotation += mRotationSpeed * Time::GetDeltaTime();
			if (Input::IsKeyPressed(EGL_KEY_E)) mCameraRotation -= mRotationSpeed * Time::GetDeltaTime();

			mCamera.SetRotation(mCameraRotation);
		}
		mCamera.SetPosition(mCameraPosition);
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		LOG_ENG("{0}", event);
		mZoomLevel -= event.GetScrollYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
		return false;
	}
	bool CameraController::OnApplicationRezised(WindowResizeEvent& event) {
		mAspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
		return false;
	}
	void CameraController::OnEvent(Event& event) {
		EventDispatcher dispacer(event);
		dispacer.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CameraController::OnApplicationRezised));
		dispacer.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::OnMouseScrolled));
	}
}