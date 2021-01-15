#include <EaglePCH.h>
#include "GameLayer.h"

// This wouldn't be inside Eagle, but probably somewhere defined.
#include "Eagle/Scripting/ExampleScript.h"

namespace Egl {
	GameLayer::GameLayer() {
		
	}
	void GameLayer::OnAttach() {
		mActiveScene = ApplicationStartup();
		mActiveScene->SceneBegin();

		//mActiveScene->SetViewportAspectRatio((float)Application::Get().GetWindow().GetWidth() / Application::Get().GetWindow().GetWidth());
	}
	void GameLayer::OnDetach() {
		mActiveScene->SceneEnd();
	}
	void GameLayer::OnUpdate() {
		mActiveScene->OnUpdate();
		LOG("Game UPDATE");
	}
	void GameLayer::ResetApplication() {
		OnDetach();
		OnAttach();
	}
	void GameLayer::OnEvent(Event& event) {

	}
	void GameLayer::OnImGuiRender() {

	}
}