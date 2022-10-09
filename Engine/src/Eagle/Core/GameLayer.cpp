#include <EaglePCH.h>
#include <EagleApplicationStartup.h>
#include "Eagle/ECS/Components.h"
#include "Eagle/Debug/EditorLayer.h"
#include "GameLayer.h"
#include "Input.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Core/AssetManager.h"

// Handles calling input and client functions.

namespace Egl {
	GameLayer::GameLayer() {
		FramebufferDefenition defenition;
		defenition.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		defenition.width = 1280;
		defenition.height = 720;
		mFramebuffer = Framebuffer::Create(defenition);
	}

	void GameLayer::ActivateScene(SceneRef sceneRef) {
		mActiveScene = sceneRef;
		Scene* scene = Assets::GetScene(mActiveScene);
		{
			EAGLE_PROFILE_SCOPE("Client - SceneBegin");
			scene->SceneBegin();
		}
		{
			EAGLE_PROFILE_SCOPE("Client - Scripts: OnCreate");
			auto view = scene->mRegistry.view<NativeScriptComponent>();
			view.each([&](auto entity, NativeScriptComponent& scriptComponent) {
				EAGLE_ENG_ASSERT(scriptComponent.baseInstance != nullptr, "Instance is a nullptr");

				scriptComponent.baseInstance->mEntity = Entity{ entity, scene };

				// Add it to an event array
				if (scriptComponent.OnEventFunc)
					scene->eventScriptsInOrder.push_back(&scriptComponent);
			});
			view.each([&](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnCreateFunc)
					scriptComponent.OnCreateFunc(scriptComponent.baseInstance);
			});
		}

		std::sort(scene->eventScriptsInOrder.begin(), scene->eventScriptsInOrder.end(), [&](NativeScriptComponent* e1, NativeScriptComponent* e2) {
			auto& mc1 = scene->mRegistry.get<MetadataComponent>((entt::entity)e1->baseInstance->GetEntity().GetID());
			auto& mc2 = scene->mRegistry.get<MetadataComponent>((entt::entity)e2->baseInstance->GetEntity().GetID());
			if (mc1.sortingLayer == mc2.sortingLayer)
				return mc1.subSorting > mc2.subSorting;
			else
				return mc1.sortingLayer > mc2.sortingLayer;
		});
		scene->sceneInitComplete = true;
	}

	void GameLayer::OnAttach() {
		{
			EAGLE_PROFILE_SCOPE("Client - ApplicationStartup");
			SceneRef scene = ApplicationStartup();
			ActivateScene(scene);
		}
	}
	void GameLayer::OnDetach() {
		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: Destroy");
			Assets::GetScene(mActiveScene)->mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnDestroyFunc)
					scriptComponent.OnDestroyFunc(scriptComponent.baseInstance);
				scriptComponent.DeleteBase();
			});
		}
		{
			EAGLE_PROFILE_SCOPE("Application - SceneEnd");
			Assets::GetScene(mActiveScene)->SceneEnd();
		}

		mActiveScene = -1;
	}
	void GameLayer::OnUpdate(bool needToDrawToScreen) {
		FramebufferDefenition def = mFramebuffer->GetDefenition();
		const glm::vec2& sceneSize = Application::Get().GetSceneWindowSize();
		if (sceneSize.x > 0.0f && sceneSize.y > 0.0f && (sceneSize.x != def.width || sceneSize.y != def.height)) {
			mFramebuffer->Resize((uint32_t)sceneSize.x, (uint32_t)sceneSize.y);
			Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene())->SetViewportAspectRatio(sceneSize.x / sceneSize.y);
		}
		
		mFramebuffer->Bind();

		Assets::GetScene(mActiveScene)->OnUpdate();

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
		Scene* scene = Assets::GetScene(mActiveScene);
		EAGLE_ENG_ASSERT(scene->sceneInitComplete, "The init isn't complete yet but there was an event");
		glm::vec2 mousePos = scene->ScreenToWorldPos(Input::MousePos());
		std::vector<NativeScriptComponent*> listenersUnderMouse;

		// TODO: Use binary search to find the objects

		// Iterate through and check what listeners are under the mouse
		for (NativeScriptComponent* comp : scene->eventScriptsInOrder) {
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
		Scene* scene = Assets::GetScene(mActiveScene);
		// Add the script here if the scene instalation is complete. Else it will be added after sceneStart
		if (scene != nullptr && scene->sceneInitComplete) {
			Insert_sorted(scene->eventScriptsInOrder, script, [&](NativeScriptComponent* e1, NativeScriptComponent* e2) {
				auto mc1 = scene->mRegistry.get<MetadataComponent>((entt::entity)e1->baseInstance->GetEntity().GetID());
				auto mc2 = scene->mRegistry.get<MetadataComponent>((entt::entity)e2->baseInstance->GetEntity().GetID());
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