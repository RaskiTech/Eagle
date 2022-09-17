#pragma once
#include <Dependencies/GLM.h>
#include "Eagle/Core/Core.h"

namespace Egl {
    namespace Particles {

        struct ParticleData {
            // Idealy should use Scope<>, but entt doesn't like that
            Ref<glm::vec2[]> position;
            Ref<bool[]> alive;

            Ref<glm::vec4[]> color;
            Ref<glm::vec4[]> startCol;
            Ref<glm::vec4[]> endCol;

            Ref<glm::vec2[]> startSize;
            Ref<glm::vec2[]> size;

            Ref<glm::vec2[]> velocity;
            Ref<glm::vec2[]> acceleration;
            Ref<glm::vec2[]> time; // x is time from 1 to 0 and y is startTime

            uint32_t mCount = 0;
            uint32_t mAliveCount = 0;

            ParticleData() {};
            explicit ParticleData(uint32_t maxCount) { Generate(maxCount); }
            ~ParticleData() {}

            void Generate(uint32_t maxSize);
            void Kill(uint32_t id);
            void Wake(uint32_t id);
            void SwapData(uint32_t a, uint32_t b);
        };
    }
}