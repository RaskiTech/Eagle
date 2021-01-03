#include <Eagle.h>
#include <Eagle/Core/EntryPoint.h>

#include "ImGui/imgui.h"
#include "EditorLayer.h"

namespace Egl {

	class EagleEditor : public Application {
	public:
		EagleEditor() : Application("Eagle Editor") {
			EAGLE_PROFILE_FUNCTION();
			AddLayer(new EditorLayer());
		}

		~EagleEditor() {

		}
	};

	Application* CreateApplication() {
		return new EagleEditor();
	}

}