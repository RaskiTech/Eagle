#pragma once
#include <Eagle.h>
#include <EagleApplicationStartup.h>
using namespace Egl;

class ExampleScene : public Scene {
	void SceneBegin() override {
		Entity mCamera = AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>().camera.SetBounds(7);
		SetPrimaryCamera(mCamera);

		auto& entity = AddEntity("Player");
		auto texture = Texture::Create("Assets/Player.png", false);
		entity.AddComponent<SpriteRendererComponent>(SubTexture::CreateFromIndexes(texture, { 0, 0 }, { 16, 16 }));
		entity.GetComponent<TransformComponent>().position = { -3, -1.5f, 0.1f };

		Entity mPlayer = AddEntity("Ground");
		mPlayer.AddComponent<SpriteRendererComponent>(glm::vec4(0.239f, 0.275f, 0.29f, 1.0f));
		mPlayer.GetComponent<TransformComponent>().scale = { 13, 8 };
		mPlayer.GetComponent<TransformComponent>().position = { 0, -6, 0 };
	
		//Entity particleOld = AddEntity("ParticleSystemOld");
		//ParticleSystemProps props;
		//props.minSize = { 0.2f, 0.2f };
		//props.minSize = { 0.4f, 0.4f };
		//props.maxColor = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);
		//props.minColor = glm::vec4(0.2f, 0.8f, 0.3f, 1.0f);
		//props.sizeAtEnd = { 0, 0 };
		//particleOld.AddComponent<ParticleSystemOldComponent>(props);
		//particleOld.GetComponent<TransformComponent>().position.y = -2.5f;

		// TODO: New particles don't reset memory when spawned. If the particle isn't new, it will use old particles memory and since time is 1 die
		// Then can also remove the memory setting in generate().

		Entity particle = AddEntity("ParticleSystem");
		auto& comp = particle.AddComponent<ParticleSystemComponent>(10000);


		Ref<ParticleUpdaterEuler> eulerUpdater = CreateRef<ParticleUpdaterEuler>();
		eulerUpdater->mGlobalAcceleration = { 0.01f, 0.01f };
		Ref<ParticleUpdaterBasicTime> timeUpdater = CreateRef<ParticleUpdaterBasicTime>();
		Ref<ParticleUpdaterBasicColor> colorUpdater = CreateRef<ParticleUpdaterBasicColor>();


		comp.particleSystem.addUpdater(timeUpdater);
		comp.particleSystem.addUpdater(colorUpdater);
		comp.particleSystem.addUpdater(eulerUpdater);


		Ref<ParticleEmitter> emitter = CreateRef<ParticleEmitter>();
		emitter->mEmitsPerSecond = 2;

		Ref<ParticleBoxGenerator> generator = CreateRef<ParticleBoxGenerator>();
		generator->mMaxStartPosOffset = { 1, 1 };

		Ref<BasicColorGen> colorGen = CreateRef<BasicColorGen>();
		Ref<BasicTimeGen> timeGen = CreateRef<BasicTimeGen>();
		Ref<BasicVelGen> velGen = CreateRef<BasicVelGen>();

		emitter->addGenerator(colorGen);
		emitter->addGenerator(velGen);
		emitter->addGenerator(timeGen);
		emitter->addGenerator(generator);
		comp.particleSystem.addEmitter(emitter);
		// Camera controller
		/*
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
		*/
	}
	void SceneEnd() {

	}
};


Ref<Scene> Egl::ApplicationStartup() {
	return CreateRef<ExampleScene>();
}