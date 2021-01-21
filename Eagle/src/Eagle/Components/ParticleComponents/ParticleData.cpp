#include <EaglePCH.h>
#include "ParticleData.h"

namespace Egl {
    namespace Particles {

        void ParticleData::Generate(uint32_t maxSize) {
            mCount = maxSize;
            mAliveCount = 0;

            position.reset(new glm::vec2[maxSize]);
            color.reset(new glm::vec4[maxSize]);
            startCol.reset(new glm::vec4[maxSize]);
            endCol.reset(new glm::vec4[maxSize]);
            startSize.reset(new glm::vec2[maxSize]);
            size.reset(new glm::vec2[maxSize]);
            velocity.reset(new glm::vec2[maxSize]);
            acceleration.reset(new glm::vec2[maxSize]);
            time.reset(new glm::vec2[maxSize]);
            alive.reset(new bool[maxSize]);
        }

        void ParticleData::Kill(uint32_t id) {
            alive[id] = false;
            SwapData(id, mAliveCount - 1);
            mAliveCount--;
        }

        void ParticleData::Wake(uint32_t id) {
            alive[id] = true;

            SwapData(id, mAliveCount);
            mAliveCount++;
        }

        void ParticleData::SwapData(uint32_t a, uint32_t b) {
            std::swap(position[a], position[b]);
            std::swap(color[a], color[b]);
            std::swap(startCol[a], startCol[b]);
            std::swap(endCol[a], endCol[b]);
            std::swap(startSize[a], startSize[b]);
            std::swap(size[a], size[b]);
            std::swap(velocity[a], velocity[b]);
            std::swap(acceleration[a], acceleration[b]);
            std::swap(time[a], time[b]);
            std::swap(alive[a], alive[b]);
        }
    }
}