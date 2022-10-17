#include <EaglePCH.h>
#include <EagleBuildSettings.h>
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
	Scene::Scene() {}

	Entity Scene::AddEntity(const std::string& name, const EntityParams& params, Entity parent) {
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
	Entity Scene::AddEntity(const std::string& name, Entity parent) { return AddEntity(name, EntityParams(), parent); }
	Entity Scene::AddEntity(const std::string& name) { return AddEntity(name, EntityParams()); }

	Entity Scene::AddEntity(const std::string& name, const EntityParams& params) {
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
	Entity Scene::AddUIEntity(const std::string& name, const UIEntityParams& params, Entity canvasOrParent) {
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
	Entity Scene::AddUIEntity(const std::string& name, Entity canvasOrParent) {
		return AddUIEntity(name, UIEntityParams(), canvasOrParent);
	}


	void Scene::DeleteEntity(Entity& entity) { 
		// Store the entities to destroy and destroy them later after all the scripts update?
		_DeleteEntityNow((entt::entity)entity.GetID()); 
	}
	void Scene::_DeleteEntityNow(entt::entity entity) {

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

		if (NativeScriptComponent* script = mRegistry.try_get<NativeScriptComponent>(entity)) {
			Application::Get().GetGameLayer()->OptOutOfEvents(script);
		}

		mRegistry.destroy(entity);
	}

	void Scene::SetPrimaryCamera(Entity& camera) {
		EAGLE_ENG_ASSERT(camera.HasComponent<CameraComponent>(), "Tried to set a primary camera, but the entity doesn't have a camera");
		EAGLE_ENG_ASSERT(camera.GetParentScene() == this, "Tried to set a primary camera, but the entity doesn't belong in this scene");
		mPrimaryCamera = (entt::entity)camera.GetID();
	}

	const Entity Scene::GetPrimaryCamera() {
		if (mPrimaryCamera != entt::null)
			return Entity(mPrimaryCamera, this);
		else
			return Entity();
	}


	void Scene::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();
		// This function handles updating components and submitting everything to the renderer

		{
			EAGLE_PROFILE_SCOPE("Application - Scripts: OnUpdate");
			mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				EAGLE_ASSERT(scriptComponent.baseInstance != nullptr, "The instance is null");
				if (scriptComponent.OnUpdateFunc)
					scriptComponent.OnUpdateFunc(scriptComponent.baseInstance);
			});
		}

		if (!mRegistry.valid(mPrimaryCamera))
			mPrimaryCamera = entt::null;

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
					if (spriteRenderer.texture == -1)
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
					if (spriteRenderer.texture == -1)
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
					textRenderer.renderer.RenderText(sorting, textRenderer.data, align.GetWorldPosition(), scale, camera.camera.GetSize());
				}
			}

	#if EAGLE_EDITOR
			Application::Get().GetEditorLayer()->DrawSelectedEntityOutline(camera);
	#endif
			Renderer::EndScene();
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

	void Scene::SetViewportAspectRatio(float aspectRatio) {
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