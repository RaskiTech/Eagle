#include <EaglePCH.h>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include "ParticleGenerators.h"

namespace Egl {
    void ParticleBoxGenerator::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
        glm::vec2 posMin { mPos.x - mMaxStartPosOffset.x, mPos.y - mMaxStartPosOffset.y };
        glm::vec2 posMax { mPos.x + mMaxStartPosOffset.x, mPos.y + mMaxStartPosOffset.y };
        for (uint32_t i = startId; i < endId; ++i)
            p->mPos[i] = glm::linearRand(posMin, posMax);
    }

	void RoundPosGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i) {
			float ang = glm::linearRand(0.0f, glm::pi<float>() * 2.0f);
			p->mPos[i] = mCenter + glm::vec2(mRadX * sin(ang), mRadY * cos(ang));
		}
	}

	void BasicColorGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i) {
			p->mStartCol[i] = glm::linearRand(mMinStartCol, mMaxStartCol);
			p->mEndCol[i] = glm::linearRand(mMinEndCol, mMaxEndCol);
		}
	}

	void BasicVelGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i)
			p->mVel[i] = glm::linearRand(mMinStartVel, mMaxStartVel);
	}

	void BasicTimeGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i) {
			p->mTime[i] = { 0, glm::linearRand(mMinTime, mMaxTime) };
		}
	}
}