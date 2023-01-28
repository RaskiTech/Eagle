#include <EaglePCH.h>
#include "Eagle/Core/Core.h"
#include "Scene.h"
#include "Eagle/Core/Application.h"
#include "Eagle/Debug/EditorLayer.h"
#include "Components.h"
#include "ComponentsInternal.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Entity.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Core/Application.h"

namespace Egl {
	Scene::Scene() {
		eventCallbacksSorted.reserve(50);
		entityDeleteQueue.reserve(50);
	}

	Entity Scene::AddEntity(std::string_view name, Entity parent, const EntityParams& params) {
		entt::entity createdEntityID = mRegistry.create();
		Entity newEntity = { createdEntityID, this };
		newEntity.AddComponent<Transform>(newEntity, params.position, params.rotation, params.scale);
		newEntity.AddComponent<MetadataComponent>(name, params.sortingLayer, params.subSorting);

		Relation& createdEntityRelation = newEntity.AddComponent<Relation>();
		createdEntityRelation.parent = (entt::entity)parent.GetID();
		Relation& parentRelation = parent.GetComponent<Relation>();
		if (parentRelation.firstChild != entt::null)
			mRegistry.get<Relation>(parentRelation.firstChild).previousSibling = createdEntityID;
		createdEntityRelation.nextSibling = parentRelation.firstChild;
		parentRelation.firstChild = createdEntityID;
		parentRelation.childCount++;
		return newEntity;
	}
	Entity Scene::AddEntity(std::string_view name, Entity parent) { return AddEntity(name, parent, EntityParams()); }
	Entity Scene::AddEntity(std::string_view name) { return AddEntity(name, EntityParams()); }

	Entity Scene::AddEntity(std::string_view name, const EntityParams& params) {
		entt::entity createdEntityID = mRegistry.create();
		Entity newEntity = { createdEntityID, this };
		newEntity.AddComponent<Transform>(newEntity, params.position, params.rotation, params.scale);
		newEntity.AddComponent<MetadataComponent>(name, params.sortingLayer, params.subSorting);
		Relation& createdEntityRelation = newEntity.AddComponent<Relation>();

		if (mFirstEntity == entt::null) {
			mFirstEntity = createdEntityID;
		}
		else {
			mRegistry.get<Relation>(mFirstEntity).previousSibling = createdEntityID;
			createdEntityRelation.nextSibling = mFirstEntity;
			mFirstEntity = createdEntityID;
		}
		return newEntity;
	}

	Entity Scene::AddCanvas() { 
		Entity e = AddEntity("Canvas");
		e.AddComponent<CanvasComponent>(); 
		return e; 
	}
	Entity Scene::AddUIEntity(std::string_view name, Entity canvasOrParent, const UIEntityParams& params) {
		EAGLE_ENG_ASSERT(canvasOrParent.HasComponent<UITransform>() || canvasOrParent.HasComponent<CanvasComponent>(), "Parent isn't a canvas or an UI entity");

		entt::entity createdEntityID = mRegistry.create();
		Entity newEntity = { createdEntityID, this };
		UITransform& align = newEntity.AddComponent<UITransform>(newEntity, params.xDrivers, params.yDrivers, params.xPrimaryValue, 
			params.yPrimaryValue, params.xSecondaryValue, params.ySecondaryValue, params.useSidesHorizontal, params.useSidesVertical);
		newEntity.AddComponent<MetadataComponent>(name, params.sortingLayer, params.subSorting);
		Relation& createdEntityRelation = newEntity.AddComponent<Relation>();
		newEntity.SetParent(canvasOrParent);
		return newEntity;
	}
	Entity Scene::AddUIEntity(std::string_view name, Entity canvasOrParent) {
		return AddUIEntity(name, canvasOrParent, UIEntityParams());
	}

