#include <EaglePCH.h>
#include <Dependencies/GLM.h>
#include "Eagle/Core/Application.h"
#include "SceneCamera.h"

namespace Egl {
	SceneCamera::SceneCamera() {
		mAspectRatio = Application::Get().GetSceneWindowSize().x / Application::Get().GetSceneWindowSize().y;
		RecaulculateProjection(); 
	}

	void SceneCamera::SetBounds(float size, float nearClip, float farClip) {
		mSize = size;
		mNearClip = nearClip;
		mFarClip = farClip;
		RecaulculateProjection();
	}

	void SceneCamera::SetAspectRatio(float aspectRatio) {
		mAspectRatio = aspectRatio;
		RecaulculateProjection();
	}
	void SceneCamera::RecaulculateProjection()
	{
		float right = 0.5f * mAspectRatio * mSize;
		float top = 0.5f * mSize;

		mProjection = glm::ortho(-right, right, -top, top, mNearClip, mFarClip);
	}
}
