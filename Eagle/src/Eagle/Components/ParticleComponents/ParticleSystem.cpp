#include <EaglePCH.h>
#include "ParticleSystem.h"
#include "ParticleData.h"
#include "ParticleSetters.h"
#include "Eagle/Rendering/Renderer.h"


namespace Egl {
	namespace Particles {

		///// Particle Emitter /////
		void ParticleEmitter::Emit(float deltaTime, ParticleData* data)
		{
			mEmitTime += deltaTime * mEmitsPerSecond;
			const uint32_t maxNewParticles = (uint32_t)mEmitTime;
			mEmitTime -= maxNewParticles;

			const uint32_t startId = data->mAliveCount;
			const uint32_t endId = std::min(startId + maxNewParticles, data->mCount - 1);

			for (auto& gen : mSetters)               // << gen loop
				gen->Apply(deltaTime, data, startId, endId);

			for (uint32_t i = startId; i < endId; ++i)    // << wake loop
				data->Wake(i);
		}

		ParticleSystem::ParticleSystem(uint32_t maxCount)
			: mCount(maxCount)
		{
			mParticles.Generate(maxCount);
		}

		void ParticleSystem::Update(float dt) {
			for (auto& em : mEmitters)
				em->Emit(dt, &mParticles);

			for (uint32_t i = 0; i < mCount; ++i)
				mParticles.acceleration[i] = glm::vec2(0.0f);

			for (auto& up : mUpdaters)
				up->Update(dt, &mParticles);
		}
		void ParticleSystem::Render() {
			for (uint32_t i = 0; i < mParticles.mAliveCount; i++)
				Renderer::DrawColorQuad(mParticles.position[i], mParticles.size[i], mParticles.color[i]);
		}
		void ParticleSystem::Reset() {
			mParticles.mAliveCount = 0;
		}
	}
}