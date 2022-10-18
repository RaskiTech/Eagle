#include <EaglePCH.h>
#include <Eagle/Debug/ImGui/ImGuiLayer.h>
#include <Dependencies/ImGui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Eagle/Core/Application.h"

// TEMP
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Egl {
    ImGuiLayer::ImGuiLayer() { }
    ImGuiLayer::~ImGuiLayer() { }

    void ImGuiLayer::OnAttach() {
        EAGLE_PROFILE_FUNCTION();
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Bold.ttf", 17.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Regular.ttf", 17.0f);

        io.IniFilename = iniFilePath.c_str();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        SetDarkThemeColors();

        Application& app = Application::Get();
        GLFWwindow* window = (GLFWwindow*)app.GetWindow().NativeWindow();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

    }
    void ImGuiLayer::OnDetach() {
        EAGLE_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin() {
        EAGLE_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void ImGuiLayer::End() {
        EAGLE_PROFILE_FUNCTION();
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
    void ImGuiLayer::SetDarkThemeColors() {
        ImGuiStyle& style = ImGui::GetStyle();

        style.Colors[ImGuiCol_WindowBg]           = EDITOR_COLOR_DARK;

        style.Colors[ImGuiCol_Header]             = EDITOR_COLOR_LIGHTBLUE1;
        style.Colors[ImGuiCol_HeaderHovered]      = EDITOR_COLOR_LIGHTBLUE1;
        style.Colors[ImGuiCol_HeaderActive]       = EDITOR_COLOR_LIGHTBLUE1;

        style.Colors[ImGuiCol_Button]             = EDITOR_COLOR_DARKGRAY2;
        style.Colors[ImGuiCol_ButtonHovered]      = EDITOR_COLOR_DARKGRAY1;
        style.Colors[ImGuiCol_ButtonActive]       = EDITOR_COLOR_DARKGRAY1;

        style.Colors[ImGuiCol_FrameBg]            = EDITOR_COLOR_DARKGRAY2;
        style.Colors[ImGuiCol_FrameBgHovered]     = EDITOR_COLOR_DARKGRAY1;
        style.Colors[ImGuiCol_FrameBgActive]      = EDITOR_COLOR_DARKGRAY1;

        style.Colors[ImGuiCol_Tab]                = EDITOR_COLOR_VIOLET1;//EDITOR_COLOR_LIGHTBLUE1;
        style.Colors[ImGuiCol_TabHovered]         = EDITOR_COLOR_VIOLET2;//EDITOR_COLOR_LIGHTBLUE2;
        style.Colors[ImGuiCol_TabActive]          = EDITOR_COLOR_VIOLET2;//EDITOR_COLOR_LIGHTBLUE2;
        style.Colors[ImGuiCol_TabUnfocused]       = EDITOR_COLOR_VIOLET1;//EDITOR_COLOR_LIGHTBLUE1;
        style.Colors[ImGuiCol_TabUnfocusedActive] = EDITOR_COLOR_VIOLET2;//EDITOR_COLOR_LIGHTBLUE2;

        style.Colors[ImGuiCol_TitleBg]            = EDITOR_COLOR_DARKGRAY3;
        style.Colors[ImGuiCol_TitleBgActive]      = EDITOR_COLOR_DARKGRAY3;
        style.Colors[ImGuiCol_TitleBgCollapsed]   = EDITOR_COLOR_DARKGRAY3;

        style.Colors[ImGuiCol_CheckMark]          = EDITOR_COLOR_WHITE1;
        style.Colors[ImGuiCol_Text]               = EDITOR_COLOR_WHITE1;
        style.Colors[ImGuiCol_TextDisabled]       = EDITOR_COLOR_WHITE2;

        style.Colors[ImGuiCol_Separator]          = EDITOR_COLOR_DARKGRAY3;
        style.Colors[ImGuiCol_SeparatorHovered]   = EDITOR_COLOR_LIGHTGRAY2;
        style.Colors[ImGuiCol_SeparatorActive]    = EDITOR_COLOR_LIGHTGRAY2;
    }
    void ImGuiLayer::OnEvent(Event& event) {
        if (!mLetMouseThrough) {
            ImGuiIO& io = ImGui::GetIO();
            event.handled |= event.IsInGategory(EventGategory::Mouse) & io.WantCaptureMouse;
            event.handled |= event.IsInGategory(EventGategory::Keyboard) & io.WantCaptureKeyboard;
        }
    }
}
