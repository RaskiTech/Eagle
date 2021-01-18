#include <EaglePCH.h>
#include "ParticleUpdater.h"
#include "ParticleData.h"

namespace Egl {
    void ParticleUpdaterEuler::update(float deltaTime, ParticleData* p) {
        const glm::vec2 globalA { mGlobalAcceleration.x, mGlobalAcceleration.y };
        const uint32_t endId = p->mAliveCount;

		uint32_t i;
        for (i = 0; i < endId; ++i)
            p->mAcc[i] += globalA;

        for (i = 0; i < endId; ++i)
            p->mVel[i] += p->mAcc[i] * deltaTime;

        for (i = 0; i < endId; ++i)
            p->mPos[i] += p->mVel[i] * deltaTime;
    }
	void ParticleUpdaterFloor::update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		for (uint32_t i = 0; i < endId; ++i)
		{
			if (p->mPos[i].y < mFloorY)
			{
				glm::vec2 force = p->mAcc[i];
				float normalFactor = glm::dot(force, glm::vec2(0.0f, 1.0f));
				if (normalFactor < 0.0f)
					force -= glm::vec2(0.0f, 1.0f) * normalFactor;

				float velFactor = glm::dot(p->mVel[i], glm::vec2(0.0f, 1.0f));

				p->mVel[i] -= glm::vec2(0.0f, 1.0f) * (1.0f + mBounceFactor) * velFactor;

				p->mAcc[i] = force;
			}
		}

	}

	void ParticleUpdaterAttractor::update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		const uint32_t countAttractors = (uint32_t)mAttractors.size();
		glm::vec2 particleOffset;
		float dist;
		uint32_t a;
		for (uint32_t i = 0; i < endId; ++i) {
			for (a = 0; a < countAttractors; ++a) {
				particleOffset.x = mAttractors[a].x - p->mPos[i].x;
				particleOffset.y = mAttractors[a].y - p->mPos[i].y;
				dist = glm::dot(particleOffset, particleOffset);

				//if (fabs(dist) > 0.00001)
				dist = mAttractors[a].z / dist;

				p->mAcc[i] += particleOffset * dist;
			}
		}
	}

	void ParticleUpdaterBasicColor::update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		for (uint32_t i = 0; i < endId; ++i)
			p->mCol[i] = glm::mix(p->mStartCol[i], p->mEndCol[i], p->mTime[i].x);
	}

	void ParticleUpdaterPosColor::update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		float scaler, scaleg;
		float diffr = mMaxPos.x - mMinPos.x;
		float diffg = mMaxPos.y - mMinPos.y;
		for (uint32_t i = 0; i < endId; ++i)
		{
			scaler = (p->mPos[i].x - mMinPos.x) / diffr;
			scaleg = (p->mPos[i].y - mMinPos.y) / diffg;
			p->mCol[i].r = scaler;// glm::mix(p->mStartCol[i].r, p->mEndCol[i].r, scaler);
			p->mCol[i].g = scaleg;// glm::mix(p->mStartCol[i].g, p->mEndCol[i].g, scaleg);
			p->mCol[i].b = glm::mix(p->mStartCol[i].b, p->mEndCol[i].b, p->mTime[i].y);
			p->mCol[i].a = glm::mix(p->mStartCol[i].a, p->mEndCol[i].a, p->mTime[i].x);
		}
	}

	void ParticleUpdaterVelColor::update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		float scaler, scaleg;
		float diffr = mMaxVel.x - mMinVel.x;
		float diffg = mMaxVel.y - mMinVel.y;
		for (uint32_t i = 0; i < endId; ++i)
		{
			scaler = (p->mVel[i].x - mMinVel.x) / diffr;
			scaleg = (p->mVel[i].y - mMinVel.y) / diffg;
			p->mCol[i].r = scaler;// glm::mix(p->mStartCol[i].r, p->mEndCol[i].r, scaler);
			p->mCol[i].g = scaleg;// glm::mix(p->mStartCol[i].g, p->mEndCol[i].g, scaleg);
			p->mCol[i].b = glm::mix(p->mStartCol[i].b, p->mEndCol[i].b, glm::atan(diffg, diffr));
			p->mCol[i].a = glm::mix(p->mStartCol[i].a, p->mEndCol[i].a, p->mTime[i].x);
		}
	}

	void ParticleUpdaterBasicTime::update(float deltaTime, ParticleData* p) {
		uint32_t endId = p->mAliveCount;
		if (endId == 0)
			return;

		for (uint32_t i = 0; i < endId; ++i) {
			p->mTime[i].x += deltaTime / p->mTime[i].y;
			if (p->mTime[i].x > 1) {
				p->kill(i);
				endId = p->mAliveCount < p->mCount ? p->mAliveCount : p->mCount;
			}
		}
	}

	void ParticleUpdaterConstantSpeed::update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		for (uint32_t i = 0; i < endId; ++i)
			p->mPos[i] += p->mVel[i] * deltaTime;
	}
}