#pragma once
#include <Eagle.h>
#include <EagleApplicationStartup.h>
using namespace Egl;

class ExampleScene : public Scene {
	Entity ParticleBegin() {
		Entity particle = AddEntity("ParticleSystem");
		auto& particleSystem = particle.AddComponent<ParticleSystemComponent>(100000).particleSystem; // <-- The max amount of particles at a time

		Ref<Particles::ParticleEmitter> emitter = CreateRef<Particles::ParticleEmitter>(4000.0f); // <-- The amount of particles spawned per second

		emitter->AddSetter(CreateRef<Particles::BoxPosSetter>(glm::vec2{ 0, 0 }, glm::vec2{ 0.15f, 0 }));
		auto colorSetter = CreateRef<Particles::ColorSetter>();
		colorSetter->maxStartColor = { 0, 0.75f, 1, 1 };
		colorSetter->minStartColor = { 0.75f, 0, 1, 1 };
		colorSetter->maxEndColor  =  { 1, 0, 1, 1 };
		colorSetter->minEndColor  =  { 0, 1, 1, 1 };
		emitter->AddSetter(colorSetter);
		emitter->AddSetter(CreateRef<Particles::SizeSetter>(glm::vec2{ 0.03f, 0.03f }, glm::vec2{ 0.03f, 0.03f }));
		emitter->AddSetter(CreateRef<Particles::VelocitySetter>(glm::vec2{ -0.5f, 2 }, glm::vec2{ 0.5f, 3 }));
		emitter->AddSetter(CreateRef<Particles::TimeSetter>(5.0f, 10.0f));

		particleSystem.AddEmitter(emitter);

		particleSystem.AddUpdater(CreateRef<Particles::EulerUpdater>(glm::vec2{ 0, -1 }));
		particleSystem.AddUpdater(CreateRef<Particles::ColorUpdater>());
		particleSystem.AddUpdater(CreateRef<Particles::PerformantFloorUpdater>( -2.0f, 0.198f ));
		particleSystem.AddUpdater(CreateRef<Particles::TimeUpdater>());

		return particle;
	}

	void SceneBegin() override {
		Entity camera = AddEntity("Camera");
		camera.AddComponent<CameraComponent>().camera.SetSize(8.85f);
		camera.GetComponent<CameraComponent>().backgroundColor = { 0.19f, 0.32f, 0.45f, 1 };
		camera.GetComponent<TransformComponent>().position = { 0, -0.6f, 0 };
		SetPrimaryCamera(camera);

		auto& player = AddEntity("Player");
		auto texture = Texture::Create("Assets/Player.png", false);
		player.AddComponent<SpriteRendererComponent>(SubTexture::CreateFromIndexes(texture, { 0, 0 }, { 16, 16 }));
		player.GetComponent<TransformComponent>().position = { -4.2f, -1.5f, -0.1f };

		Entity ground = AddEntity("Ground");
		ground.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.4f, 0.42f, 0.52f, 1 });
		ground.GetComponent<TransformComponent>().scale = { 13, 8 };
		ground.GetComponent<TransformComponent>().position = { 0, -6, 0 };

		Entity particleSystem = ParticleBegin();

		Entity fireHydrant = AddEntity("Fire hydrant");
		fireHydrant.AddComponent<SpriteRendererComponent>(Texture::Create("Assets/FireHydrant.png", false));
		fireHydrant.GetComponent<TransformComponent>().scale = { 1, 2 };
		fireHydrant.GetComponent<TransformComponent>().position = { 0, -1, 0 };
		fireHydrant.AddChild(particleSystem);

		// Camera controller
		class CameraController : public Script {
		public:
			void OnUpdate() {
				auto& transform = GetComponent<TransformComponent>();
				auto& camera = GetComponent<CameraComponent>().camera;
				float zoomSize = camera.GetCameraSize();
				float speed = 5;

				if (Input::IsKeyPressed(EGL_KEY_A)) transform.position.x -= speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_D)) transform.position.x += speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_S)) transform.position.y -= speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_W)) transform.position.y += speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				int scroll = Input::MouseScrolledY();
				if (scroll < 0 || zoomSize > 1) camera.SetSize(zoomSize+(-scroll * Time::GetFrameDelta() * 100));
			}
		};
		camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}
	void SceneEnd() {

	}
};


Ref<Scene> Egl::ApplicationStartup() {
	return CreateRef<ExampleScene>();
}