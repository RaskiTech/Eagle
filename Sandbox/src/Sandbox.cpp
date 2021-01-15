#pragma once
#include <Eagle.h>
#include <EagleApplicationStart.h>
using namespace Egl;

class ExampleScene2 : public Scene {
	void SceneBegin() override {
		Entity mCamera = AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>().camera.SetBounds(7);
		SetPrimaryCamera(mCamera);

		// Camera controller
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
				//if (Input::IsKeyPressed(EGL_KEY_X)) SwitchScene(CreateRef<ExampleScene>());
			}
		};
		mCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		auto e = AddEntity("Entity");
		e.AddComponent<SpriteComponent>();
	}
	void SceneEnd() {

	}
};

class ExampleScene : public Scene {
	void SceneBegin() override {
		for (int i = 0; i < 8; i++) {
			Entity mPlayer = AddEntity("Cube");
			mPlayer.AddComponent<SpriteComponent>(glm::vec4(0.2f, 0.3f, 0.4f, 1.0f));
			mPlayer.GetComponent<TransformComponent>().position = { i * 1.5f - 5, 2, 0 };
			mPlayer.GetComponent<TransformComponent>().rotation = (float)i;
		}

		Entity mCamera = AddEntity("Camera");
		mCamera.AddComponent<CameraComponent>().camera.SetBounds(7);
		SetPrimaryCamera(mCamera);

		// Camera controller
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
				//if (Input::IsKeyPressed(EGL_KEY_Z)) AddEntity("Entity");
				//if (Input::IsKeyPressed(EGL_KEY_Z)) SwitchScene(CreateRef<ExampleScene2>());
			}
		};
		mCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		Entity particle = AddEntity("ParticleSystem");
		ParticleSystemProps props;
		props.minSize = { 0.2f, 0.2f };
		props.minSize = { 0.4f, 0.4f };
		props.maxColor = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);
		props.minColor = glm::vec4(0.2f, 0.8f, 0.3f, 1.0f);
		props.sizeAtEnd = { 0, 0 };
		particle.AddComponent<ParticleSystemComponent>(props);
	}
	void SceneEnd() {

	}
};


Ref<Scene> Egl::ApplicationStartup() {
	return CreateRef<ExampleScene>();
}