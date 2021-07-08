#include <EaglePCH.h>
#include <EagleApplicationStartup.h>
#include "Eagle/ECS/Components.h"
#include "GameLayer.h"
#include "Input.h"

// Handles calling input and client functions.

namespace Egl {
	GameLayer::GameLayer() {}

	void GameLayer::ActivateScene(Ref<Scene> scene) {
		mActiveScene = scene;
		{
			EAGLE_PROFILE_SCOPE("Client - SceneBegin");
			mActiveScene->SceneBegin();
		}
		{
			EAGLE_PROFILE_SCOPE("Client - Scripts: OnCreate");
			mActiveScene->mRegistry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& scriptComponent) {
				EAGLE_ENG_ASSERT(scriptComponent.baseInstance != nullptr, "Instance is a nullptr");

				scriptComponent.baseInstance->mEntity = Entity{ entity, mActiveScene.get() };

				// Add it to an event array
				if (scriptComponent.OnEventFunc)
					mActiveScene->eventScriptsInOrder.push_back(&scriptComponent);

				if (scriptComponent.OnCreateFunc)
					scriptComponent.OnCreateFunc(scriptComponent.baseInstance);
			});
		}

		std::sort(mActiveScene->eventScriptsInOrder.begin(), mActiveScene->eventScriptsInOrder.end(), [&](NativeScriptComponent* e1, NativeScriptComponent* e2) {
			auto& mc1 = mActiveScene->mRegistry.get<MetadataComponent>((entt::entity)e1->baseInstance->GetEntity().GetID());
			auto& mc2 = mActiveScene->mRegistry.get<MetadataComponent>((entt::entity)e2->baseInstance->GetEntity().GetID());
			if (mc1.sortingLayer == mc2.sortingLayer)
				return mc1.subSorting > mc2.subSorting;
			else
				return mc1.sortingLayer > mc2.sortingLayer;
		});
		mActiveScene->sceneInitComplete = true;
	}

	void GameLayer::OnAttach() {
		FramebufferDefenition defenition;
		defenition.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth };
		defenition.width = 1280;
		defenition.height = 720;
		mFramebuffer = Framebuffer::Create(defenition);

		{
			EAGLE_PROFILE_SCOPE("Client - ApplicationStartup");
			Ref<Scene> scene = ApplicationStartup();
			ActivateScene(scene);
		}
	}
	void GameLayer::OnDetach() {
		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: Destroy");
			mActiveScene->mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnDestroyFunc)
					scriptComponent.OnDestroyFunc(scriptComponent.baseInstance);
				scriptComponent.DeleteBase();
			});
		}
		{
			EAGLE_PROFILE_SCOPE("Application - SceneEnd");
			mActiveScene->SceneEnd();
		}
	}
	void GameLayer::OnUpdate() {
		FramebufferDefenition def = mFramebuffer->GetDefenition();
		const glm::vec2& sceneSize = Application::Get().GetSceneWindowSize();
		if (sceneSize.x > 0.0f && sceneSize.y > 0.0f && (sceneSize.x != def.width || sceneSize.y != def.height)) {
			mFramebuffer->Resize((uint32_t)sceneSize.x, (uint32_t)sceneSize.y);
			Application::Get().GetGameLayer()->GetActiveScene()->SetViewportAspectRatio(sceneSize.x / sceneSize.y);
		}
		
		mFramebuffer->Bind();

		//mFramebuffer->ClearAttachment();
		mActiveScene->OnUpdate();

		// Testing code
		//glm::ivec2 mouse = (glm::ivec2)Input::MousePos();
		//glm::ivec2 sceneSizeInt = (glm::ivec2)Application::Get().GetSceneWindowSize();
		//mouse.y = sceneSize.y - mouse.y;
		//if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < sceneSizeInt.x && mouse.y < sceneSizeInt.y) {
		//	uint32_t pixelData = mFramebuffer->ReadPixel(1, mouse.x, mouse.y);
		//	LOG("Pixel data = {0}", pixelData);
		//}

		Input::ResetInputState();
		mFramebuffer->Unbind();
	}

	void GameLayer::DistributeEvent(Event& e) {
		EAGLE_ENG_ASSERT(mActiveScene->sceneInitComplete, "The init isn't complete yet but there was an event");

		for (NativeScriptComponent* comp : mActiveScene->eventScriptsInOrder) {
			EAGLE_ENG_ASSERT(comp->OnEventFunc, "The script doesn't have an event function but it is in the list");
			if (comp->OnEventFunc(comp->baseInstance, e))
				break;
		}
	}
	template< typename T, typename Pred >
	static typename std::vector<T>::iterator Insert_sorted(std::vector<T>& vec, T const& item, Pred pred) {
		return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, pred), item);
	}
	void GameLayer::SubscribeToEvents(NativeScriptComponent* script) {
		// Add the script here if the scene instalation is complete. Else it will be added after sceneStart
		if (mActiveScene != nullptr && mActiveScene->sceneInitComplete) {
			Insert_sorted(mActiveScene->eventScriptsInOrder, script, [&](NativeScriptComponent* e1, NativeScriptComponent* e2) {
				auto mc1 = mActiveScene->mRegistry.get<MetadataComponent>((entt::entity)e1->baseInstance->GetEntity().GetID());
				auto mc2 = mActiveScene->mRegistry.get<MetadataComponent>((entt::entity)e2->baseInstance->GetEntity().GetID());
				if (mc1.sortingLayer == mc2.sortingLayer)
					return mc1.subSorting > mc2.subSorting;
				else
					return mc1.sortingLayer > mc2.sortingLayer;
			});
		}
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