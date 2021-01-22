#include <EaglePCH.h>
#include <EagleApplicationStartup.h>
#include "Eagle/ECS/Components.h"
#include "GameLayer.h"
#include "Input.h"

// Handles calling input and client functions.

namespace Egl {
	GameLayer::GameLayer() {
		
	}
	void GameLayer::OnAttach() {
		// Might need to move some of this stuff when scenes can be changed
		{
			EAGLE_PROFILE_SCOPE("Application - ApplicationStartup");
			mActiveScene = ApplicationStartup();
		}
		{
			EAGLE_PROFILE_SCOPE("Application - SceneBegin");
			mActiveScene->SceneBegin();
		}
		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: OnCreate");
			LOG("starting");
			mActiveScene->mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				scriptComponent.InstantiateFunc();
				LOG("run it once {0}", !scriptComponent.baseInstance);
				scriptComponent.baseInstance->mEntity = Entity{ entity, mActiveScene.get() };
				LOG("run it once {0}", !scriptComponent.baseInstance);
				if (scriptComponent.OnCreateFunc)
					scriptComponent.OnCreateFunc(scriptComponent.baseInstance);
			});
		}
	}
	void GameLayer::OnDetach() {
		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: Destroy");
			mActiveScene->mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnDestroyFunc)
					scriptComponent.OnDestroyFunc(scriptComponent.baseInstance);
				scriptComponent.DestroyFunc();
			});
		}
		{
			EAGLE_PROFILE_SCOPE("Application - SceneEnd");
			mActiveScene->SceneEnd();
		}
	}
	void GameLayer::OnUpdate() {
		mActiveScene->OnUpdate();
		Input::ResetInputState();
	}
	void GameLayer::ResetApplication() {
		OnDetach();
		OnAttach();
	}
	void GameLayer::OnEvent(Event& event) {
		Input::OnEvent(event);
	}
	void GameLayer::OnImGuiRender() {

	}
}