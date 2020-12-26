#pragma once

// This file is for applications only. They can do #include <Eagle.g> and have everything needed

#include "Eagle/Application.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Log.h"
#include "Eagle/Layers/Layer.h"
#include "Eagle/Keycodes.h"
#include "Eagle/Input.h"

#include "Eagle/ImGui/ImGuiLayer.h"


//Rendering
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Rendering/Camera.h"
#include "Eagle/Rendering/VertexArray.h"
#include "Eagle/Rendering/Buffer.h"
#include "Eagle/Rendering/Shader.h"

// Entry point
#include "Eagle/EntryPoint.h"