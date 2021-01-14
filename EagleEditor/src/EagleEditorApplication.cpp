#include <Eagle.h>
#include "Eagle/Core/Application.h"
#include "ImGui/imgui.h"
#include "EditorLayer.h"

namespace Egl {

	class EagleEditor : public Application {
	public:
		EagleEditor() : Application("Eagle Editor") {
			EAGLE_PROFILE_FUNCTION();
			mEditorLayer = new EditorLayer();
			AddLayer(mEditorLayer);
		}
		~EagleEditor() {

		}
	private:
		Layer* mEditorLayer;
	};

	Application* CreateApplication() {
		return new EagleEditor();
	}
	Layer* MakeEditorLayer() {
		return new EditorLayer();
	}

}