#include <EaglePCH.h>
#include "Scene.h"
#include "Components.h"
#include "ComponentsInternal.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Entity.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Core/Application.h"

namespace Egl {
	Scene::Scene() {

	}

	Entity Scene::AddEntity(const EntityParams& params) {
		entt::entity createdEntityID = mRegistry.create();
		Entity newEntity = { createdEntityID, this };
		newEntity.AddComponent<TransformComponent>(newEntity, params.position, params.rotation, params.scale);
		newEntity.AddComponent<MetadataComponent>(params.name, params.sortingLayer, params.subSorting);
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
	Entity Scene::AddEntity(const std::string& name) {
		return AddEntity(EntityParams(name));
	}
	Entity Scene::AddCanvas() { 
		Entity e = AddEntity(EntityParams("Canvas"));
		e.AddComponent<CanvasComponent>(); 
		return e; 
	}
	Entity Scene::AddUIEntity(const UIEntityParams& params, Entity canvasOrParent) {
		EAGLE_ENG_ASSERT(canvasOrParent.HasComponent<UIAlignComponent>() || canvasOrParent.HasComponent<CanvasComponent>(), "Parent isn't a canvas or an UI entity");

		entt::entity createdEntityID = mRegistry.create();
		Entity newEntity = { createdEntityID, this };
		newEntity.AddComponent<UIAlignComponent>(newEntity, params.xDrivers, params.yDrivers, params.xPrimaryValue, params.yPrimaryValue, 
			params.xSecondaryValue, params.ySecondaryValue, params.useSidesHorizontal, params.useSidesVertical);
		newEntity.AddComponent<MetadataComponent>(params.name, params.sortingLayer, params.subSorting);
		Relation& createdEntityRelation = newEntity.AddComponent<Relation>();
		newEntity.SetParent(canvasOrParent);
		return newEntity;
	}
	Entity Scene::AddUIEntity(const std::string& name, Entity canvasOrParent) {
		return AddUIEntity(UIEntityParams(name), canvasOrParent);
	}

	void Scene::RemoveEntity(Entity& entity) {
		mRegistry.destroy((entt::entity)entity.GetID());
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

		if (mPrimaryCamera != entt::null) {
			CameraComponent& camera = mRegistry.get<CameraComponent>(mPrimaryCamera);
			TransformComponent& camTrans = mRegistry.get<TransformComponent>(mPrimaryCamera);

			RenderCommand::SetColor(camera.backgroundColor);
			RenderCommand::Clear();

			Renderer::BeginScene(camera.camera, camTrans.GetTransform());
			
			static auto texture = Texture::Create("Assets/FireHydrant.png", false);

			/////// View Scaler ///////
			{
				auto group = mRegistry.group<CanvasComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto& transform = group.get<TransformComponent>(entity);

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
				auto group = mRegistry.group<ParticleSystemComponent>(entt::get<TransformComponent, MetadataComponent>);
				for (auto entity : group) {
					auto [particleSystem, transform, metadata] = group.get<ParticleSystemComponent, TransformComponent, MetadataComponent>(entity);
					float delta = Time::GetFrameDelta();
					particleSystem.particleSystem.Update(delta, transform);
					uint16_t sorting = ((uint16_t)metadata.sortingLayer << 8) + (uint16_t)metadata.subSorting;
					particleSystem.particleSystem.Render(sorting);
				}
			}

			/////// Sprite ///////
			{
				auto group = mRegistry.group<SpriteRendererComponent>(entt::get<TransformComponent, MetadataComponent>);
				for (auto entity : group) {
					auto [spriteRenderer, transform, metadata] = group.get<SpriteRendererComponent, TransformComponent, MetadataComponent>(entity);
					uint16_t sorting = ((uint16_t)metadata.sortingLayer << 8) + (uint16_t)metadata.subSorting;
					if (spriteRenderer.texture == nullptr)
						Renderer::DrawColorQuad(sorting, transform.GetTransform(), spriteRenderer.color);
					else
						Renderer::DrawTextureQuad(sorting, transform.GetTransform(), spriteRenderer.texture->GetTexture(), spriteRenderer.texture->GetTextureCoords(), spriteRenderer.tilingFactor, spriteRenderer.color);
				}
			}

			/////// UIAligment ///////
			{
				auto group = mRegistry.group<UIAlignComponent>(entt::get<SpriteRendererComponent, MetadataComponent>);
				for (auto entity : group) {
					auto [spriteRenderer, align, metadata] = group.get<SpriteRendererComponent, UIAlignComponent, MetadataComponent>(entity);
					uint16_t sorting = ((uint16_t)metadata.sortingLayer << 8) + (uint16_t)metadata.subSorting;
					if (spriteRenderer.texture == nullptr)
						Renderer::DrawColorQuad(sorting, align.GetTransform(), spriteRenderer.color);
					else
						Renderer::DrawTextureQuad(sorting, align.GetTransform(), spriteRenderer.texture->GetTexture(), spriteRenderer.texture->GetTextureCoords(), spriteRenderer.tilingFactor, spriteRenderer.color);
				}
			}

			Renderer::EndScene();
		}
	}

	glm::vec2 Scene::ScreenToWorldPos(const glm::vec2& pixelCoordinate) const {
		const glm::vec2& camPos = mRegistry.get<TransformComponent>(mPrimaryCamera).GetPosition();
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
		const glm::vec2& camPos = mRegistry.get<TransformComponent>(mPrimaryCamera).GetPosition();
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