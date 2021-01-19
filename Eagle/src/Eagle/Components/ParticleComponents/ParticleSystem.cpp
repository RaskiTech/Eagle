#include <EaglePCH.h>
#include "ParticleSystem.h"
#include "ParticleData.h"
#include "ParticleGenerators.h"
#include "Eagle/Rendering/Renderer.h"

/*
The second idea (not tested) would be to allow full dynamic configuration. Instead 
of having named set of available parameters we could store a map of <name, array>. 
Both name and type of param (vector, scalar, int) would be configurable. This would 
mean a lot of work, but for some kind of an particle editor this could be a real benefit.
*/

namespace Egl {
    ///// Particle Emitter /////
    void ParticleEmitter::Emit(float deltaTime, ParticleData* data)
    {
		mEmitTime += deltaTime * mEmitsPerSecond;
        const uint32_t maxNewParticles = (uint32_t)mEmitTime;
		mEmitTime -= maxNewParticles;

        const uint32_t startId = data->mAliveCount;
        const uint32_t endId = std::min(startId + maxNewParticles, data->mCount - 1);

        for (auto& gen : mGenerators)               // << gen loop
            gen->generate(deltaTime, data, startId, endId);

        for (uint32_t i = startId; i < endId; ++i)    // << wake loop
            data->wake(i);
    }

	ParticleSystem::ParticleSystem(uint32_t maxCount)
		: mCount(maxCount)
	{
		mParticles.generate(maxCount);
	}

	void ParticleSystem::Update(float dt) {
		for (auto& em : mEmitters)
			em->Emit(dt, &mParticles);

		for (uint32_t i = 0; i < mCount; ++i)
			mParticles.mAcc[i] = glm::vec2(0.0f);

		for (auto& up : mUpdaters)
			up->Update(dt, &mParticles);
	}
	void ParticleSystem::Render() {
		for (uint32_t i = 0; i < mParticles.mAliveCount; i++)
			Renderer::DrawColorQuad(mParticles.mPos[i], { 0.05f, 0.05f }, mParticles.mCol[i]);
	}
	void ParticleSystem::Reset() {
		mParticles.mAliveCount = 0;
	}
}