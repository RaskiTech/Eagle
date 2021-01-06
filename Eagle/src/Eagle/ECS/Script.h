#pragma once
#include "Entity.h"

// Baseclass for all client scripts
namespace Egl {

	class Script {
	public:

		template<typename T>
		T& GetComponent() const { return mEntity.GetComponent<T>(); }
	
	private:
		Entity mEntity;
		friend class Scene;
	};
}