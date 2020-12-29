#include <EaglePCH.h>

// Disable warninigs that are coused by imgui
#pragma warning( push )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26451 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 6031 )

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"