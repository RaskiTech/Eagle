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

		// First don't put entities in order, since a lot of them are made there. Now sort them
		std::sort(mActiveScene->entitiesInSortOrder.begin(), mActiveScene->entitiesInSortOrder.end(), [&](entt::entity e1, entt::entity e2) {
			auto& mc1 = mActiveScene->mRegistry.get<MetadataComponent>(e1);
			auto& mc2 = mActiveScene->mRegistry.get<MetadataComponent>(e2);
			if (mc1.sortingLayer == mc2.sortingLayer)
				return mc1.subSorting > mc2.subSorting;
			else
				return mc1.sortingLayer > mc2.sortingLayer;
		});
		mActiveScene->areEntitiesInOrder = true;

		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: OnCreate");
			mActiveScene->mRegistry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& scriptComponent) {
				EAGLE_ENG_ASSERT(scriptComponent.baseInstance != nullptr, "Instance is a nullptr");

				#pragma warning( suppress: 6011 )
				scriptComponent.baseInstance->mEntity = Entity{ entity, mActiveScene.get() };

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
				//scriptComponent.DestroyFunc();
				scriptComponent.DeleteBase();
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