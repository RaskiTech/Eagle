#pragma once
#include "Eagle/Rendering/Camera.h"

namespace Egl {
	class SceneCamera : public Camera {
	public:
		SceneCamera() { RecaulculateProjection(); }
		virtual ~SceneCamera() = default;


		void SetBounds(float size, float nearClip = -1, float farClip = 1);
		inline void SetSize(float size) { SetBounds(size); }
		void SetAspectRatio(float aspectRatio);

		float GetCameraSize() { return mSize; }
	private:
		void RecaulculateProjection();

		float mAspectRatio;
		float mSize = 2;
		float mNearClip = -1, mFarClip = 1;
	};
}
