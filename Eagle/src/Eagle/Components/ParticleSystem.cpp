#include <EaglePCH.h>
#include "ParticleSystem.h"
#include "Eagle/Core/Random.h"
#include "Eagle/Core/Time.h"
#include <glm/gtx/compatibility.hpp>

#include "Eagle/Rendering/Renderer.h"

namespace Egl {
	ParticleSystem::ParticleSystem(ParticleSystemProps& particleProps, uint32_t particleAmount = 10000)
		: mProps(particleProps), mNextParticleIndex(particleAmount-1) 
	{
		EAGLE_PROFILE_FUNCTION();
		mData.resize(particleAmount);
	}

	void ParticleSystem::OnRender(Camera& camera) {
		EAGLE_PROFILE_FUNCTION();
		Renderer::BeginScene(camera);

		for (ParticleData& particle : mData) {
			if (!particle.active)
				continue;

			if (particle.beenAlive > particle.lifeTime) {
				particle.active = false;
				continue;
			}

			particle.beenAlive += Time::GetFrameDelta();
			float lifePercentage = particle.beenAlive / particle.lifeTime;

			particle.velocity += particle.velocity * mProps.acceleration * Time::GetFrameDelta();
			particle.position += particle.velocity * Time::GetFrameDelta();
			particle.rotation += mProps.rotationOverLifetime * Time::GetFrameDelta();

			glm::vec2 size = glm::lerp(particle.startSize, mProps.sizeOverLifetime+particle.startSize, lifePercentage);

			Renderer::DrawRotatedColorQuad(glm::vec3(particle.position, mProps.zPosition), particle.rotation, size, particle.color);
		}
		Renderer::EndScene();
	}

	void ParticleSystem::Emit(const glm::vec2& position) {
		EAGLE_PROFILE_FUNCTION();
		ParticleData& data = mData[mNextParticleIndex];
		data.active = true;
		data.lifeTime = Random::Float(mProps.minLifeTime, mProps.maxLifetime);
		data.beenAlive = 0;
		data.color = glm::lerp(mProps.minColor, mProps.maxColor, Random::Float01());
		data.position = position;
		data.rotation = Random::Float(mProps.minRotation, mProps.maxRotation);
		data.startSize = glm::lerp(mProps.minSize, mProps.maxSize, Random::Float01());

		float angle = Random::Float(mProps.emitDirOffset, mProps.emitAngleWidthRadiant + mProps.emitDirOffset);
		data.velocity = glm::vec2(glm::cos(angle), sin(angle)) * Random::Float(mProps.minVelocity, mProps.maxVelocity);

		mNextParticleIndex = --mNextParticleIndex % mData.size();
	}
}