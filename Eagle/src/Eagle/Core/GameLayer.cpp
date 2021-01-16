#include <EaglePCH.h>
#include <EagleApplicationStartup.h>
#include "GameLayer.h"

namespace Egl {
	GameLayer::GameLayer() {
		
	}
	void GameLayer::OnAttach() {
		{
			EAGLE_PROFILE_SCOPE("Application - ApplicationStartup");
			mActiveScene = ApplicationStartup();
		}
		{
			EAGLE_PROFILE_SCOPE("Application - SceneBegin");
			mActiveScene->SceneBegin();
		}
	}
	void GameLayer::OnDetach() {
		{
			EAGLE_PROFILE_SCOPE("Application - SceneEnd");
			mActiveScene->SceneEnd();
		}
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