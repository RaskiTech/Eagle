#include <EaglePCH.h>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Dependencies/ImGui.h>
#include "ParticleSetters.h"
#include "Eagle/Core/Random.h"

namespace Egl {
	namespace Particles {

		void BoxPosSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			//glm::vec2 posMin{ position.x - maxOffset.x, position.y - maxOffset.y };
			//glm::vec2 posMax{ position.x + maxOffset.x, position.y + maxOffset.y };
			float posMinX = position.x - maxOffset.x;
			float posMinY = position.y - maxOffset.y;
			float posMaxX = position.x + maxOffset.x;
			float posMaxY = position.y + maxOffset.y;

			for (uint32_t i = startId; i < endId; ++i)
				data->position[i] = { Random::Float(posMinX, posMaxX), Random::Float(posMinY, posMaxY) }; // glm::linearRand(posMin, posMax);
		}
		void BoxPosSetter::OnImGuiRender() {
			ImGui::Text("Box Setter");
			ImGui::Indent();
			ImGui::DragFloat2("Position", glm::value_ptr(position));
			ImGui::DragFloat2("MaxOffset", glm::value_ptr(maxOffset));
			ImGui::Unindent();
		}

		void CirclePosSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			constexpr float pi2 = glm::pi<float>() * 2.0f;
			for (uint32_t i = startId; i < endId; ++i) {
				float ang = Random::Float(pi2); // glm::linearRand(0.0f, glm::pi<float>() * 2.0f);
				data->position[i] = position + glm::vec2(radius.x * sin(ang), radius.y * cos(ang));
			}
		}
		void CirclePosSetter::OnImGuiRender() {
			ImGui::Text("Circle Setter");
			ImGui::Indent();
			ImGui::DragFloat2("Position", glm::value_ptr(position));
			ImGui::DragFloat2("Radius", glm::value_ptr(radius));
			ImGui::Unindent();
		}

		void ColorSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i) {
				data->startCol[i] = { Random::Float(minStartColor.r, maxStartColor.r), Random::Float(minStartColor.g, maxStartColor.g), Random::Float(minStartColor.b, maxStartColor.b), Random::Float(minStartColor.a, maxStartColor.a) };  // glm::linearRand(minStartColor, maxStartColor);
				data->endCol[i] = { Random::Float(minEndColor.r, maxEndColor.r), Random::Float(minEndColor.g, maxEndColor.g), Random::Float(minEndColor.b, maxEndColor.b), Random::Float(minEndColor.a, maxEndColor.a) };  // glm::linearRand(minEndColor, maxEndColor);
			}
		}
		void ColorSetter::OnImGuiRender() {
			ImGui::Text("Color Setter");
			ImGui::Indent();
			ImGui::DragFloat4("StartColor Min", glm::value_ptr(minStartColor), 0.1f, 0, 1);
			ImGui::DragFloat4("StartColor Max", glm::value_ptr(maxStartColor), 0.1f, 0, 1);
			ImGui::DragFloat4("EndColor Min ", glm::value_ptr(minEndColor), 0.1f, 0, 1);
			ImGui::DragFloat4("EndColor Max", glm::value_ptr(maxEndColor), 0.1f, 0, 1);
			ImGui::Unindent();
		}

		void VelocitySetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i)
				data->velocity[i] = { Random::Float(minVelocity.x, maxVelocity.x), Random::Float(minVelocity.y, maxVelocity.y) }; // glm::linearRand(minVelocity, maxVelocity);
		}
		void VelocitySetter::OnImGuiRender() {
			ImGui::Text("Velocity Setter");
			ImGui::Indent();
			ImGui::DragFloat2("Min Velocity", glm::value_ptr(minVelocity));
			ImGui::DragFloat2("Max Velocity", glm::value_ptr(maxVelocity));
			ImGui::Unindent();
		}

		void TimeSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i)
				data->time[i] = { 1, Random::Float(minTime, maxTime) }; // glm::linearRand(minTime, maxTime) };
		}
		void TimeSetter::OnImGuiRender() {
			ImGui::Text("Time Setter");
			ImGui::Indent();
			ImGui::DragFloat("Min Time", &minTime);
			ImGui::DragFloat("Max Time", &maxTime);
			ImGui::Unindent();
		}

		void SizeSetter::Apply(float deltaTime, ParticleData* data, uint32_t startId, uint32_t endId) {
			for (uint32_t i = startId; i < endId; ++i) {
				const glm::vec2 size = { Random::Float(minSize.x, maxSize.x), Random::Float(minSize.y, maxSize.y) }; // glm::linearRand(minSize, maxSize);
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