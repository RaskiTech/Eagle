#pragma once
#include "Entity.h"

// Baseclass for all client scripts
namespace Egl {

	class Script {
	public:

		template<typename T>
		T& GetComponent() const { return mEntity.GetComponent<T>(); }
		template<typename T>
		T& AddComponent() const { return mEntity.AddComponent<T>(); }

	private:
		Entity mEntity;
		friend class GameLayer;
	};
}