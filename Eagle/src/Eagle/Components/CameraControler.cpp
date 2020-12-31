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
		EAGLE_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(EGL_KEY_A)) mCameraPosition -= glm::vec3(mMoveSpeed, 0, 0) * Time::GetFrameDelta();
		if (Input::IsKeyPressed(EGL_KEY_D)) mCameraPosition += glm::vec3(mMoveSpeed, 0, 0) * Time::GetFrameDelta();
		if (Input::IsKeyPressed(EGL_KEY_W)) mCameraPosition += glm::vec3(0, mMoveSpeed, 0) * Time::GetFrameDelta();
		if (Input::IsKeyPressed(EGL_KEY_S)) mCameraPosition -= glm::vec3(0, mMoveSpeed, 0) * Time::GetFrameDelta();

		if (mShouldRotate) {
			if (Input::IsKeyPressed(EGL_KEY_Q)) mCameraRotation += mRotationSpeed * Time::GetFrameDelta();
			if (Input::IsKeyPressed(EGL_KEY_E)) mCameraRotation -= mRotationSpeed * Time::GetFrameDelta();

			mCamera.SetRotation(mCameraRotation);
		}
		mCamera.SetPosition(mCameraPosition);
		mMoveSpeed = mZoomLevel;
	}
	void CameraController::SetZoom(float zoom) {
		EAGLE_PROFILE_FUNCTION();
		mZoomLevel = zoom;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		EAGLE_PROFILE_FUNCTION();
		SetZoom(mZoomLevel - (event.GetScrollYOffset() * 0.25f));
		return false;
	}
	bool CameraController::OnApplicationRezised(WindowResizeEvent& event) {
		EAGLE_PROFILE_FUNCTION();
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