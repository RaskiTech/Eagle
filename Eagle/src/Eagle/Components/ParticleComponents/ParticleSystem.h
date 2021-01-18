#pragma once
#include "Eagle/Core/Core.h"
#include "ParticleData.h"
#include "ParticleGenerators.h"
#include "ParticleUpdater.h"

namespace Egl {
	///// Particle Emitter /////
    struct ParticleEmitter {
	public:
		ParticleEmitter() { }
		virtual ~ParticleEmitter() { }

		// calls all the generators and at the end it activates (wakes) particle
		virtual void Emit(float dt, ParticleData* p);

		void addGenerator(Ref<ParticleGenerator> gen) { mGenerators.push_back(gen); }

		float mEmitsPerSecond{ 0.0 };
	protected:
		std::vector<Ref<ParticleGenerator>> mGenerators;
	private:
		float mEmitTime{ 0 };
    };

	///// Particle System /////
    class ParticleSystem {
	public:
		explicit ParticleSystem(uint32_t maxCount = 10000);
		~ParticleSystem() {}

		void Update(float deltaTime);
		void Render();
		void Reset();

		uint32_t AllParticlesCount() const { return mParticles.mCount; }
		uint32_t AliveParticlesCount() const { return mParticles.mAliveCount; }

		void addEmitter(Ref<ParticleEmitter> em) { mEmitters.push_back(em); }
		void addUpdater(Ref<ParticleUpdater> up) { mUpdaters.push_back(up); }

		ParticleData* finalData() { return &mParticles; }
	protected:
		ParticleData mParticles;
		// ParticleData mAliveParticles; ?

		uint32_t mCount;

		std::vector<Ref<ParticleEmitter>> mEmitters;
		std::vector<Ref<ParticleUpdater>> mUpdaters;
    };

}