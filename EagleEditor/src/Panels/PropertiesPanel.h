#pragma once
#include "Eagle/Core/Core.h"
#include "Eagle/ECS/Entity.h"

namespace Egl {
	class PropertiesPanel {
	public:
		void OnImGuiRender();
		void SetDrawedEntity(Entity e) { drawedEntity = e; }
	private:
		void DrawProperties();

		Entity drawedEntity;
	};
}