#pragma once
#include <glm/glm.hpp>
#include "Eagle/Rendering/Texture.h"
#include "SceneCamera.h"

namespace Egl {
	struct TransformComponent {
		glm::mat4 transform = glm::mat4(1);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : transform(transform) {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
	};
	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag) : tag(tag) {};

	};

	struct SpriteComponent {
		glm::vec4 color = { 1, 1, 1, 1 };
		//Texture texture;

		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) : color(color) {}
	};

	struct CameraComponent {
		SceneCamera camera;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
	};
}