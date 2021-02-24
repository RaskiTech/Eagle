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
	template< typename T, typename Pred >
	typename std::vector<T>::iterator Insert_sorted(std::vector<T>& vec, T const& item, Pred pred) {
		return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, pred), item);
	}

	Scene::Scene() {

	}

	Entity Scene::AddEntity(const std::string& name) {
		entt::entity createdEntityID = mRegistry.create();
		Entity entity = { createdEntityID, this };
		entity.AddComponent<TransformComponent>(entity, glm::vec3{0, 0, 0});
		entity.AddComponent<MetadataComponent>(name, 0);
		Relation& createdEntityRelation = entity.AddComponent<Relation>();

		if (mFirstEntity == entt::null) {
			mFirstEntity = createdEntityID;
		}
		else {
			mRegistry.get<Relation>(mFirstEntity).previousSibling = createdEntityID;
			createdEntityRelation.nextSibling = mFirstEntity;
			mFirstEntity = createdEntityID;
		}
		if (areEntitiesInOrder)
			Insert_sorted(entitiesInSortOrder, createdEntityID, [&](entt::entity e1, entt::entity e2) {
				auto mc1 = mRegistry.get<MetadataComponent>(e1);
				auto mc2 = mRegistry.get<MetadataComponent>(e2);
				if (mc1.sortingLayer == mc2.sortingLayer)
					return mc1.subSorting > mc2.subSorting;
				else
					return mc1.sortingLayer > mc2.sortingLayer;
			});
		else
			entitiesInSortOrder.push_back(createdEntityID);
		return entity;
	}

	Entity Scene::AddUIEntity(const std::string& name, Entity UIParent) {
		EAGLE_ENG_ASSERT(UIParent.HasComponent<UIAlignComponent>() || UIParent.HasComponent<CanvasComponent>(), "parent isn't a canvas or an UI entity");

		entt::entity createdEntityID = mRegistry.create();
		Entity entity = { createdEntityID, this };
		entity.AddComponent<UIAlignComponent>(entity);
		entity.AddComponent<MetadataComponent>(name, 100);
		Relation& createdEntityRelation = entity.AddComponent<Relation>();
		entity.SetParent(UIParent);

		if (areEntitiesInOrder)
			Insert_sorted(entitiesInSortOrder, createdEntityID, [&](entt::entity e1, entt::entity e2) {
				auto mc1 = mRegistry.get<MetadataComponent>(e1);
				auto mc2 = mRegistry.get<MetadataComponent>(e2);
				if (mc1.sortingLayer == mc2.sortingLayer)
					return mc1.subSorting > mc2.subSorting;
				else
					return mc1.sortingLayer > mc2.sortingLayer;
			});
		else
			entitiesInSortOrder.push_back(createdEntityID);
		return entity;
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
		// This function handles rendering the objects in this scene and updating components.

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
					auto [viewScaler, transform] = group.get<CanvasComponent, TransformComponent>(entity);

					const glm::vec2 size = { camera.camera.GetSize() * camera.camera.GetAspectRatio(), camera.camera.GetSize() };
					transform.SetScale(size);
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
					//LOG("{0} ended up with sorting layer {1}", metadata.tag, sorting);
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
					//LOG("{0} ended up with sorting layer {1}", metadata.tag, sorting);
					if (spriteRenderer.texture == nullptr)
						Renderer::DrawColorQuad(sorting, align.GetTransform(), spriteRenderer.color);
					else
						Renderer::DrawTextureQuad(sorting, align.GetTransform(), spriteRenderer.texture->GetTexture(), spriteRenderer.texture->GetTextureCoords(), spriteRenderer.tilingFactor, spriteRenderer.color);
					// TODO: Set the need only if parent has changed scale. Remember canvas
					align.SetNeedToCalculateDimensions();
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
		
		// More readale version
		// const glm::vec2 posInScreen01 = { (pixelCoordinate.x - sceneOffsetInPixel.x) / viewSize.x, -(pixelCoordinate.y - sceneOffsetInPixel.y) / viewSize.y };
		// const glm::vec2 screenSizeInWorld = { screenSize * camCam.camera.GetAspectRatio(), screenSize };
		// const glm::vec2 posInScreenWorld = posInScreen01 * screenSizeInWorld;
		// const glm::vec2 topCornerInWorld = camPos + glm::vec2{ -screenSizeInWorld.x / 2, screenSizeInWorld.y / 2 };
		// return posInScreenWorld + topCornerInWorld;

		float screenSizeInWorld_X = screenSize * camCam.camera.GetAspectRatio();
		float posX = pixelCoordinate.x / viewSize.x * screenSizeInWorld_X + camPos.x - screenSizeInWorld_X / 2;
		float posY = -pixelCoordinate.y / viewSize.y * screenSize + camPos.y + screenSize / 2;
		return { posX, posY };
	}
	glm::vec2 Scene::WorldToScreenPos(const glm::vec2& worldPos) const {
		const glm::vec2& camPos = mRegistry.get<TransformComponent>(mPrimaryCamera).GetPosition();
		auto& camCam = mRegistry.get<CameraComponent>(mPrimaryCamera);
		float screenSize = camCam.camera.GetSize();
		const glm::vec2& viewSize = Application::Get().GetSceneWindowSize();
		const glm::vec2& sceneOffsetInPixel = Application::Get().GetSceneScreenOffset();

		// More readale version
		const glm::vec2 screenSizeInWorld = { screenSize * camCam.camera.GetAspectRatio(), screenSize };
		const glm::vec2 topCornerInWorld = camPos + glm::vec2{ -screenSizeInWorld.x / 2, screenSizeInWorld.y / 2 };
		const glm::vec2 posInScreen01 = { (worldPos.x - topCornerInWorld.x) / screenSizeInWorld.x, (worldPos.y - topCornerInWorld.y) / screenSizeInWorld.y };
		return { posInScreen01.x * viewSize.x, -posInScreen01.y * viewSize.y };

		float screenSizeInWorld_X = screenSize * camCam.camera.GetAspectRatio();
		float posX = viewSize.x * ((worldPos.x - (camPos.x + -screenSizeInWorld_X / 2)) / screenSizeInWorld_X);
		float posY = -((worldPos.y - (camPos.y + screenSize / 2)) / screenSize) * viewSize.x;
		return { posX, posY };
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