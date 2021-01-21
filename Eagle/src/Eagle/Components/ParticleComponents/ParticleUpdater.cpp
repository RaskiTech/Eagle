#include <EaglePCH.h>
#include <glm/gtc/type_ptr.hpp>
#include <Dependencies/ImGui.h>
#include "Eagle/Core/UniqueID.h"
#include "ParticleUpdater.h"
#include "ParticleData.h"

namespace Egl {
	namespace Particles {
		void EulerUpdater::Update(float deltaTime, ParticleData* data) {
			const uint32_t endId = data->mAliveCount;

			uint32_t i;
			for (i = 0; i < endId; ++i)
				data->acceleration[i] += globalAcceleration;

			for (i = 0; i < endId; ++i)
				data->velocity[i] += data->acceleration[i] * deltaTime;

			for (i = 0; i < endId; ++i)
				data->position[i] += data->velocity[i] * deltaTime;
		}
		void EulerUpdater::OnImGuiRender() {
			ImGui::Text("Euler updater");
			ImGui::Indent();
			ImGui::DragFloat2("Global Acceleration", glm::value_ptr(globalAcceleration));
			ImGui::Unindent();
		}

		void ConstSpeedUpdater::Update(float deltaTime, ParticleData* data) {
			const uint32_t endId = data->mAliveCount;
			for (uint32_t i = 0; i < endId; ++i)
				data->position[i] += data->velocity[i] * deltaTime;
		}
		void ConstSpeedUpdater::OnImGuiRender() {
			ImGui::Text("Constant speed updater");
		}

		void FloorUpdater::Update(float deltaTime, ParticleData* data) {
			const uint32_t endId = data->mAliveCount;
			for (uint32_t i = 0; i < endId; ++i) {
				if (data->position[i].y < floorY) {
					glm::vec2 force = data->acceleration[i];
					float normalFactor = glm::dot(force, glm::vec2(0.0f, 1.0f));

					if (normalFactor < 0.0f)
						force -= glm::vec2(0.0f, 1.0f) * normalFactor;

					float velFactor = glm::dot(data->velocity[i], glm::vec2(0.0f, 1.0f));

					data->velocity[i] -= glm::vec2(0.0f, 1.0f) * (1.0f + bounceFactor) * velFactor;
					data->acceleration[i] = force;
				}
			}

		}
		void FloorUpdater::OnImGuiRender() {
			ImGui::Text("Floor updater");
			ImGui::Indent();
			ImGui::DragFloat("FloorY", &floorY);
			ImGui::DragFloat("Bounce amount", &bounceFactor);
			ImGui::Unindent();
		}

