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

			Renderer::BeginScene(camera.camera, transform.transform);

			{
				/////// Sprite ///////
				auto group = mRegistry.group<SpriteComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto [sprite, transform] = group.get<SpriteComponent, TransformComponent>(entity);
					Renderer::DrawColorQuad(transform, sprite.color);
				}
			}

			{
				/////// ParticleSystem ///////
				auto group = mRegistry.group<ParticleSystemComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto [particleSystem, transform] = group.get<ParticleSystemComponent, TransformComponent>(entity);
					while (particleSystem.timeUntilEmit < 0) {
						particleSystem.particleSystem.Emit({ transform.transform[3][0], transform.transform[3][1] });
						particleSystem.timeUntilEmit = particleSystem.timeBetweenEmits;
					}
					particleSystem.timeUntilEmit -= Time::GetFrameDelta();
					particleSystem.particleSystem.OnRender();
				}
			}

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