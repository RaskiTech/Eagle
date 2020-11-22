#pragma once
#include "Core.h"

namespace Egl {

	class EAGLE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// Client will define
	Application* CreateApplication();
}