	void Scene::DeleteEntity(Entity& entity) { 
		EAGLE_ENG_ASSERT((entt::entity)entity.GetID() != entt::null, "Tried to delete an entity that was null."); 
		entityDeleteQueue.push_back((entt::entity)entity.GetID()); 
	}
	void Scene::_DeleteEntityNow(entt::entity entity) {

		// Run OnDestroy
		// Remove component dependencies
		NativeScriptComponent* script = mRegistry.try_get<NativeScriptComponent>(entity);
		if (script) {
			if (script->OnDestroyFunc)
				script->OnDestroyFunc(script->baseInstance);
			script->DeleteScript();
		}
		
		// Destroy childs

		Relation& rel = mRegistry.get<Relation>(entity);
		entt::entity child = rel.firstChild;
		while (child != entt::null) {
			entt::entity next = mRegistry.get<Relation>(child).nextSibling;
			_DeleteEntityNow(child);
			child = next;
		}

		if (rel.previousSibling != entt::null)
			mRegistry.get<Relation>(rel.previousSibling).nextSibling = rel.nextSibling;
		if (rel.nextSibling != entt::null)
			mRegistry.get<Relation>(rel.nextSibling).previousSibling = rel.previousSibling;
		if (rel.parent != entt::null) {
			Relation& parent = mRegistry.get<Relation>(rel.parent);
			parent.childCount--;
			if (parent.firstChild == entity)
				parent.firstChild = rel.nextSibling;
		}
		else {
			if (mFirstEntity == entity) {
				mFirstEntity = mRegistry.get<Relation>(mFirstEntity).nextSibling;
			}
		}

		// Destroy self
		mRegistry.destroy(entity);
	}

	void Scene::SetPrimaryCamera(Entity& camera) {
		EAGLE_ENG_ASSERT(camera.HasComponent<CameraComponent>(), "Tried to set a primary camera, but the entity doesn't have a camera");
		EAGLE_ENG_ASSERT(camera.GetScene() == this, "Tried to set a primary camera, but the entity doesn't belong in this scene");
		mPrimaryCamera = (entt::entity)camera.GetID();
	}

	const Entity Scene::GetPrimaryCamera() {
		if (mPrimaryCamera != entt::null)
			return Entity(mPrimaryCamera, this);
		else
			return Entity();
	}


