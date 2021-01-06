#pragma once
#include <glm/glm.hpp>

namespace Egl {
	// Mainly used in CameraComponent
	class Camera {
	public:
		Camera(const glm::mat4& projection) : mProjection(projection) {}

		const glm::mat4 GetProjection() const { return mProjection; }
	private:
		glm::mat4 mProjection;
	};

	// From when didn't have an ECS
	/*
	// An orthographic camera
	class Camera {
	public:
		Camera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

		inline void SetPosition(const glm::vec3& position) { mPosition = position; RecalculateViewMatrix(); }
		inline void SetRotation(float rotation) { mRotation = rotation; RecalculateViewMatrix(); }
		
		inline const glm::vec3& GetPosition() const { return mPosition; }
		inline float GetRotation() const { return mRotation; }

		inline const glm::mat4 GetViewProjectionMatrix() const { return mViewProjectionMatrix; }
		inline const glm::mat4 GetProjectionMatrix() const { return mProjectionMatrix; }
		inline const glm::mat4 GetViewMatrix() const { return mViewMatrix; }
	private:
		void RecalculateViewMatrix();

		glm::mat4 mViewProjectionMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

		glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
		float mRotation = 0;
	};
	*/
}