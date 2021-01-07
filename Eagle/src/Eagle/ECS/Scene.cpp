#include <EaglePCH.h>
#include "Scene.h"
#include "Components.h"
#include "Eagle/Rendering/Renderer.h"
#include "Entity.h"

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
			Camera& camera = mRegistry.get<CameraComponent>(mPrimaryCamera).camera;
			TransformComponent& transform = mRegistry.get<TransformComponent>(mPrimaryCamera);

			Renderer::BeginScene(camera, transform.transform);

			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer::DrawColorQuad(transform, sprite.color);
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