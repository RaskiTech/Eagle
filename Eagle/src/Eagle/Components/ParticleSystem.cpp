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

			glm::vec2 size = glm::lerp(particle.startSize, mProps.sizeOverLifetime+particle.startSize, lifePercentage);
			float rotation = glm::lerp(mProps.rotationOverLifetime+particle.startRotation, particle.startRotation, lifePercentage);
			glm::vec3 position = { particle.startPosition + particle.startVelocity * lifePercentage + mProps.acceleration * lifePercentage * lifePercentage, mProps.zPosition };

			Renderer::DrawRotatedColorQuad(position, rotation, size, particle.color);
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
		data.startPosition = position;
		data.startRotation = Random::Float(mProps.minRotation, mProps.maxRotation);
		data.startSize = glm::lerp(mProps.minSize, mProps.maxSize, Random::Float01());

		float angle = Random::Float(mProps.emitDirOffset, mProps.emitAngleWidthRadiant + mProps.emitDirOffset);
		data.startVelocity = glm::vec2(glm::cos(angle), sin(angle)) * Random::Float(mProps.minVelocity, mProps.maxVelocity);

		mNextParticleIndex = --mNextParticleIndex % mData.size();
	}
}