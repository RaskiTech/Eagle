#pragma once
#include "Eagle/Core/Core.h"
#include "Eagle/ECS/Entity.h"

namespace Egl {
	class PropertiesPanel {
	public:
		void OnImGuiRender();
		void SetDrawedEntity(Entity e);
	private:
		void DrawProperties();

		// UITransformComponent
		float value1Speed = 1, value2Speed = 1, value3Speed = 1, value4Speed = 1;

		Entity drawedEntity;
	};
}