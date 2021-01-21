#pragma once
#include <Eagle.h>
#include <EagleApplicationStartup.h>
using namespace Egl;

class ExampleScene : public Scene {
	void ParticleBegin() {
		Entity particle = AddEntity("ParticleSystem");
		auto& comp = particle.AddComponent<ParticleSystemComponent>(50000); // <-- The max amount of particles at one time

		Ref<Particles::ParticleEmitter> emitter = CreateRef<Particles::ParticleEmitter>(1000); // <-- The amount of particles spawned per second

		//// Setters
		Ref<Particles::BoxPosSetter> posSetter = CreateRef<Particles::BoxPosSetter>();
		posSetter->position = { 0, -4 };
		posSetter->maxOffset = { 14, 9 };

		Ref<Particles::ColorSetter> colorSetter = CreateRef<Particles::ColorSetter>();
		colorSetter->maxStartColor = { 0, 1, 1, 1 };
		colorSetter->minStartColor = { 1, 1, 0, 1 };
		colorSetter->maxEndColor = { 1, 0, 0, 1 };
		colorSetter->minEndColor = { 1, 0, 1, 1 };
		Ref<Particles::TimeSetter> timeSetter = CreateRef<Particles::TimeSetter>();
		timeSetter->maxTime = 9;
		timeSetter->minTime = 6;
		Ref<Particles::VelocitySetter> velSetter = CreateRef<Particles::VelocitySetter>();
		velSetter->minVelocity = { -2, 2 };
		velSetter->maxVelocity = { 2, 7 };
		Ref<Particles::SizeSetter> sizeSetter = CreateRef<Particles::SizeSetter>();
		sizeSetter->minSize = { 0.01f, 0.01f };
		sizeSetter->maxSize = { 0.01f, 0.01f };

		emitter->AddSetter(colorSetter);
		emitter->AddSetter(timeSetter);
		emitter->AddSetter(sizeSetter);
		emitter->AddSetter(velSetter);
		emitter->AddSetter(posSetter);
		comp.particleSystem.AddEmitter(emitter);
		////

		//// Updation
		comp.particleSystem.AddUpdater(CreateRef<Particles::TimeUpdater>());
		comp.particleSystem.AddUpdater(CreateRef<Particles::ColorUpdater>());
		comp.particleSystem.AddUpdater(CreateRef<Particles::EulerUpdater>(glm::vec2{ 0, -1 }));
		comp.particleSystem.AddUpdater(CreateRef<Particles::EndSizeUpdater>(glm::vec2{ 0, 0 }));
		////
	}

	void SceneBegin() override {
		Entity mCamera = AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>().camera.SetBounds(8.85f);
		mCamera.GetComponent<TransformComponent>().position = { 0, -0.6f, 0 };
		SetPrimaryCamera(mCamera);

		auto& entity = AddEntity("Player");
		auto texture = Texture::Create("Assets/Player.png", false);
		entity.AddComponent<SpriteRendererComponent>(SubTexture::CreateFromIndexes(texture, { 0, 0 }, { 16, 16 }));
		entity.GetComponent<TransformComponent>().position = { -4.2f, -1.5f, -0.1f };

		Entity mPlayer = AddEntity("Ground");
		mPlayer.AddComponent<SpriteRendererComponent>(glm::vec4{ 102.0f/255, 104.0f/255, 133.0f/255, 1.0f });
		mPlayer.GetComponent<TransformComponent>().scale = { 13, 8 };
		mPlayer.GetComponent<TransformComponent>().position = { 0, -6, 0 };

		ParticleBegin();

		// Camera controller
		//*
		class CameraController : public Script {
		public:
			void OnUpdate() {
				auto& transform = GetComponent<TransformComponent>();
				float zoomSize = GetComponent<CameraComponent>().camera.GetCameraSize();
				float speed = 5;

				if (Input::IsKeyPressed(EGL_KEY_A)) transform.position.x -= speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_D)) transform.position.x += speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_S)) transform.position.y -= speed * Time::GetFrameDelta() * zoomSize * 0.2f;
				if (Input::IsKeyPressed(EGL_KEY_W)) transform.position.y += speed * Time::GetFrameDelta() * zoomSize * 0.2f;
			}
		};
		mCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		//*/
	}
	void SceneEnd() {

	}
};


Ref<Scene> Egl::ApplicationStartup() {
	return CreateRef<ExampleScene>();
}