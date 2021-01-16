#include <EaglePCH.h>
#include "Scene.h"
#include "Components.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Entity.h"
#include "Eagle/Core/Time.h"

namespace Egl {
	Scene::Scene()
	{

	}

	Entity Scene::AddEntity(const std::string& name)
	{
		Entity entity = { mRegistry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::RemoveEntity(Entity& entity)
	{
		mRegistry.destroy((entt::entity)entity.GetID());
	}

	void Scene::SetPrimaryCamera(Entity& camera)
	{
		EAGLE_ENG_ASSERT(camera.HasComponent<CameraComponent>(), "Tried to set a primary camera, but the entity doesn't have a camera");
		EAGLE_ENG_ASSERT(camera.GetParentScene() == this, "Tried to set a primary camera, but the entity doesn't belong in this scene");
		mPrimaryCamera = (entt::entity)camera.GetID();
	}

	const Entity Scene::GetPrimaryCamera()
	{
		if (mPrimaryCamera != entt::null)
			return Entity(mPrimaryCamera, this);
		else
			return Entity();
	}

	void Scene::OnUpdate()
	{
		EAGLE_PROFILE_FUNCTION();
		// This function handles rendering the objects in this scene and updating components.

		{
			// TODO: Move this to somewhere in startup and also call the OnDestroy func
			mRegistry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				//EAGLE_ASSERT(!scriptComponent.baseInstance, "The instance is null");
				if (!scriptComponent.baseInstance) {
					scriptComponent.InstantiateFunc();
					scriptComponent.baseInstance->mEntity = Entity{ entity, this };
					if (scriptComponent.OnCreateFunc)
						scriptComponent.OnCreateFunc(scriptComponent.baseInstance);
				}
				if (scriptComponent.OnUpdateFunc)
					scriptComponent.OnUpdateFunc(scriptComponent.baseInstance);
			});
		}

		if (mPrimaryCamera != entt::null) {
			CameraComponent& camera = mRegistry.get<CameraComponent>(mPrimaryCamera);
			TransformComponent& transform = mRegistry.get<TransformComponent>(mPrimaryCamera);

			RenderCommand::SetColor(camera.backgroundColor);
			RenderCommand::Clear();

			Renderer::BeginScene(camera.camera, transform.GetTransform());


			{
				/////// ParticleSystem ///////
				auto group = mRegistry.group<ParticleSystemComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto [particleSystem, transform] = group.get<ParticleSystemComponent, TransformComponent>(entity);
					while (particleSystem.timeUntilEmit < 0) {
						particleSystem.particleSystem.Emit(transform.position);
						particleSystem.timeUntilEmit = particleSystem.timeBetweenEmits;
					}
					particleSystem.timeUntilEmit -= Time::GetFrameDelta();
					particleSystem.particleSystem.OnRender(transform.position.z);
				}
			}

			{
				/////// Sprite ///////
				auto group = mRegistry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto [spriteRenderer, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);
					if (spriteRenderer.texture == nullptr)
						Renderer::DrawColorQuad(transform.GetTransform(), spriteRenderer.color);
					else
						Renderer::DrawTextureQuad(transform.GetTransform(), spriteRenderer.texture, spriteRenderer.tilingFactor, spriteRenderer.color);
				}
			}

			//glm::mat4 transformMat = glm::translate(glm::mat4(1), { 0, 5, 0.9f })
			//	* glm::scale(glm::mat4(1), { 6, 1, 1 });
			//glm::vec2 texCoords[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
			//
			//auto group = mRegistry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			//auto entity = group[0];
			//auto [spriteRenderer, transformA] = group.get<SpriteRendererComponent, TransformComponent>(entity);
			//if (spriteRenderer.texture == nullptr)
			//	Renderer::DrawColorQuad(transformA.GetTransform() + glm::mat4(1), spriteRenderer.color);
			//else
			//	Renderer::DrawTextureQuad(transformA.GetTransform()+glm::mat4(1), spriteRenderer.texture, spriteRenderer.tilingFactor, spriteRenderer.color);
			//Renderer::DrawTextureQuad(transformMat, Texture::Create("Assets/Player.png", false), texCoords, 1, { 1, 1, 1, 1 });
			Renderer::EndScene();
		}
	}
	void Scene::SetViewportAspectRatio(float aspectRatio)
	{
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