#pragma once
#include <glm/glm.hpp>
#include "ParticleData.h"

namespace Egl {
	struct ParticleGenerator {
        virtual void generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) = 0;
	};

    struct ParticleBoxGenerator : public ParticleGenerator
    {
        glm::vec2 mPos{ 0.0 };
        glm::vec2 mMaxStartPosOffset{ 0.0 };

        ParticleBoxGenerator() { }

        virtual void generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) override;
    };
	class RoundPosGen : public ParticleGenerator
	{
	public:
		glm::vec2 mCenter { 0.0 };
		float mRadX { 0.0 };
		float mRadY { 0.0 };
	public:
		RoundPosGen() { }
		RoundPosGen(const glm::vec2& center, float radX, float radY) : mCenter(center), mRadX(radX), mRadY(radY) {}

		virtual void generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) override;
	};

	class BasicColorGen : public ParticleGenerator
	{
	public:
		glm::vec4 mMinStartCol { 1 };
		glm::vec4 mMaxStartCol { 1 };
		glm::vec4 mMinEndCol { 1 };
		glm::vec4 mMaxEndCol { 1 };
	public:
		BasicColorGen() { }

		virtual void generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) override;
	};

	class BasicVelGen : public ParticleGenerator
	{
	public:
		glm::vec2 mMinStartVel { 0 };
		glm::vec2 mMaxStartVel { 0 };
	public:
		BasicVelGen() { }

		virtual void generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) override;
	};

	class BasicTimeGen : public ParticleGenerator
	{
	public:
		float mMinTime = 1;
		float mMaxTime = 1;
	public:
		BasicTimeGen() { }

		virtual void generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) override;
	};
}