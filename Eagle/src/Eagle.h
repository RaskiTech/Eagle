#pragma once

// This file is for applications only. They can do #include <Eagle.g> and have everything needed

#include "Eagle/Core/Application.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Core/Log.h"
#include "Eagle/Core/Layers/Layer.h"
#include "Eagle/Core/Keycodes.h"
#include "Eagle/Core/Input.h"

#include "Eagle/ImGui/ImGuiLayer.h"
#include "Eagle/Components/CameraController.h"

//Rendering
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Rendering/Camera.h"
#include "Eagle/Rendering/VertexArray.h"
#include "Eagle/Rendering/Buffer.h"
#include "Eagle/Rendering/Shader.h"
#include "Eagle/Rendering/Texture.h"