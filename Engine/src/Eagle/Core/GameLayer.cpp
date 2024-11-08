#include <EaglePCH.h>
#include <EagleApplicationStartup.h>

#include "Eagle/Core/Application.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/Debug/EditorLayer.h"
#include "GameLayer.h"
#include "Input.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Core/AssetManager.h"

// Handles scene management

namespace Egl {
	GameLayer::GameLayer() {
		FramebufferDefenition defenition;
		defenition.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		defenition.width = 1280;
		defenition.height = 720;
		_framebuffer = Framebuffer::Create(defenition);
	}

	void GameLayer::ActivateScene(SceneRef sceneRef) {
		EAGLE_PROFILE_FUNCTION();

		if (_activeScene)
			DeactivateCurrentScene();

		_activeScene = sceneRef;
		Scene* scene = Assets::GetScene(_activeScene);
		{
			EAGLE_PROFILE_SCOPE("Client - SceneBegin");
			scene->SceneBegin();
		}
		scene->_sceneState = Scene::SceneState::SceneBeginCalled_1;

		{
			EAGLE_PROFILE_SCOPE("Client - Scripts: OnCreate");
			auto view = scene->mRegistry.view<NativeScriptComponent>();
			view.each([&](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnCreateFunc)
					scriptComponent.OnCreateFunc(scriptComponent.baseInstance);
			});
		}
		scene->_sceneState = Scene::SceneState::CreateCalled_2;

		std::sort(scene->eventCallbacksSorted.begin(), scene->eventCallbacksSorted.end(), [&](auto& e1, auto& e2) {
			auto& mc1 = scene->mRegistry.get<MetadataComponent>((entt::entity)e1.entity.GetID());
			auto& mc2 = scene->mRegistry.get<MetadataComponent>((entt::entity)e2.entity.GetID());
			if (mc1.sortingLayer == mc2.sortingLayer)
				return mc1.subSorting > mc2.subSorting;
			else
				return mc1.sortingLayer > mc2.sortingLayer;
		});
		scene->_sceneState = Scene::SceneState::Running_3;
	}

	void GameLayer::DeactivateCurrentScene() {
		EAGLE_PROFILE_FUNCTION();

		Scene* scene = Assets::GetScene(_activeScene);
		EAGLE_ENG_ASSERT(scene->_sceneState == Scene::SceneState::Running_3, "Tried to deactivate scene that wasn't in a running state.");

		scene->_sceneState = Scene::SceneState::StartedDestroying_4;

		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: Destroy");
			scene->mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnDestroyFunc)
					scriptComponent.OnDestroyFunc(scriptComponent.baseInstance);
				scriptComponent.DeleteScript();
			});
		}
		scene->_sceneState = Scene::SceneState::DestroyCalled_5;

		{
			EAGLE_PROFILE_SCOPE("Application - SceneEnd");
			scene->SceneEnd();
		}
		scene->_sceneState = Scene::SceneState::SceneEndCalled_6;

		EAGLE_EDITOR_ONLY(if (Application::Get().GetEditorLayer() != nullptr) Application::Get().GetEditorLayer()->OnSceneDelete());
		_activeScene = -1;
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
		FramebufferDefenition def = _framebuffer->GetDefenition();
		const glm::vec2& sceneSize = Application::Get().GetSceneWindowSize();
		if (sceneSize.x > 0.0f && sceneSize.y > 0.0f && (sceneSize.x != def.width || sceneSize.y != def.height)) {
			_framebuffer->Resize((uint32_t)sceneSize.x, (uint32_t)sceneSize.y);
			Assets::GetScene(GetActiveScene())->ChangeCameraAspectRatios(sceneSize.x / sceneSize.y);
		}
		
		_framebuffer->Bind();

		if (_scheduledSceneSwitch) {

			ActivateScene(_scheduledSceneSwitch);
			_scheduledSceneSwitch = -1;
		}
		Assets::GetScene(_activeScene)->OnUpdate();

		Input::ResetInputState();

		// If running in the editor or not
		if (needToDrawToScreen) {
			const glm::vec2& size = Application::Get().GetSceneWindowSize();
			_framebuffer->DrawToScreenAndUnbind((uint32_t)size.x, (uint32_t)size.y);
		}
		else {
			_framebuffer->Unbind();
		}
	}

	#define IS_UNDER_CURSOR(mousePos, objPos, objScale) (glm::abs(mousePos.x-objPos.x) < objScale.x/2 && glm::abs(mousePos.y-objPos.y) < objScale.y/2)
	void GameLayer::DistributeEvent(Event& e) {
		Scene* scene = Assets::GetScene(_activeScene);
		EAGLE_ENG_ASSERT(scene->GetSceneState() == Scene::SceneState::Running_3, "The scene isn't running but we had an event.");
		glm::vec2 mousePos = scene->GetPrimaryCamera().IsValid() ? scene->ScreenToWorldPos(Input::MousePos()) : glm::vec2{ 0, 0 };
		std::vector<Scene::EventCallbackData> listenersUnderMouse;
		bool isMouseEvent = e.GetGategoryFlags() & (int)Egl::EventGategory::Mouse;


		auto IsUnderCursor = [](const Entity& entity, const glm::vec2& mousePos) {
			if (entity.HasComponent<Transform>()) { auto& comp = entity.GetComponent<Transform>(); return IS_UNDER_CURSOR(mousePos, comp.GetPosition(), comp.GetScale()); }
			else                               { auto& comp = entity.GetComponent<UITransform>(); return IS_UNDER_CURSOR(mousePos, comp.GetWorldPosition(), comp.GetWorldScale()); }
		};

		// TODO: Use something like a space has grid to find the objects

		bool disruptSearch = scene->GetUIManager().PreEventDistribution(e);

		// Iterate through and check what listeners are under the mouse
		for (auto& eventCallback : scene->eventCallbacksSorted) {
			EAGLE_ENG_ASSERT(eventCallback.callback, "The script doesn't have an event function but it is in the event list");

			if (!isMouseEvent || IsUnderCursor(eventCallback.entity, mousePos))
				listenersUnderMouse.push_back(eventCallback);
		}
		for (auto& eventScript : listenersUnderMouse)
		{
			if (disruptSearch)
				break;

			disruptSearch = eventScript.callback(eventScript.entity, e);
		}

		// If the event is MouseMoved event we need to check for other events to distribute
		if (MouseMovedEvent* mouseMovedEvent = e.FilterAs<MouseMovedEvent>())
		{
			for (auto& hoverCallback : scene->hoverCallbacks)
			{
				if (IsUnderCursor(hoverCallback.entity, mousePos) != hoverCallback.hovering)
				{
					hoverCallback.hovering = !hoverCallback.hovering;
					if (hoverCallback.hovering)
						hoverCallback.callback(hoverCallback.entity, MouseHoverEnterEvent());
					else
						hoverCallback.callback(hoverCallback.entity, MouseHoverExitEvent());
				}
			}
		}
			
	}
	

	void GameLayer::ResetApplication() {
		OnDetach();
		LOG_ENG_INFO("Application reset.");
		OnAttach();
	}
	void GameLayer::OnEvent(Event& event) {
		Input::OnEvent(event);
		DistributeEvent(event);
	}
	void GameLayer::OnImGuiRender() {
		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: OnEditor");
			Scene* scene = Assets::GetScene(GetActiveScene());
			scene->mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				if (scriptComponent.OnImGuiFunc)
					scriptComponent.OnImGuiFunc(scriptComponent.baseInstance);
			});
		}
	}
}