#pragma once
#include "ParticleData.h"

/*
FloorUpdater - can bounce particle off the floor.
AttractorUpdater - attractors in a gravity system.
BasicColorUpdater - generate current particle color based on time and min and max color.
PosColorUpdater - current color comes from position.
VelColorUpdater - current color comes from velocity.
BasicTimeUpdater - measures the time of life of a particle. It kills a particle if its time is over.
*/

namespace Egl {
	namespace Particles {
		struct ParticleUpdater {
			ParticleUpdater() = default;
			virtual ~ParticleUpdater() = default;

			virtual void Update(float deltaTime, ParticleData* data) = 0;
			virtual void OnImGuiRender() = 0;
		};
	}
}


namespace Egl {
	namespace Particles {

		struct EulerUpdater : public ParticleUpdater {
			EulerUpdater(const glm::vec2& globalAcceleration = { 0, 0 }) : globalAcceleration(globalAcceleration) {}

			virtual void Update(float deltaTime, ParticleData* data) override;
			virtual void OnImGuiRender() override;

			glm::vec2 globalAcceleration { 0.0f };
		};

		struct ConstSpeedUpdater : public ParticleUpdater {
			ConstSpeedUpdater() = default;

			virtual void Update(float deltaTime, ParticleData* data) override;
			virtual void OnImGuiRender() override;
		};

		struct PerformantFloorUpdater : public ParticleUpdater {
			PerformantFloorUpdater() = default;
			PerformantFloorUpdater(float floorY, float bounceFactor) : floorY(floorY), bounceFactor(bounceFactor) {}

			virtual void Update(float deltaTime, ParticleData* data) override;
			virtual void OnImGuiRender() override;

			float floorY{ 0 };
			float bounceFactor{ 0.5f };
		};

		struct AttractorUpdater : public ParticleUpdater {
			virtual void Update(float dt, ParticleData* data) override;
			virtual void OnImGuiRender() override;

			uint32_t AttractorCount() const { return (uint32_t)mAttractors.size(); }
			void AddAttractor(const glm::vec3& attr) { mAttractors.push_back(attr); }
			glm::vec3& Get(uint32_t id) { return mAttractors[id]; }
		protected:
			std::vector<glm::vec3> mAttractors; // .z is force
		};

		struct ColorUpdater : public ParticleUpdater {
			ColorUpdater() = default;


			virtual void Update(float deltaTime, ParticleData* data) override;
			virtual void OnImGuiRender() override;
		};

		struct PosToColorUpdater : public ParticleUpdater {
			PosToColorUpdater() = default;
			PosToColorUpdater(const glm::vec2& minPos, const glm::vec2& maxPos) : minPos(minPos), maxPos(maxPos) {}

			virtual void Update(float deltaTime, ParticleData* data) override;
			virtual void OnImGuiRender() override;

			glm::vec2 minPos { 0 };
			glm::vec2 maxPos { 1 };
		};

		struct VelocityToColorUpdater : public ParticleUpdater {
			VelocityToColorUpdater() = default;
			VelocityToColorUpdater(const glm::vec2& minVel, const glm::vec2& maxVel) : minVel(minVel), maxVel(maxVel) {}

			virtual void Update(float deltaTime, ParticleData* data) override;
			virtual void OnImGuiRender() override;

			glm::vec2 minVel { 0 };
			glm::vec2 maxVel { 1 };
		};

		struct TimeUpdater : public ParticleUpdater {
			TimeUpdater() = default;

			virtual void Update(float deltaTime, ParticleData* data) override;
			virtual void OnImGuiRender() override;
		};

		struct EndSizeUpdater : public ParticleUpdater {
			EndSizeUpdater() = default;
			EndSizeUpdater(const glm::vec2& endSize) : endSize(endSize) {}

			virtual void Update(float deltaTime, ParticleData * data) override;
			virtual void OnImGuiRender() override;

			glm::vec2 endSize { 0 };
		};
		struct SizeChangeUpdater : public ParticleUpdater {
			SizeChangeUpdater() = default;
			SizeChangeUpdater(const glm::vec2 & sizeChangeInSecond) : sizeChangeInSecond(sizeChangeInSecond) {}

			virtual void Update(float deltaTime, ParticleData * data) override;
			virtual void OnImGuiRender() override;

			glm::vec2 sizeChangeInSecond { 0 };
		};
	}
}