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
    class ParticleUpdater
    {
    public:
		ParticleUpdater() = default;
		virtual ~ParticleUpdater() = default;

        virtual void update(float deltaTime, ParticleData* p) = 0;
    };
}


namespace Egl {
    class ParticleUpdaterEuler : public ParticleUpdater
    {
    public:
        glm::vec2 mGlobalAcceleration{ 0.0f };
        virtual void update(float deltaTime, ParticleData* p) override;
    };
	class ParticleUpdaterConstantSpeed : public ParticleUpdater
	{
	public:
		virtual void update(float deltaTime, ParticleData* p) override;
	};
	// collision with the floor :) todo: implement a collision model
	class ParticleUpdaterFloor : public ParticleUpdater
	{
	public:
		float mFloorY{ 0.0f };
		float mBounceFactor{ 0.5f };
	public:
		virtual void update(float deltaTime, ParticleData* p) override;
	};

	class ParticleUpdaterAttractor : public ParticleUpdater
	{
	protected:
		std::vector<glm::vec3> mAttractors; // .z is force
	public:
		virtual void update(float dt, ParticleData* p) override;

		uint32_t collectionSize() const { return (uint32_t)mAttractors.size(); }
		void add(const glm::vec3& attr) { mAttractors.push_back(attr); }
		glm::vec3& get(uint32_t id) { return mAttractors[id]; }
	};

	class ParticleUpdaterBasicColor : public ParticleUpdater
	{
	public:
		virtual void update(float deltaTime, ParticleData* p) override;
	};

	class ParticleUpdaterPosColor : public ParticleUpdater
	{
	public:
		glm::vec2 mMinPos{ 0.0 };
		glm::vec2 mMaxPos{ 1.0 };
	public:
		virtual void update(float deltaTime, ParticleData* p) override;
	};

	class ParticleUpdaterVelColor : public ParticleUpdater
	{
	public:
		glm::vec2 mMinVel{ 0.0 };
		glm::vec2 mMaxVel{ 1.0 };
	public:
		virtual void update(float deltaTime, ParticleData* p) override;
	};

	class ParticleUpdaterBasicTime : public ParticleUpdater
	{
	public:
		virtual void update(float deltaTime, ParticleData* p) override;
	};
}