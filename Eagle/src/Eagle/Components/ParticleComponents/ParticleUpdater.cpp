#include <EaglePCH.h>
#include <glm/gtc/type_ptr.hpp>
#include <Dependencies/ImGui.h>
#include "Eagle/Core/UniqueID.h"
#include "ParticleUpdater.h"
#include "ParticleData.h"

namespace Egl {
    void ParticleUpdaterEuler::Update(float deltaTime, ParticleData* p) {
        const glm::vec2 globalA { mGlobalAcceleration.x, mGlobalAcceleration.y };
        const uint32_t endId = p->mAliveCount;

		uint32_t i;
        for (i = 0; i < endId; ++i)
            p->mAcc[i] += globalA;

        for (i = 0; i < endId; ++i)
            p->mVel[i] += p->mAcc[i] * deltaTime;

        for (i = 0; i < endId; ++i)
            p->mPos[i] += p->mVel[i] * deltaTime;
    }
	void ParticleUpdaterEuler::OnImGuiRender() {
		ImGui::Text("Euler updater");
		ImGui::Indent();
		ImGui::DragFloat2("Global Acceleration", glm::value_ptr(mGlobalAcceleration));
		ImGui::Unindent();
	}

	void ParticleUpdaterFloor::Update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		for (uint32_t i = 0; i < endId; ++i)
		{
			if (p->mPos[i].y < mFloorY)
			{
				glm::vec2 force = p->mAcc[i];
				float normalFactor = glm::dot(force, glm::vec2(0.0f, 1.0f));
				if (normalFactor < 0.0f)
					force -= glm::vec2(0.0f, 1.0f) * normalFactor;

				float velFactor = glm::dot(p->mVel[i], glm::vec2(0.0f, 1.0f));

				p->mVel[i] -= glm::vec2(0.0f, 1.0f) * (1.0f + mBounceFactor) * velFactor;

				p->mAcc[i] = force;
			}
		}

	}
	void ParticleUpdaterFloor::OnImGuiRender() {
		ImGui::Text("Floor updater");
		ImGui::Indent();
		ImGui::DragFloat("FloorY", &mFloorY);
		ImGui::DragFloat("Bounce amount", &mBounceFactor);
		ImGui::Unindent();
	}

	void ParticleUpdaterAttractor::Update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		const uint32_t attractorCount = (uint32_t)mAttractors.size();
		glm::vec2 particleOffset;
		float dist;
		uint32_t a;
		for (uint32_t i = 0; i < endId; ++i) {
			for (a = 0; a < attractorCount; ++a) {
				particleOffset.x = mAttractors[a].x - p->mPos[i].x;
				particleOffset.y = mAttractors[a].y - p->mPos[i].y;
				dist = glm::dot(particleOffset, particleOffset);

				//if (fabs(dist) > 0.00001)
				dist = mAttractors[a].z / dist;

				p->mAcc[i] += particleOffset * dist;
			}
		}
	}
	void ParticleUpdaterAttractor::OnImGuiRender() {
		ImGui::Text("Attractor updater");

		if (ImGui::Button("Add Attractor"))
			mAttractors.push_back(glm::vec3(0));

		for (auto& attractor : mAttractors) {
			ImGui::PushID(UniqueID::GetUniqueFrameID());
			ImGui::Text("Attractor");
			ImGui::Indent();

			ImGui::DragFloat2("Position", glm::value_ptr(attractor));
			ImGui::DragFloat("Force", &attractor.z);

			ImGui::Unindent();
			ImGui::PopID();
		}

	}

	void ParticleUpdaterBasicColor::Update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		for (uint32_t i = 0; i < endId; ++i)
			p->mCol[i] = glm::mix(p->mStartCol[i], p->mEndCol[i], p->mTime[i].x);
	}
	void ParticleUpdaterBasicColor::OnImGuiRender() {
		ImGui::Text("Color updater");
	}

	void ParticleUpdaterPosColor::Update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		float scaler, scaleg;
		float diffr = mMaxPos.x - mMinPos.x;
		float diffg = mMaxPos.y - mMinPos.y;
		for (uint32_t i = 0; i < endId; ++i)
		{
			scaler = (p->mPos[i].x - mMinPos.x) / diffr;
			scaleg = (p->mPos[i].y - mMinPos.y) / diffg;
			p->mCol[i].r = scaler;// glm::mix(p->mStartCol[i].r, p->mEndCol[i].r, scaler);
			p->mCol[i].g = scaleg;// glm::mix(p->mStartCol[i].g, p->mEndCol[i].g, scaleg);
			p->mCol[i].b = glm::mix(p->mStartCol[i].b, p->mEndCol[i].b, p->mTime[i].y);
			p->mCol[i].a = glm::mix(p->mStartCol[i].a, p->mEndCol[i].a, p->mTime[i].x);
		}
	}
	void ParticleUpdaterPosColor::OnImGuiRender() {
		ImGui::Text("Position updater");
		ImGui::DragFloat2("Min position", glm::value_ptr(mMinPos));
		ImGui::DragFloat2("Max position", glm::value_ptr(mMaxPos));
	}

	void ParticleUpdaterVelColor::Update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		float scaler, scaleg;
		float diffr = mMaxVel.x - mMinVel.x;
		float diffg = mMaxVel.y - mMinVel.y;
		for (uint32_t i = 0; i < endId; ++i)
		{
			scaler = (p->mVel[i].x - mMinVel.x) / diffr;
			scaleg = (p->mVel[i].y - mMinVel.y) / diffg;
			p->mCol[i].r = scaler;// glm::mix(p->mStartCol[i].r, p->mEndCol[i].r, scaler);
			p->mCol[i].g = scaleg;// glm::mix(p->mStartCol[i].g, p->mEndCol[i].g, scaleg);
			p->mCol[i].b = glm::mix(p->mStartCol[i].b, p->mEndCol[i].b, glm::atan(diffg, diffr));
			p->mCol[i].a = glm::mix(p->mStartCol[i].a, p->mEndCol[i].a, p->mTime[i].x);
		}
	}
	void ParticleUpdaterVelColor::OnImGuiRender() {
		ImGui::Text("Velocity color updater");
		ImGui::DragFloat2("Min velocity", glm::value_ptr(mMinVel));
		ImGui::DragFloat2("Max velocity", glm::value_ptr(mMaxVel));

	}

	void ParticleUpdaterBasicTime::Update(float deltaTime, ParticleData* p) {
		uint32_t endId = p->mAliveCount;
		if (endId == 0)
			return;

		for (uint32_t i = 0; i < endId; ++i) {
			p->mTime[i].x += deltaTime / p->mTime[i].y;
			if (p->mTime[i].x > 1) {
				p->kill(i);
				endId = p->mAliveCount < p->mCount ? p->mAliveCount : p->mCount;
			}
		}
	}
	void ParticleUpdaterBasicTime::OnImGuiRender() {
		ImGui::Text("Time updater");
	}

	void ParticleUpdaterConstantSpeed::Update(float deltaTime, ParticleData* p) {
		const uint32_t endId = p->mAliveCount;
		for (uint32_t i = 0; i < endId; ++i)
			p->mPos[i] += p->mVel[i] * deltaTime;
	}
	void ParticleUpdaterConstantSpeed::OnImGuiRender() {
		ImGui::Text("Constant speed updater");
	}
}