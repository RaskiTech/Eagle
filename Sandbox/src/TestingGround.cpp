#include "TestingGround.h"
#include "ImGui/imgui.h"

TestingGround::TestingGround() : Layer("TestingGround") {

}

///////////////////// On Attach //////////////////////
void TestingGround::OnAttach() {
	EAGLE_PROFILE_FUNCTION();

}

void TestingGround::OnDetach() {
	EAGLE_PROFILE_FUNCTION();
}

///////////////////// On Update //////////////////////
void TestingGround::OnUpdate() {
	EAGLE_PROFILE_FUNCTION();
}

void TestingGround::OnEvent(Egl::Event& event) {
	EAGLE_PROFILE_FUNCTION();
}

void TestingGround::OnImGuiRender() {
	EAGLE_PROFILE_FUNCTION();

	ImGui::Begin("Example window");
	ImGui::Text("Scene editor");
	ImGui::ColorEdit4("Square color", color);
	ImGui::ColorEdit4("Texture color", texColor);
	ImGui::DragFloat("Rotation", &rotation, 0.01f, -10, 10);
	ImGui::End();
}