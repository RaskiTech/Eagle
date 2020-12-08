#include <EaglePCH.h>
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platforms/OpenGL/ImGuiOpenGLRenderer.h"
#include "Eagle/Application.h"

// TEMP
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Egl {
    ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") {

    }
    ImGuiLayer::~ImGuiLayer() {

    }
    void ImGuiLayer::OnAttach() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // TEMP: Should switch to Eagle keycodes
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
    }
    void ImGuiLayer::OnDetach() {

    }
    void ImGuiLayer::OnUpdate() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        float time = glfwGetTime();
        io.DeltaTime = mTime > 0 ? (time = mTime) : (1.0f / 60.0f);
        mTime = time;

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void ImGuiLayer::OnEvent(Event& event) {
        EventDispatcher dispacher = EventDispatcher(event);
        dispacher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispacher.Dispatch<MousePressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMousePressedEvent));
        dispacher.Dispatch<MouseReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseReleasedEvent));
        dispacher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispacher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEventEvent));
        dispacher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispacher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispacher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }
    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetPosX(), e.GetPosY());
        return false;
    }
    bool ImGuiLayer::OnMousePressedEvent(MousePressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetButton()] = true;
        return false;
    }
    bool ImGuiLayer::OnMouseReleasedEvent(MouseReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetButton()] = false;
        return false;
    }
    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.GetScrollY();
        io.MouseWheelH += e.GetScrollX();
        return false;
    }
    bool ImGuiLayer::OnKeyPressedEventEvent(KeyPressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;
        LOG("Key pressed");
        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        return false;
    }
    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;
        return false;
    }
    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        int key = e.GetKeyCode();
        if (key > 0 && key < 0x10000)
            io.AddInputCharacter((unsigned short)key);

        return false;
    }
    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());
        return false;
    }
}
