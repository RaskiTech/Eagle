#include <EaglePCH.h>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneCamera.h"

namespace Egl {
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
