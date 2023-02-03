#pragma once
#include <Eagle.h>
#include <EagleApplicationStartup.h>
#include <Eagle/Rendering/Text/TextRenderer.h>
using namespace Egl;

////
// This is an example script designed to show most
// common API functionalities.
////

class example_CameraController : public Script {
public:
	void OnUpdate() {
		auto& transform = GetComponent<Transform>();
		auto& camera = GetComponent<CameraComponent>().camera;
		float zoomSize = camera.GetSize();
		auto pos = transform.GetPosition();
		float speed = 5;

		if (Input::IsKeyPressed(EGL_KEY_A)) transform.SetPosition(pos.x -= speed * Time::GetFrameDelta() * zoomSize * 0.2f, pos.y);
		if (Input::IsKeyPressed(EGL_KEY_D)) transform.SetPosition(pos.x += speed * Time::GetFrameDelta() * zoomSize * 0.2f, pos.y);
		if (Input::IsKeyPressed(EGL_KEY_S)) transform.SetPosition(pos.x, pos.y -= speed * Time::GetFrameDelta() * zoomSize * 0.2f);
		if (Input::IsKeyPressed(EGL_KEY_W)) transform.SetPosition(pos.x, pos.y += speed * Time::GetFrameDelta() * zoomSize * 0.2f);
		int scroll = Input::MouseScrolledY();
		if (scroll < 0 || zoomSize > 1) camera.SetSize(zoomSize - scroll);

	}

	bool OnEvent(Event& e) {
		if (KeyPressedEvent* keyEvent = e.FilterAs<KeyPressedEvent>()) {
			LOG("Key pressed: ", keyEvent->GetKeyCode());
		}

		return false;
	}
};

class ExampleScene : public Scene {
	void example_UI() {
		Entity canvas = AddCanvas();
		UIEntityParams topCornerParams(UITransform::XDriver::Left, 25, UITransform::WidthDriver::Relative, 0.2f,
			UITransform::YDriver::Top, 25, UITransform::HeightDriver::Relative, 0.1f);
		UIEntityParams middleSquare(UITransform::LeftDriver::Constant, 25, UITransform::RightDriver::Constant, 25,
			UITransform::TopDriver::Constant, 15, UITransform::BottomDriver::Constant, 15);

		Entity exampleSquare = AddUIEntity("Top corner square", canvas, topCornerParams);
		exampleSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.3f, 0.3f, 0.3f, 1 });


		FontRef font = Assets::LoadFont("Assets/Fonts/Roboto/Roboto-Regular.ttf");

		glm::vec4 btnColor = { 0.2f, 0.8f, 0.2f, 1.0f };
		auto callback = []() { LOG("Click"); };
		Entity button = Premade::AddButton(exampleSquare, "Button", Button(btnColor, btnColor * 0.9f, btnColor * 0.6f, callback), UIEntityParams(0, 1))
			.SetText("This is a button").SetFont(font).SetFontSize(3);

		auto callbackSlider = [](float val) { LOG("Value:", val); };
		Entity slider = Premade::AddSlider(canvas, "Slider", Slider(btnColor, callbackSlider), 
			UIEntityParams(UITransform::XDriver::Center, 0.11811, UITransform::WidthDriver::Relative, 0.614, UITransform::YDriver::Top, 22.555, UITransform::HeightDriver::Relative, 0.076, 0, 1));

		auto& textComp = AddUIEntity("Text", canvas).AddComponent<TextComponent>(font);
		textComp.SetText("Example scene");
	}

	void SceneBegin() override {
		Entity camera = AddEntity("Camera");
		auto& cameraComp = camera.AddComponent<CameraComponent>();
		cameraComp.camera.SetSize(8.85f);
		cameraComp.backgroundColor = { 0.19f, 0.32f, 0.45f, 1.0f };
		camera.GetTransform().SetPosition(0.0f, -0.6f);
		SetPrimaryCamera(camera);
		camera.AttachScript<example_CameraController>();
		
		auto& player = AddEntity("Player");
		auto texture = Assets::LoadTexture("Assets/Player.png", false);
		player.AddComponent<SpriteRendererComponent>(Assets::LoadSubTexture(texture, { 16, 16 }, { 0, 0 }, { 1, 1 }));
		player.GetTransform().SetPosition(-4.2f, -1.5f);
		player.AddComponent<AudioSource>(Assets::LoadClip("Assets/Sound.wav"));

		Entity ground = AddEntity("Ground");
		ground.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.4f, 0.42f, 0.52f, 1 });
		ground.GetTransform().SetScale(13, 8).SetPosition(0, -6);
		
		Entity pedistal = AddEntity("Firehydrant stucture");
		pedistal.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.5f, 0.52f, 0.62f, 1 });
		pedistal.GetTransform().SetScale(2, 0.25f).SetPosition(1, -1.875f);

		Entity fireHydrant = AddEntity("Fire hydrant", pedistal);
		fireHydrant.AddComponent<SpriteRendererComponent>(Assets::LoadTexture("Assets/FireHydrant.png", false));
		fireHydrant.GetTransform().SetScale(1, 1.75f).SetLocalPosition(0, 4);
		
		example_UI();
	}


	void SceneEnd() {

	}
};

SceneRef Egl::ApplicationStartup() {
	return Assets::CreateScene<ExampleScene>();
}