#pragma once
#include <Eagle.h>
#include <EagleApplicationStartup.h>

#include <Eagle/Rendering/Text/TextRenderer.h>
using namespace Egl;

////
// This is an example script designed to show the API. Everything exept the
// SceneBegin() and SceneEnd() functions can be removed.
////

class example_CameraController : public Script {
public:
	void OnUpdate() {
		auto& transform = GetComponent<TransformComponent>();
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
		// return: Does this event function consume the event
		return false;
	}
};

class ExampleScene : public Scene {
	void example_UI() {
		Entity canvas = AddCanvas();
		UIEntityParams topCornerParams = UIEntityParams("Top corner square", (UIAlignComponent::Driver)UIAlignComponent::XDriver::ConstLeft 
			| (UIAlignComponent::Driver)UIAlignComponent::WidthDriver::RelativeWidth, (UIAlignComponent::Driver)UIAlignComponent::YDriver::ConstTop 
			| (UIAlignComponent::Driver)UIAlignComponent::HeightDriver::RelativeHeight, 25, 25, 0.15f, 0.1f, false, false);
		UIEntityParams middleSquare = UIEntityParams("middleSquare", (UIAlignComponent::Driver)UIAlignComponent::LeftSideDriver::ConstOffset 
			| (UIAlignComponent::Driver)UIAlignComponent::RightSideDriver::ConstOffset, (UIAlignComponent::Driver)UIAlignComponent::TopDriver::ConstOffset 
			| (UIAlignComponent::Driver)UIAlignComponent::BottomDriver::ConstOffset, 25, 15, 25, 15, true, true);

		Entity exampleSquare = AddUIEntity(topCornerParams, canvas);
		exampleSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.3f, 0.3f, 0.3f, 1 });

		Entity exampleOtherSquare = AddUIEntity(middleSquare, exampleSquare);
		exampleOtherSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.3f, 0.2f, 1 });
		exampleOtherSquare.GetComponent<MetadataComponent>().subSorting = 1;

		auto& textComp = AddUIEntity("Text", canvas).AddComponent<TextComponent>();
		textComp.SetText("Example scene");
		textComp.data.fontSize = 10;
		textComp.data.alignVertical = TextAlignVertical::Middle;
		textComp.data.alignHorizontal = TextAlignHorizontal::Middle;
	}

	Entity example_ParticleBegin() {
		Entity particle = AddEntity(EntityParams("ParticleSystem"));
		particle.GetComponent<TransformComponent>().SetLocalPosition(0, 8.5f);
		auto& particleSystem = particle.AddComponent<ParticleSystemComponent>(100000).particleSystem; // <-- The max amount of particles at a time
		Ref<Particles::Emitter> emitter = CreateRef<Particles::Emitter>(30.0f); // <-- The amount of particles spawned per second

		emitter->AddSetter(CreateRef<Particles::BoxPosSetter>(glm::vec2{ 0.15f, 0 }));
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
		auto& cameraComp = camera.AddComponent<CameraComponent>();
		cameraComp.camera.SetSize(8.85f);
		cameraComp.backgroundColor = { 0.19f, 0.32f, 0.45f, 1.0f };
		camera.GetComponent<TransformComponent>().SetPosition(0.0f, -0.6f);
		SetPrimaryCamera(camera);
		
		auto& player = AddEntity("Player");
		auto texture = Texture::Create("Assets/Player.png", false);
		player.AddComponent<SpriteRendererComponent>(SubTexture::CreateFromIndexes(texture, { 0, 0 }, { 16, 16 }));
		player.GetComponent<TransformComponent>().SetPosition(-4.2f, -1.5f);
		
		Entity ground = AddEntity("Ground");
		ground.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.4f, 0.42f, 0.52f, 1 });
		ground.GetComponent<TransformComponent>().SetScale(13, 8);
		ground.GetComponent<TransformComponent>().SetPosition(0, -6);
		
		Entity particleSystem = example_ParticleBegin();
		
		Entity fireHydrant = AddEntity(EntityParams("Fire hydrant"));
		fireHydrant.AddComponent<SpriteRendererComponent>(Texture::Create("Assets/FireHydrant.png", false));
		fireHydrant.GetComponent<TransformComponent>().SetScale(0.5f, 8);
		fireHydrant.GetComponent<TransformComponent>().SetLocalPosition(0, 4.5f);
		
		Entity pedistal = AddEntity(EntityParams("Firehydrant stucture"), fireHydrant, particleSystem);
		pedistal.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.5f, 0.52f, 0.62f, 1 });
		pedistal.GetComponent<TransformComponent>().SetScale(2, 0.25f);
		pedistal.GetComponent<TransformComponent>().SetPosition(1, -1.875f);
		
		example_UI();

		camera.AddComponent<NativeScriptComponent>().Bind<example_CameraController>();
		Serialize::Save("file.bin", WindowProps("Show", 10, 10));
	}
	void SceneEnd() {

	}
};

Ref<Scene> Egl::ApplicationStartup() {
	return CreateRef<ExampleScene>();
}