#include <EaglePCH.h>
#include "ParticleData.h"

namespace Egl {
    void ParticleData::generate(uint32_t maxSize) {
        mCount = maxSize;
        mAliveCount = 0;

        mPos.reset(new glm::vec2[maxSize]);
        mCol.reset(new glm::vec4[maxSize]);
        mStartCol.reset(new glm::vec4[maxSize]);
        mEndCol.reset(new glm::vec4[maxSize]);
        mVel.reset(new glm::vec2[maxSize]);
        mAcc.reset(new glm::vec2[maxSize]);
        mTime.reset(new glm::vec2[maxSize]);
        mAlive.reset(new bool[maxSize]);
    }

    void ParticleData::kill(uint32_t id) {
        mAlive[id] = false;
        swapData(id, mAliveCount - 1);
        mAliveCount--;
    }

    void ParticleData::wake(uint32_t id) {
        mTime[id] = { 0, 1 };
        mAlive[id] = true;

        swapData(id, mAliveCount);
        mAliveCount++;
    }

    void ParticleData::swapData(uint32_t a, uint32_t b) {
        std::swap(mPos[a], mPos[b]);
        std::swap(mCol[a], mCol[b]);
        std::swap(mStartCol[a], mStartCol[b]);
        std::swap(mEndCol[a], mEndCol[b]);
        std::swap(mVel[a], mVel[b]);
        std::swap(mAcc[a], mAcc[b]);
        std::swap(mTime[a], mTime[b]);
        std::swap(mAlive[a], mAlive[b]);
    }
}