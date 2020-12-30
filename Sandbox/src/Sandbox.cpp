#include <Eagle.h>
#include <Eagle/Core/EntryPoint.h>

#include "ImGui/imgui.h"
#include "TestingGround.h"

using namespace Egl;

class BaseLayer : public Layer {
public:
	BaseLayer() : Layer("BaseLayer", true) 
	{

	}
};
class Sandbox : public Egl::Application {
public:
	Sandbox() {
		auto texture2 = Texture::Create("Assets/Water.png");
		auto texture3 = Texture::Create("Assets/Water.png");
		auto texture4 = Texture::Create("Assets/Water.png");
		auto texture5 = Texture::Create("Assets/Water.png");
		auto texture6 = Texture::Create("Assets/Water.png");
		auto texture7 = Texture::Create("Assets/Water.png");
		auto texture8 = Texture::Create("Assets/Water.png");
		auto texture9 = Texture::Create("Assets/Water.png");
		auto texture0 = Texture::Create("Assets/Water.png");
		auto texture1 = Texture::Create("Assets/Water.png");
		auto texture = Texture::Create("Assets/Water.png");
		auto texture12 = Texture::Create("Assets/Water.png");
		auto texture13 = Texture::Create("Assets/Water.png");
		auto texture14 = Texture::Create("Assets/Water.png");
		auto texture15 = Texture::Create("Assets/Water.png");
		auto texture16 = Texture::Create("Assets/Water.png");
		auto texture17 = Texture::Create("Assets/Water.png");
		auto texture18 = Texture::Create("Assets/Water.png");
		auto texture19 = Texture::Create("Assets/Water.png");
		auto texture21 = Texture::Create("Assets/Water.png");
		auto texture22 = Texture::Create("Assets/Water.png");
		auto texture23 = Texture::Create("Assets/Water.png");
		auto texture24 = Texture::Create("Assets/Water.png");
		auto texture25 = Texture::Create("Assets/Water.png");
		
		auto qtexture2 = Texture::Create("Assets/Water.png");
		auto qtexture3 = Texture::Create("Assets/Water.png");
		auto qtexture4 = Texture::Create("Assets/Water.png");
		auto qtexture5 = Texture::Create("Assets/Water.png");
		auto qtexture6 = Texture::Create("Assets/Water.png");
		auto qtexture7 = Texture::Create("Assets/Water.png");
		auto qtexture8 = Texture::Create("Assets/Water.png");
		auto qtexture9 = Texture::Create("Assets/Water.png");
		auto qtexture0 = Texture::Create("Assets/Water.png");
		auto qtexture1 = Texture::Create("Assets/Water.png");
		auto qtexture = Texture::Create("Assets/Water.png");
		auto qtexture12 = Texture::Create("Assets/Water.png");
		auto qtexture13 = Texture::Create("Assets/Water.png");
		auto qtexture14 = Texture::Create("Assets/Water.png");
		auto qtexture15 = Texture::Create("Assets/Water.png");
		auto qtexture16 = Texture::Create("Assets/Water.png");
		auto qtexture17 = Texture::Create("Assets/Water.png");
		auto qtexture18 = Texture::Create("Assets/Water.png");
		auto qtexture19 = Texture::Create("Assets/Water.png");
		auto qtexture21 = Texture::Create("Assets/Water.png");
		auto qtexture22 = Texture::Create("Assets/Water.png");
		auto qtexture23 = Texture::Create("Assets/Water.png");
		auto qtexture24 = Texture::Create("Assets/Water.png");
		auto qtexture25 = Texture::Create("Assets/Water.png");
		EAGLE_PROFILE_FUNCTION();
		AddLayer(new TestingGround());
		//AddLayer(new BaseLayer());
	}

	~Sandbox() {

	}
};

Egl::Application* Egl::CreateApplication() {
	return new Sandbox();
}
