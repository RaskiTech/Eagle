#pragma once
#include <Eagle.h>
#include <EagleApplicationStartup.h>
using namespace Egl;

class ExampleScene : public Scene {
	void ParticleBegin() {
		{
			Entity particle = AddEntity("ParticleSystem");
			auto& comp = particle.AddComponent<ParticleSystemComponent>(130000);

			Ref<ParticleEmitter> emitter = CreateRef<ParticleEmitter>();

			emitter->mEmitsPerSecond = 10000; // The amount of particles spawned

			//// Generation
			Ref<RoundPosGen> generator = CreateRef<RoundPosGen>();
			generator->mCenter = { -11, -4 };
			generator->mRadius = { 2, 2 };

			Ref<BasicColorGen> colorGen = CreateRef<BasicColorGen>();
			colorGen->mMaxStartCol = { 1, 1, 0, 1 };
			colorGen->mMinStartCol = { 0, 1, 1, 1 };
			colorGen->mMaxEndCol = { 1, 0, 1, 1 };
			colorGen->mMinEndCol = { 1, 0, 0, 1 };
			Ref<BasicTimeGen> timeGen = CreateRef<BasicTimeGen>();
			timeGen->mMaxTime = 9;
			timeGen->mMinTime = 6;
			Ref<BasicVelGen> velGen = CreateRef<BasicVelGen>();
			velGen->mMinStartVel = { 1, 7 };
			velGen->mMaxStartVel = { 2, 7 };

			emitter->addGenerator(colorGen);
			emitter->addGenerator(velGen);
			emitter->addGenerator(timeGen);
			emitter->addGenerator(generator);
			comp.particleSystem.addEmitter(emitter);
			////

			//// Updation
			Ref<ParticleUpdaterEuler> eulerUpdater = CreateRef<ParticleUpdaterEuler>();
			eulerUpdater->mGlobalAcceleration = { 0, -2 };
			Ref<ParticleUpdaterBasicTime> timeUpdater = CreateRef<ParticleUpdaterBasicTime>();
			Ref<ParticleUpdaterBasicColor> colorUpdater = CreateRef<ParticleUpdaterBasicColor>();
			Ref<ParticleUpdaterAttractor> attractorUpdater = CreateRef< ParticleUpdaterAttractor>();
			attractorUpdater->AddAttractor({ -4, -2, 310 });

			comp.particleSystem.addUpdater(attractorUpdater);
			comp.particleSystem.addUpdater(timeUpdater);
			comp.particleSystem.addUpdater(colorUpdater);
			comp.particleSystem.addUpdater(eulerUpdater);
			////
		}
	}

	void SceneBegin() override {
		Entity mCamera = AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>().camera.SetBounds(30);
		mCamera.GetComponent<TransformComponent>().position = { 0, -8, 0 };
		SetPrimaryCamera(mCamera);

		auto& entity = AddEntity("Player");
		auto texture = Texture::Create("Assets/Player.png", false);
		entity.AddComponent<SpriteRendererComponent>(SubTexture::CreateFromIndexes(texture, { 0, 0 }, { 16, 16 }));
		entity.GetComponent<TransformComponent>().position = { -3, -1.5f, -0.1f };

		Entity mPlayer = AddEntity("Ground");
		mPlayer.AddComponent<SpriteRendererComponent>(glm::vec4(0.239f, 0.275f, 0.29f, 1.0f));
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