#include <EaglePCH.h>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Dependencies/ImGui.h>
#include "ParticleSetters.h"

namespace Egl {
	namespace Particles {

		void BoxPosSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			glm::vec2 posMin{ position.x - maxOffset.x, position.y - maxOffset.y };
			glm::vec2 posMax{ position.x + maxOffset.x, position.y + maxOffset.y };
			for (uint32_t i = startId; i < endId; ++i)
				data->position[i] = glm::linearRand(posMin, posMax);
		}
		void BoxPosSetter::OnImGuiRender() {
			ImGui::Text("Box Generator");
			ImGui::Indent();
			ImGui::DragFloat2("Position", glm::value_ptr(position));
			ImGui::DragFloat2("MaxOffset", glm::value_ptr(maxOffset));
			ImGui::Unindent();
		}

		void CirclePosSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i) {
				float ang = glm::linearRand(0.0f, glm::pi<float>() * 2.0f);
				data->position[i] = position + glm::vec2(radius.x * sin(ang), radius.y * cos(ang));
			}
		}
		void CirclePosSetter::OnImGuiRender() {
			ImGui::Text("Circle Generator");
			ImGui::Indent();
			ImGui::DragFloat2("Position", glm::value_ptr(position));
			ImGui::DragFloat2("Radius", glm::value_ptr(radius));
			ImGui::Unindent();
		}

		void ColorSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i) {
				data->startCol[i] = glm::linearRand(minStartColor, maxStartColor);
				data->endCol[i] = glm::linearRand(minEndColor, maxEndColor);
			}
		}
		void ColorSetter::OnImGuiRender() {
			ImGui::Text("Color Generator");
			ImGui::Indent();
			ImGui::DragFloat4("StartColor Min", glm::value_ptr(minStartColor), 0.1f, 0, 1);
			ImGui::DragFloat4("StartColor Max", glm::value_ptr(maxStartColor), 0.1f, 0, 1);
			ImGui::DragFloat4("EndColor Min ", glm::value_ptr(minEndColor), 0.1f, 0, 1);
			ImGui::DragFloat4("EndColor Max", glm::value_ptr(maxEndColor), 0.1f, 0, 1);
			ImGui::Unindent();
		}

		void VelocitySetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i)
				data->velocity[i] = glm::linearRand(minVelocity, maxVelocity);
		}
		void VelocitySetter::OnImGuiRender() {
			ImGui::Text("Velocity Generator");
			ImGui::Indent();
			ImGui::DragFloat2("Min Velocity", glm::value_ptr(minVelocity));
			ImGui::DragFloat2("Max Velocity", glm::value_ptr(maxVelocity));
			ImGui::Unindent();
		}

		void TimeSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i)
				data->time[i] = { 1, glm::linearRand(minTime, maxTime) };
		}
		void TimeSetter::OnImGuiRender() {
			ImGui::Text("Time Generator");
			ImGui::Indent();
			ImGui::DragFloat("Min Time", &minTime);
			ImGui::DragFloat("Max Time", &maxTime);
			ImGui::Unindent();
		}

		void SizeSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i) {
				const glm::vec2 size = glm::linearRand(minSize, maxSize);
				data->size[i] = size;
				data->startSize[i] = size;
			}
		}
		void SizeSetter::OnImGuiRender() {
			ImGui::Text("Size setter");
			ImGui::Indent();
			ImGui::DragFloat2("Min Size", glm::value_ptr(minSize));
			ImGui::DragFloat2("Max Size", glm::value_ptr(maxSize));
			ImGui::Unindent();
		}
	}
}