#include <EaglePCH.h>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Dependencies/ImGui.h>
#include "ParticleGenerators.h"

namespace Egl {
	// BoxGen
    void ParticleBoxGenerator::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
        glm::vec2 posMin { mPos.x - mMaxStartPosOffset.x, mPos.y - mMaxStartPosOffset.y };
        glm::vec2 posMax { mPos.x + mMaxStartPosOffset.x, mPos.y + mMaxStartPosOffset.y };
        for (uint32_t i = startId; i < endId; ++i)
            p->mPos[i] = glm::linearRand(posMin, posMax);
    }
	void ParticleBoxGenerator::OnImGuiRender() {
		ImGui::Text("Box Generator");
		ImGui::Indent();
		ImGui::DragFloat2("Position", glm::value_ptr(mPos));
		ImGui::DragFloat2("MaxOffset", glm::value_ptr(mMaxStartPosOffset));
		ImGui::Unindent();
	}

	// RoundGen
	void RoundPosGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i) {
			float ang = glm::linearRand(0.0f, glm::pi<float>() * 2.0f);
			p->mPos[i] = mCenter + glm::vec2(mRadius.x * sin(ang), mRadius.y * cos(ang));
		}
	}
	void RoundPosGen::OnImGuiRender() {
		ImGui::Text("Circle Generator");
		ImGui::Indent();
		ImGui::DragFloat2("Position", glm::value_ptr(mCenter));
		ImGui::DragFloat2("Radius", glm::value_ptr(mRadius));
		ImGui::Unindent();
	}

	// ColorGen
	void BasicColorGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i) {
			p->mStartCol[i] = glm::linearRand(mMinStartCol, mMaxStartCol);
			p->mEndCol[i] = glm::linearRand(mMinEndCol, mMaxEndCol);
		}
	}
	void BasicColorGen::OnImGuiRender() {
		ImGui::Text("Color Generator");
		ImGui::Indent();
		ImGui::DragFloat4("StartColor Min", glm::value_ptr(mMinStartCol), 0.1f, 0, 1);
		ImGui::DragFloat4("StartColor Max", glm::value_ptr(mMaxStartCol), 0.1f, 0, 1);
		ImGui::DragFloat4("EndColor Min ", glm::value_ptr(mMinEndCol), 0.1f, 0, 1);
		ImGui::DragFloat4("EndColor Max", glm::value_ptr(mMaxEndCol), 0.1f, 0, 1);
		ImGui::Unindent();
	}

	// VelGen
	void BasicVelGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i)
			p->mVel[i] = glm::linearRand(mMinStartVel, mMaxStartVel);
	}
	void BasicVelGen::OnImGuiRender() {
		ImGui::Text("Velocity Generator");
		ImGui::Indent();
		ImGui::DragFloat2("Min Velocity", glm::value_ptr(mMinStartVel));
		ImGui::DragFloat2("Max Velocity", glm::value_ptr(mMaxStartVel));
		ImGui::Unindent();
	}

	// TimeGen
	void BasicTimeGen::generate(float deltaTime, ParticleData* p, uint32_t startId, uint32_t endId) {
		for (uint32_t i = startId; i < endId; ++i) {
			p->mTime[i] = { 0, glm::linearRand(mMinTime, mMaxTime) };
		}
	}
	void BasicTimeGen::OnImGuiRender() {
		ImGui::Text("Time Generator");
		ImGui::Indent();
		ImGui::DragFloat("Min Time", &mMinTime);
		ImGui::DragFloat("Max Time", &mMaxTime);
		ImGui::Unindent();
	}
}