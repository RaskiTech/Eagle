#include <EaglePCH.h>

// Disable warninigs that are coused by imgui
#pragma warning( push )
#pragma warning( disable : 26495 )

#include <Eagle/ImGui/ImGuiLayer.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Eagle/Core/Application.h"

// TEMP
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Egl {
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer", true) {

    }
    ImGuiLayer::~ImGuiLayer() {

    }
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

        // From british palette, flatuicolors
        ImVec4 white1     = ImVec4{ 245.0f / 255, 246.0f / 255, 250.0f / 255, 1.0f };
        ImVec4 white2     = ImVec4{ 220.0f / 255, 221.0f / 255, 225.0f / 255, 1.0f };

        ImVec4 violet1    = ImVec4{ 156.0f / 255 - 0.1f, 136.0f / 255 - 0.1f, 255.0f / 255 - 0.1f, 1.0f };
        ImVec4 violet2    = ImVec4{ 140.0f / 255 - 0.1f, 122.0f / 255 - 0.1f, 230.0f / 255 - 0.1f, 1.0f };

        ImVec4 lightBlue1 = ImVec4{ 64.0f / 255, 115.0f / 255, 158.0f / 255, 1.0f };
        ImVec4 lightBlue2 = ImVec4{ 72.0f / 255, 126.0f / 255, 176.0f / 255, 1.0f };
                                        
        ImVec4 darkBlue1  = ImVec4{ 25.0f / 255, 42.0f / 255, 86.0f / 255, 1.0f };
        ImVec4 darkBlue2  = ImVec4{ 39.0f / 255, 60.0f / 255, 117.0f / 255, 1.0f };

        ImVec4 lightGray1 = ImVec4{ 127.0f / 255, 143.0f / 255, 166.0f / 255, 1.0f };
        ImVec4 lightGray2 = ImVec4{ 113.0f / 255, 128.0f / 255, 147.0f / 255, 1.0f };

        ImVec4 darkGray0  = ImVec4{ 53.0f / 255+0.15f, 59.0f / 255+0.15f, 72.0f / 255+0.15f, 1.0f };
        ImVec4 darkGray1  = ImVec4{ 53.0f / 255+0.1f, 59.0f / 255 + 0.1f, 72.0f / 255 + 0.1f, 1.0f };
        ImVec4 darkGray2  = ImVec4{ 47.0f / 255, 54.0f / 255, 64.0f / 255, 1.0f };

        ImVec4 dark = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };

        style.Colors[ImGuiCol_WindowBg]           = dark;

        style.Colors[ImGuiCol_Header]             = lightBlue1;
        style.Colors[ImGuiCol_HeaderHovered]      = lightBlue1;
        style.Colors[ImGuiCol_HeaderActive]       = lightBlue1;

        style.Colors[ImGuiCol_Button]             = darkGray1;
        style.Colors[ImGuiCol_ButtonHovered]      = darkGray0;
        style.Colors[ImGuiCol_ButtonActive]       = darkGray0;
                                                    
        style.Colors[ImGuiCol_FrameBg]            = darkGray1;
        style.Colors[ImGuiCol_FrameBgHovered]     = darkGray0;
        style.Colors[ImGuiCol_FrameBgActive]      = darkGray0;

        style.Colors[ImGuiCol_Tab]                = lightBlue1;
        style.Colors[ImGuiCol_TabHovered]         = lightBlue2;
        style.Colors[ImGuiCol_TabActive]          = lightBlue2;
        style.Colors[ImGuiCol_TabUnfocused]       = lightBlue1;
        style.Colors[ImGuiCol_TabUnfocusedActive] = lightBlue2;
                                                                   
        style.Colors[ImGuiCol_TitleBg]            = darkGray2;
        style.Colors[ImGuiCol_TitleBgActive]      = darkGray2;
        style.Colors[ImGuiCol_TitleBgCollapsed]   = darkGray2;

        style.Colors[ImGuiCol_CheckMark]          = white1;
        style.Colors[ImGuiCol_Text]               = white1;
        style.Colors[ImGuiCol_TextDisabled]       = white2;

        style.Colors[ImGuiCol_Separator]          = darkGray2;
        style.Colors[ImGuiCol_SeparatorHovered]   = lightGray2;
        style.Colors[ImGuiCol_SeparatorActive]    = lightGray2;
    }
    void ImGuiLayer::OnEvent(Event& event) {
        if (!mLetMouseThrough) {
            ImGuiIO& io = ImGui::GetIO();
            event.handled |= event.IsInGategory(EventGategoryMouse) & io.WantCaptureMouse;
            event.handled |= event.IsInGategory(EventGategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }
}
