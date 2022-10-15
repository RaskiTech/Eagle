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
		_Framebuffer = Framebuffer::Create(defenition);
	}

	void GameLayer::ActivateScene(SceneRef sceneRef) {
		if (_ActiveScene != -1)
			DeactivateCurrentScene();

		_ActiveScene = sceneRef;
		Scene* scene = Assets::GetScene(_ActiveScene);
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

	void GameLayer::DeactivateCurrentScene() {
		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: Destroy");
			Assets::GetScene(_ActiveScene)->mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnDestroyFunc)
					scriptComponent.OnDestroyFunc(scriptComponent.baseInstance);
				scriptComponent.DeleteBase();
			});
		}
		{
			EAGLE_PROFILE_SCOPE("Application - SceneEnd");
			Assets::GetScene(_ActiveScene)->SceneEnd();
		}

		EAGLE_EDITOR_ONLY(Application::Get().GetEditorLayer()->OnSceneDelete());
		_ActiveScene = -1;
	}

	void GameLayer::OnAttach() {
		{
			EAGLE_PROFILE_SCOPE("Client - ApplicationStartup");
			SceneRef scene = ApplicationStartup();
			ActivateScene(scene);
		}
	}
	void GameLayer::OnDetach() {
		DeactivateCurrentScene();
	}
	void GameLayer::OnUpdate(bool needToDrawToScreen) {
		FramebufferDefenition def = _Framebuffer->GetDefenition();
		const glm::vec2& sceneSize = Application::Get().GetSceneWindowSize();
		if (sceneSize.x > 0.0f && sceneSize.y > 0.0f && (sceneSize.x != def.width || sceneSize.y != def.height)) {
			_Framebuffer->Resize((uint32_t)sceneSize.x, (uint32_t)sceneSize.y);
			Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene())->SetViewportAspectRatio(sceneSize.x / sceneSize.y);
		}
		
		_Framebuffer->Bind();

		if (_ScheduledSceneSwitch != -1) {

			ActivateScene(_ScheduledSceneSwitch);
			_ScheduledSceneSwitch = -1;
		}
		Assets::GetScene(_ActiveScene)->OnUpdate();

		Input::ResetInputState();

		// If running in the editor or not
		if (needToDrawToScreen) {
			const glm::vec2& size = Application::Get().GetSceneWindowSize();
			_Framebuffer->DrawToScreenAndUnbind((uint32_t)size.x, (uint32_t)size.y);
		}
		else {
			_Framebuffer->Unbind();
		}
	}

	#define IS_UNDER_CURSOR(mousePos, objPos, objScale) (glm::abs(mousePos.x-objPos.x) < objScale.x/2 && glm::abs(mousePos.y-objPos.y) < objScale.y/2)
	void GameLayer::DistributeEvent(Event& e) {
		Scene* scene = Assets::GetScene(_ActiveScene);
		EAGLE_ENG_ASSERT(scene->sceneInitComplete, "The init isn't complete yet but there was an event");
		glm::vec2 mousePos = scene->ScreenToWorldPos(Input::MousePos());
		std::vector<NativeScriptComponent*> listenersUnderMouse;
		bool isMouseEvent = e.GetGategoryFlags() & (int)Egl::EventGategory::Mouse;

		// TODO: Use binary search to find the objects

		// Iterate through and check what listeners are under the mouse
		for (NativeScriptComponent* comp : scene->eventScriptsInOrder) {
			EAGLE_ENG_ASSERT(comp->OnEventFunc, "The script doesn't have an event function but it is in the event list");
			const Entity& entity = comp->baseInstance->GetEntity();
			if (entity.HasComponent<Transform>()) {
				auto& tComp = entity.GetComponent<Transform>();
				if (!isMouseEvent || IS_UNDER_CURSOR(mousePos, tComp.GetPosition(), tComp.GetScale()))
					listenersUnderMouse.push_back(comp);
			}
			else {
				auto& tComp = entity.GetComponent<UITransform>();
				//LOG(glm::abs(mouseX - objX), objSizeX, glm::abs(mouseY-objY), objSizeY);
				if (!isMouseEvent || IS_UNDER_CURSOR(mousePos, tComp.GetWorldPosition(), tComp.GetWorldScale()))
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
		Scene* scene = Assets::GetScene(_ActiveScene);
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