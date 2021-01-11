#pragma once
#include <glm/glm.hpp>
#include "Eagle/Rendering/Camera.h"

namespace Egl {
	struct ParticleSystemProps {
		float emitAngleWidthRadiant = 3.141f * 2, emitDirOffset = 0;
		float minVelocity = 1, maxVelocity = 1;

		float acceleration = 0;
		float minLifeTime = 5, maxLifetime = 5;
		glm::vec4 minColor = { 1, 1, 1, 1 }, maxColor = { 1, 1, 1, 1 };
		float minRotation = 0, maxRotation = 0;
		glm::vec2 minSize = { 1, 1 }, maxSize = { 1, 1 };

		glm::vec2 sizeAtEnd = { -1, -1 };
		float rotationOverLifetime = 0;
	};

	class ParticleSystem {
	public:
		ParticleSystem() {}
		ParticleSystem(ParticleSystemProps& props, uint32_t particleAmount = 10000);
		void SetProps(ParticleSystemProps& props) { mProps = props; }

		void OnRender(float zPosition);
		void Emit(const glm::vec3& position);
	private:
		struct ParticleData {
			float lifeTime, beenAlive;
			glm::vec2 position;
			float rotation;

			glm::vec2 velocity;
			glm::vec2 startSize;
			glm::vec4 color;

			bool active = false;
		};
		ParticleSystemProps mProps;
		std::vector<ParticleData> mData;

		uint32_t mNextParticleIndex = 999;
	};
}