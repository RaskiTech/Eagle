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
		return false;
	}
};

class ExampleScene : public Scene {
	void example_UI() {
		Entity canvas = AddCanvas();
		UIEntityParams topCornerParams(UITransform::XDriver::ConstLeft, 25, UITransform::WidthDriver::RelativeWidth, 0.15f,
			UITransform::YDriver::ConstTop, 25, UITransform::HeightDriver::RelativeHeight, 0.1f);
		UIEntityParams middleSquare(UITransform::LeftDriver::Constant, 25, UITransform::RightDriver::Constant, 25,
			UITransform::TopDriver::Constant, 15, UITransform::BottomDriver::Constant, 15);

		Entity exampleSquare = AddUIEntity("Top corner square", topCornerParams, canvas);
		exampleSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.3f, 0.3f, 0.3f, 1 });

		Entity exampleOtherSquare = AddUIEntity("middle square", middleSquare, exampleSquare);
		exampleOtherSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.3f, 0.2f, 1 });
		exampleOtherSquare.GetComponent<MetadataComponent>().subSorting = 1;

		auto& textComp = AddUIEntity("Text", canvas).AddComponent<TextComponent>();
		textComp.SetText("Example scene");
		textComp.data.fontSize = 10;
		textComp.data.alignVertical = TextAlignVertical::Middle;
		textComp.data.alignHorizontal = TextAlignHorizontal::Middle;
	}

	void SceneBegin() override {
		Entity camera = AddEntity("Camera");
		auto& cameraComp = camera.AddComponent<CameraComponent>();
		cameraComp.camera.SetSize(8.85f);
		cameraComp.backgroundColor = { 0.19f, 0.32f, 0.45f, 1.0f };
		camera.GetTransform().SetPosition(0.0f, -0.6f);
		SetPrimaryCamera(camera);
		
		auto& player = AddEntity("Player");
		auto texture = Texture::Create("Assets/Player.png", false);
		player.AddComponent<SpriteRendererComponent>(SubTexture::CreateFromIndexes(texture, { 0, 0 }, { 16, 16 }));
		player.GetTransform().SetPosition(-4.2f, -1.5f);
		
		Entity ground = AddEntity("Ground");
		ground.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.4f, 0.42f, 0.52f, 1 });
		ground.GetTransform().SetScale(13, 8).SetPosition(0, -6);
		
		Entity pedistal = AddEntity("Firehydrant stucture");
		pedistal.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.5f, 0.52f, 0.62f, 1 });
		pedistal.GetTransform().SetScale(2, 0.25f).SetPosition(1, -1.875f);

		Entity fireHydrant = AddEntity("Fire hydrant", pedistal);
		fireHydrant.AddComponent<SpriteRendererComponent>(Texture::Create("Assets/FireHydrant.png", false));
		fireHydrant.GetTransform().SetScale(1, 1.75f).SetLocalPosition(0, 4);
		
		example_UI();

		camera.AddComponent<NativeScriptComponent>().Bind<example_CameraController>();
	}
	void SceneEnd() {

	}
};

Ref<Scene> Egl::ApplicationStartup() {
	return CreateRef<ExampleScene>();
}