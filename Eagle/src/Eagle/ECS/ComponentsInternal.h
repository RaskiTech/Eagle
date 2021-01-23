#pragma once
#include "Dependencies/Entt.h"

// Components that shouldn't be accessable to the client

namespace Egl {
	struct Relation {
		entt::entity parent = entt::null;

		entt::entity previousSibling = entt::null;
		entt::entity nextSibling = entt::null;

		std::uint8_t childrenCount = 0;
		entt::entity firstChild = entt::null;
	};
}