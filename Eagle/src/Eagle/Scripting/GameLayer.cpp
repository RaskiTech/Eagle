#include <EaglePCH.h>
#include <EagleApplicationStart.h>
#include "GameLayer.h"

namespace Egl {
	GameLayer::GameLayer() {
		
	}
	void GameLayer::OnAttach() {
		mActiveScene = ApplicationStartup();
		mActiveScene->SceneBegin();
	}
	void GameLayer::OnDetach() {
		mActiveScene->SceneEnd();
	}
	void GameLayer::OnUpdate() {
		mActiveScene->OnUpdate();
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