		void AttractorUpdater::Update(float deltaTime, ParticleData* data) {
			const uint32_t endId = data->mAliveCount;
			const uint32_t attractorCount = (uint32_t)mAttractors.size();
			glm::vec2 particleOffset;
			float dist;
			uint32_t a;
			for (uint32_t i = 0; i < endId; ++i) {
				for (a = 0; a < attractorCount; ++a) {
					particleOffset.x = mAttractors[a].x - data->position[i].x;
					particleOffset.y = mAttractors[a].y - data->position[i].y;
					dist = glm::dot(particleOffset, particleOffset);

					//if (fabs(dist) > 0.00001)
					dist = mAttractors[a].z / dist;

					data->acceleration[i] += particleOffset * dist;
				}
			}
		}
		void AttractorUpdater::OnImGuiRender() {
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

		void ColorUpdater::Update(float deltaTime, ParticleData* data) {
			const uint32_t endId = data->mAliveCount;
			for (uint32_t i = 0; i < endId; ++i)
				data->color[i] = glm::mix(data->endCol[i], data->startCol[i], data->time[i].x);
		}
		void ColorUpdater::OnImGuiRender() {
			ImGui::Text("Color updater");
		}

		void PosToColorUpdater::Update(float deltaTime, ParticleData* data) {
			const uint32_t endId = data->mAliveCount;
			float scaler, scaleg;
			float diffr = maxPos.x - minPos.x;
			float diffg = maxPos.y - minPos.y;
			for (uint32_t i = 0; i < endId; ++i) {
				scaler = (data->position[i].x - minPos.x) / diffr;
				scaleg = (data->position[i].y - minPos.y) / diffg;
				data->color[i].r = scaler;// glm::mix(p->mStartCol[i].r, p->mEndCol[i].r, scaler);
				data->color[i].g = scaleg;// glm::mix(p->mStartCol[i].g, p->mEndCol[i].g, scaleg);
				data->color[i].b = glm::mix(data->endCol[i].b, data->startCol[i].b, data->time[i].y);
				data->color[i].a = glm::mix(data->endCol[i].a, data->startCol[i].a, data->time[i].x);
			}
		}
		void PosToColorUpdater::OnImGuiRender() {
			ImGui::Text("Position updater");
			ImGui::Indent();
			ImGui::DragFloat2("Min position", glm::value_ptr(minPos));
			ImGui::DragFloat2("Max position", glm::value_ptr(maxPos));
			ImGui::Unindent();
		}

		void VelocityToColorUpdater::Update(float deltaTime, ParticleData* data) {
			const uint32_t endId = data->mAliveCount;
			float scaler, scaleg;
			float diffr = maxVel.x - minVel.x;
			float diffg = maxVel.y - minVel.y;
			for (uint32_t i = 0; i < endId; ++i) {
				scaler = (data->velocity[i].x - minVel.x) / diffr;
				scaleg = (data->velocity[i].y - minVel.y) / diffg;
				data->color[i].r = scaler;// glm::mix(p->mStartCol[i].r, p->mEndCol[i].r, scaler);
				data->color[i].g = scaleg;// glm::mix(p->mStartCol[i].g, p->mEndCol[i].g, scaleg);
				data->color[i].b = glm::mix(data->endCol[i].b, data->startCol[i].b, glm::atan(diffg, diffr));
				data->color[i].a = glm::mix(data->endCol[i].a, data->startCol[i].a, data->time[i].x);
			}
		}
		void VelocityToColorUpdater::OnImGuiRender() {
			ImGui::Text("Velocity color updater");
			ImGui::Indent();
			ImGui::DragFloat2("Min velocity", glm::value_ptr(minVel));
			ImGui::DragFloat2("Max velocity", glm::value_ptr(maxVel));
			ImGui::Unindent();
		}

		void TimeUpdater::Update(float deltaTime, ParticleData* data) {
			uint32_t endId = data->mAliveCount;

			for (uint32_t i = 0; i < endId; ++i) {
				data->time[i].x -= deltaTime / data->time[i].y;
				if (data->time[i].x < 0) {
					data->Kill(i);
					endId = data->mAliveCount < data->mCount ? data->mAliveCount : data->mCount;
				}
			}
		}
		void TimeUpdater::OnImGuiRender() {
			ImGui::Text("Time updater");
		}

		void EndSizeUpdater::Update(float deltaTime, ParticleData* data) {
			uint32_t endId = data->mAliveCount;
			for (uint32_t i = 0; i < endId; ++i)
				data->size[i] = (data->startSize[i]-endSize) * (data->time[i].x) + endSize;
		}
		void EndSizeUpdater::OnImGuiRender() {
			ImGui::Text("End size updater");
			ImGui::Indent();
			ImGui::DragFloat2("End size", glm::value_ptr(endSize));
			ImGui::Unindent();
		}

		void SizeChangeUpdater::Update(float deltaTime, ParticleData* data) {
			uint32_t endId = data->mAliveCount;
			glm::vec2 amountToChange = deltaTime * sizeChangeInSecond;
			for (uint32_t i = 0; i < endId; ++i)
				data->size[i] += amountToChange;
		}
		void SizeChangeUpdater::OnImGuiRender() {
			ImGui::Text("Size change updater");
			ImGui::Indent();
			ImGui::DragFloat2("Size change", glm::value_ptr(sizeChangeInSecond));
			ImGui::Unindent();
		}

}
}