	// This function handles updating components and submitting everything to the renderer
	void Scene::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();

		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: OnUpdate");
			mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				EAGLE_ASSERT(scriptComponent.baseInstance != nullptr, "The instance is null");
				if (scriptComponent.OnUpdateFunc)
					scriptComponent.OnUpdateFunc(scriptComponent.baseInstance);
			});
		}

		if (mPrimaryCamera != entt::null) {
			CameraComponent& camera = mRegistry.get<CameraComponent>(mPrimaryCamera);
			Transform& camTrans = mRegistry.get<Transform>(mPrimaryCamera);

			RenderCommand::SetColor(camera.backgroundColor);
			RenderCommand::Clear();

			Renderer::BeginScene(camera.camera, camTrans.GetTransform());

			/////// View Scaler ///////
			{
				auto group = mRegistry.group<CanvasComponent>(entt::get<Transform>);
				for (auto entity : group) {
					auto& transform = group.get<Transform>(entity);

					const glm::vec2 size = { camera.camera.GetSize() * camera.camera.GetAspectRatio(), camera.camera.GetSize() };
					if (transform.GetScale() != size)
						transform.SetScale(size);
					if (camTrans.GetPosition() != transform.GetPosition())
						transform.SetPosition(camTrans.GetPosition());
				}
			}


			////////////////////////////////////////////////// Rendering //////////////////////////////////////////////////

			/////// ParticleSystem ///////
			{
				auto group = mRegistry.group<ParticleSystemComponent>(entt::get<Transform, MetadataComponent>);
				for (auto entity : group) {
					auto [particleSystem, transform, metadata] = group.get<ParticleSystemComponent, Transform, MetadataComponent>(entity);
					float delta = Time::GetFrameDelta();
					particleSystem.particleSystem.Update(delta, transform);
					uint16_t sorting = ((uint16_t)metadata.sortingLayer << 8) + (uint16_t)metadata.subSorting;
					particleSystem.particleSystem.Render(sorting);
				}
			}

			/////// Sprite ///////
			{
				auto group = mRegistry.group<SpriteRendererComponent>(entt::get<Transform, MetadataComponent>);
				for (auto entity : group) {
					auto [spriteRenderer, transform, metadata] = group.get<SpriteRendererComponent, Transform, MetadataComponent>(entity);
					uint16_t sorting = metadata.CalculateSorting();
					if (!spriteRenderer.texture)
						Renderer::DrawColorQuad(sorting, transform.GetTransform(), spriteRenderer.color);
					else
						Renderer::DrawTextureQuad(sorting, transform.GetTransform(), Assets::GetSubTexture(spriteRenderer.texture)->GetTexture(), Assets::GetSubTexture(spriteRenderer.texture)->GetTextureCoords(), spriteRenderer.tilingFactor, spriteRenderer.color);
				}
			}

			/////// UIAligment && Sprite ///////
			{
				auto group = mRegistry.group<UITransform>(entt::get<SpriteRendererComponent, MetadataComponent>);
				for (auto entity : group) {
					auto [spriteRenderer, align, metadata] = group.get<SpriteRendererComponent, UITransform, MetadataComponent>(entity);
					uint16_t sorting = metadata.CalculateSorting();
					if (!spriteRenderer.texture)
						Renderer::DrawColorQuad(sorting, align.GetTransform(), spriteRenderer.color);
					else
						Renderer::DrawTextureQuad(sorting, align.GetTransform(), Assets::GetSubTexture(spriteRenderer.texture)->GetTexture(), Assets::GetSubTexture(spriteRenderer.texture)->GetTextureCoords(), spriteRenderer.tilingFactor, spriteRenderer.color);
				}
			}

			/////// Text ///////
			{
				auto group = mRegistry.group<TextComponent>(entt::get<UITransform, MetadataComponent>);
				for (auto entity : group) {
					auto [textRenderer, align, metadata] = group.get<TextComponent, UITransform, MetadataComponent>(entity);
					uint16_t sorting = metadata.CalculateSorting();
					const glm::vec2& scale = align.GetWorldScale();
					textRenderer.textRenderer.RenderText(sorting, textRenderer.props, align.GetWorldPosition(), scale, camera.camera.GetSize());
				}
			}

	#if EAGLE_EDITOR
			Application::Get().GetEditorLayer()->DrawSelectedEntityOutline(camera);
	#endif
			Renderer::EndScene();
		}


		for (int i = 0; i < entityDeleteQueue.size(); i++) {
			// Was it a child of an already deleted entity? Did the delete on the entity get called twice?
			if (mRegistry.valid(entityDeleteQueue[i]))
				_DeleteEntityNow(entityDeleteQueue[i]);
		}

		// If the camera got deleted for example, set it to null
		ValidateSceneCameraExistence();
	}

	template< typename T, typename Pred >
	static typename std::vector<T>::iterator Insert_sorted(std::vector<T>& vec, T const& item, Pred pred) {
		return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, pred), item);
	}
	void Scene::SubscribeToEvents(NativeScriptComponent* script)
	{
		EAGLE_ENG_ASSERT(script->OnEventFunc, "Script doesn't have an event function.");
		SubscribeToEvents(script->baseInstance->GetEntity(), script->OnEventFunc);
	}

	void Scene::SubscribeToEvents(Entity entity, std::function<bool(Entity, Event&)> callback)
	{
		EAGLE_ENG_ASSERT(callback != nullptr, "Event callback was a nullptr.");

		// Add the script here if the scene instalation is complete. Else it will be added after sceneStart
		if (GetSceneState() >= SceneState::Running_3) {
			Insert_sorted(eventCallbacksSorted, EventCallbackData{ entity, callback }, [&](auto& e1, auto& e2) {
				auto mc1 = mRegistry.get<MetadataComponent>((entt::entity)entity.GetID());
				auto mc2 = mRegistry.get<MetadataComponent>((entt::entity)entity.GetID());
				if (mc1.sortingLayer == mc2.sortingLayer)
					return mc1.subSorting > mc2.subSorting;
				else
					return mc1.sortingLayer > mc2.sortingLayer;
			});
		}
		else
			eventCallbacksSorted.push_back(EventCallbackData{ entity, callback });
	}
	void Scene::SubscribeToEnterExitEvents(Entity instance, std::function<bool(Entity, Event&)> callback)
	{
		EAGLE_ENG_ASSERT(callback != nullptr, "Event callback was a nullptr.");
		// This could be changed later, but currently we always notify every function so sorting doesn't matter
		hoverCallbacks.push_back(HoverCallbackData{ instance, false, callback });
	}


	void Scene::OptOutOfEvents(NativeScriptComponent* script) {
		EAGLE_ENG_ASSERT(script->OnEventFunc, "Script doesn't have an event function.");

		int i = 0;
		while (true) {
			EAGLE_ENG_ASSERT(i < eventCallbacksSorted.size(), "Script event function wasn't in the eventScript list");
			if (eventCallbacksSorted[i].entity == script->baseInstance->GetEntity()) {
				eventCallbacksSorted.erase(eventCallbacksSorted.begin() + i);
				break;
			}
			i++;
		}
	}

	glm::vec2 Scene::ScreenToWorldPos(const glm::vec2& pixelCoordinate) const {
		EAGLE_ENG_ASSERT(mPrimaryCamera != entt::null, "ScreenToWorldPos was called, but we don't have a primary camera.")

		const glm::vec2& camPos = mRegistry.get<Transform>(mPrimaryCamera).GetPosition();
		auto& camCam = mRegistry.get<CameraComponent>(mPrimaryCamera);
		float screenSize = camCam.camera.GetSize();
		const glm::vec2& viewSize = Application::Get().GetSceneWindowSize();
		
		const glm::vec2 posInScreen01 = pixelCoordinate / glm::vec2{ viewSize.x, -viewSize.y };
		const glm::vec2 screenSizeInWorld = { screenSize * camCam.camera.GetAspectRatio(), screenSize };
		const glm::vec2 posInScreenWorld = posInScreen01 * screenSizeInWorld;
		const glm::vec2 topCornerInWorld = camPos + glm::vec2{ -screenSizeInWorld.x / 2, screenSizeInWorld.y / 2 };
		return posInScreenWorld + topCornerInWorld; 
	}
	glm::vec2 Scene::WorldToScreenPos(const glm::vec2& worldPos) const {
		EAGLE_ENG_ASSERT(mPrimaryCamera != entt::null, "WorldToScreenPos was called, but we don't have a primary camera.")

		const glm::vec2& camPos = mRegistry.get<Transform>(mPrimaryCamera).GetPosition();
		auto& camCam = mRegistry.get<CameraComponent>(mPrimaryCamera);
		float screenSize = camCam.camera.GetSize();
		const glm::vec2& viewSize = Application::Get().GetSceneWindowSize();
		const glm::vec2& sceneOffsetInPixel = Application::Get().GetSceneScreenOffset();

		const glm::vec2 screenSizeInWorld = { screenSize * camCam.camera.GetAspectRatio(), screenSize };
		const glm::vec2 topCornerInWorld = camPos + glm::vec2{ -screenSizeInWorld.x / 2, screenSizeInWorld.y / 2 };
		const glm::vec2 posInScreenWorld = worldPos - topCornerInWorld;
		const glm::vec2 posInScreen01 = posInScreenWorld / screenSizeInWorld;
		return { posInScreen01.x * viewSize.x, posInScreen01.y * -viewSize.y };
	}

	void Scene::SwitchToScene(SceneRef scene) const {
		Application::Get().GetGameLayer()->ScheduleSceneSwitch(scene);
	}

	void Scene::ChangeCameraAspectRatios(float aspectRatio) {
		// Resize cameras that don't have a fixed aspect ratio
		auto& view = mRegistry.view<CameraComponent>();
		for (auto entity : view) {
			CameraComponent& camera = view.get<CameraComponent>(entity);
			if (!camera.fixedAspectRatio) {
				camera.camera.SetAspectRatio(aspectRatio);
			}
		}
	}
}