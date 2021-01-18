#pragma once
#include <glm/glm.hpp>
#include "Eagle/Core/Core.h"

namespace Egl {
    struct ParticleData {
        // Idealy should use Scope<>, but entt doesn't like that
        Ref<glm::vec2[]> mPos;
        Ref<glm::vec4[]> mCol;
        Ref<glm::vec4[]> mStartCol;
        Ref<glm::vec4[]> mEndCol;
        Ref<glm::vec2[]> mVel;
        Ref<glm::vec2[]> mAcc;
        Ref<glm::vec2[]> mTime; // x is time amive from 0 to 1 and y is maxTime
        Ref<bool[]> mAlive;

        uint32_t mCount = 0;
        uint32_t mAliveCount = 0;

        ParticleData() {};
        explicit ParticleData(uint32_t maxCount) { generate(maxCount); }
        ~ParticleData() {}

        void generate(uint32_t maxSize);
        void kill(uint32_t id);
        void wake(uint32_t id);
        void swapData(uint32_t a, uint32_t b);
    };
}