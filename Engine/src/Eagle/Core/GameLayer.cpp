#include <EaglePCH.h>
#include <EagleApplicationStartup.h>
#include "Eagle/ECS/Components.h"
#include "Eagle/Debug/EditorLayer.h"
#include "GameLayer.h"
#include "Input.h"
#include "Eagle/Rendering/RenderCommand.h"

// Handles calling input and client functions.

namespace Egl {
	GameLayer::GameLayer() {
		FramebufferDefenition defenition;
		defenition.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		defenition.width = 1280;
		defenition.height = 720;
		mFramebuffer = Framebuffer::Create(defenition);
	}

	void GameLayer::ActivateScene(Ref<Scene> scene) {
		mActiveScene = scene;
		{
			EAGLE_PROFILE_SCOPE("Client - SceneBegin");
			mActiveScene->SceneBegin();
		}
		{
			EAGLE_PROFILE_SCOPE("Client - Scripts: OnCreate");
			auto view = mActiveScene->mRegistry.view<NativeScriptComponent>();
			view.each([&](auto entity, NativeScriptComponent& scriptComponent) {
				EAGLE_ENG_ASSERT(scriptComponent.baseInstance != nullptr, "Instance is a nullptr");

				scriptComponent.baseInstance->mEntity = Entity{ entity, mActiveScene.get() };

				// Add it to an event array
				if (scriptComponent.OnEventFunc)
					mActiveScene->eventScriptsInOrder.push_back(&scriptComponent);
			});
			view.each([&](auto entity, NativeScriptComponent& scriptComponent) {
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
	void GameLayer::OnUpdate(bool needToDrawToScreen) {
		FramebufferDefenition def = mFramebuffer->GetDefenition();
		const glm::vec2& sceneSize = Application::Get().GetSceneWindowSize();
		if (sceneSize.x > 0.0f && sceneSize.y > 0.0f && (sceneSize.x != def.width || sceneSize.y != def.height)) {
			mFramebuffer->Resize((uint32_t)sceneSize.x, (uint32_t)sceneSize.y);
			Application::Get().GetGameLayer()->GetActiveScene()->SetViewportAspectRatio(sceneSize.x / sceneSize.y);
		}
		
		mFramebuffer->Bind();

		mActiveScene->OnUpdate();

		Input::ResetInputState();

		// If running in the editor or not
		if (needToDrawToScreen) {
			const glm::vec2& size = Application::Get().GetSceneWindowSize();
			mFramebuffer->DrawToScreenAndUnbind((uint32_t)size.x, (uint32_t)size.y);
		}
		else {
			mFramebuffer->Unbind();
		}
	}

	#define IS_UNDER_MOUSE(mouseX, mouseY, objX, objY, objXRad, objYRad) (glm::abs(mouseX-objX) < objXRad && glm::abs(mouseY-objY) < objYRad)
	void GameLayer::DistributeEvent(Event& e) {
		EAGLE_ENG_ASSERT(mActiveScene->sceneInitComplete, "The init isn't complete yet but there was an event");
		glm::vec2 mousePos = mActiveScene->ScreenToWorldPos(Input::MousePos());
		std::vector<NativeScriptComponent*> listenersUnderMouse;

		// TODO: Use binary search to find the objects

		// Iterate through and check what listeners are under the mouse
		for (NativeScriptComponent* comp : mActiveScene->eventScriptsInOrder) {
			EAGLE_ENG_ASSERT(comp->OnEventFunc, "The script doesn't have an event function but it is in the event list");
			const Entity& entity = comp->baseInstance->GetEntity();
			if (entity.HasComponent<Transform>()) {
				auto& tComp = entity.GetComponent<Transform>();
				if (IS_UNDER_MOUSE(mousePos.x, mousePos.y, tComp.GetPosition().x, tComp.GetPosition().y, tComp.GetScale().x/2, tComp.GetScale().y/2))
					listenersUnderMouse.push_back(comp);
			}
			else {
				auto& tComp = entity.GetComponent<UITransform>();
				//LOG(glm::abs(mouseX - objX), objSizeX, glm::abs(mouseY-objY), objSizeY);
				if (IS_UNDER_MOUSE(mousePos.x, mousePos.y, tComp.GetWorldPosition().x, tComp.GetWorldPosition().y, tComp.GetWorldScale().x/2, tComp.GetWorldScale().y/2))
					listenersUnderMouse.push_back(comp);
			}
		}
		for (NativeScriptComponent* comp : listenersUnderMouse)
			if (comp->OnEventFunc(comp->baseInstance, e))
				break;
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