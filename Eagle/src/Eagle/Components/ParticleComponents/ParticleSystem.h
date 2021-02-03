#pragma once
#include "Eagle/Core/Core.h"
#include "ParticleData.h"
#include "ParticleSetters.h"
#include "ParticleUpdater.h"

namespace Egl {
	namespace Particles {
		///// Particle Emitter /////
		struct Emitter {
		public:
			Emitter(float emitsPerSecond = 0) : mEmitsPerSecond(emitsPerSecond) {}
			virtual ~Emitter() = default;

			// calls all the generators and at the end it activates (wakes) particle
			virtual void Emit(float dt, ParticleData* data, TransformComponent& tr);

			void AddSetter(Ref<ParticleSetter> gen) { mSetters.push_back(gen); }
			auto& GetSetters() const { return mSetters; }

			void SetEmitsPerSecond(float emitsPerSecond) { mEmitsPerSecond  = emitsPerSecond; }
			float GetEmistPerSecond() const { return mEmitsPerSecond; }

		private:
			float mEmitsPerSecond = 0;
			std::vector<Ref<ParticleSetter>> mSetters;
			float mEmitTime{ 0 };
		};

		///// Particle System /////
		class ParticleSystem {
		public:
			explicit ParticleSystem(uint32_t maxCount = 10000);
			~ParticleSystem() {}

			void Update(float deltaTime, TransformComponent& tr);
			void Render();
			void Reset();

			uint32_t AllParticlesCount() const { return mParticles.mCount; }
			uint32_t AliveParticlesCount() const { return mParticles.mAliveCount; }

			void AddEmitter(Ref<Emitter> em) { mEmitters.push_back(em); }
			void AddUpdater(Ref<ParticleUpdater> up) { mUpdaters.push_back(up); }
			auto& GetEmitters() const { return mEmitters; }
			auto& GetUpdaters() const { return mUpdaters; }

			const ParticleData* FinalData() { return &mParticles; }
		private:
			ParticleData mParticles;

			uint32_t mCount;

			std::vector<Ref<Emitter>> mEmitters;
			std::vector<Ref<ParticleUpdater>> mUpdaters;
		};
	}
}