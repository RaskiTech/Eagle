#pragma once
#include "Eagle/Rendering/Camera.h"

namespace Egl {
	class SceneCamera : public Camera {
	public:
		SceneCamera() { RecaulculateProjection(); }
		virtual ~SceneCamera() = default;


		void SetBounds(float size, float nearClip = -1, float farClip = 1);
		void SetAspectRatio(float aspectRatio);
		inline float GetAspectRatio() const { return mAspectRatio; }

		inline void SetSize(float size) { SetBounds(size); }
		inline float GetSize() const { return mSize; }
	private:
		void RecaulculateProjection();

		float mAspectRatio;
		float mSize = 2;
		float mNearClip = -1, mFarClip = 1;
	};
}
