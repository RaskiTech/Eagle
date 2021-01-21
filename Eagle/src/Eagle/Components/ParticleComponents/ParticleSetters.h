#pragma once
#include <glm/glm.hpp>
#include "ParticleData.h"

namespace Egl {
	namespace Particles {
		struct ParticleSetter {
			virtual void Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) = 0;
			virtual void OnImGuiRender() = 0;
		};
	}
}

namespace Egl {
	namespace Particles {

		struct BoxPosSetter : public ParticleSetter {
			BoxPosSetter() {}
			BoxPosSetter(const glm::vec2& position, const glm::vec2& maxOffset) : position(position), maxOffset(maxOffset) {}

			virtual void Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) override;
			virtual void OnImGuiRender() override;

			glm::vec2 position { 0 };
			glm::vec2 maxOffset { 0 };
		};
		struct CirclePosSetter : public ParticleSetter {
			CirclePosSetter() { }
			CirclePosSetter(const glm::vec2& position, const glm::vec2& radius) : position(position), radius(radius) {}

			virtual void Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) override;
			virtual void OnImGuiRender() override;

			glm::vec2 position { 0 };
			glm::vec2 radius { 0 };
		};

		struct ColorSetter : public ParticleSetter {
			ColorSetter() {}
			ColorSetter(const glm::vec4& minStartColor, const glm::vec4& maxStartColor, const glm::vec4& minEndColor, const glm::vec4& maxEndColor)
				: minStartColor(minStartColor), maxStartColor(maxStartColor), minEndColor(minEndColor), maxEndColor(maxEndColor) {}

			virtual void Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) override;
			virtual void OnImGuiRender() override;

			glm::vec4 minStartColor { 1 };
			glm::vec4 maxStartColor { 1 };
			glm::vec4 minEndColor { 1 };
			glm::vec4 maxEndColor { 1 };
		};

		struct VelocitySetter : public ParticleSetter {
			VelocitySetter() {}
			VelocitySetter(const glm::vec2& minVelocity, const glm::vec2& maxVelocity) : minVelocity(minVelocity), maxVelocity(maxVelocity) {}

			virtual void Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) override;
			virtual void OnImGuiRender() override;

			glm::vec2 minVelocity { 0 };
			glm::vec2 maxVelocity { 0 };
		};

		struct TimeSetter : public ParticleSetter {
			TimeSetter() {}
			TimeSetter(float minTime, float maxTime) : minTime(minTime), maxTime(maxTime) {}

			virtual void Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) override;
			virtual void OnImGuiRender() override;

			float minTime = 1;
			float maxTime = 1;
		};

		struct SizeSetter : public ParticleSetter {
			SizeSetter() {}
			SizeSetter(const glm::vec2& minSize, const glm::vec2& maxSize) : minSize(minSize), maxSize(maxSize) {}

			virtual void Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) override;
			virtual void OnImGuiRender() override;

			glm::vec2 minSize{ 0 };
			glm::vec2 maxSize{ 0 };
		};
	}
}