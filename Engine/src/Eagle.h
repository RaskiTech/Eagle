#pragma once

// This file is for applications only. They can do #include <Eagle.h> and have everything they need

#include "Eagle/Core/Application.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Core/Log.h"
#include "Eagle/Core/Keycodes.h"
#include "Eagle/Core/Input.h"
#include "Eagle/Core/Random.h"
#include "Eagle/Core/Audio.h"
#include "Eagle/Core/AudioSample.h"
#include "Eagle/Core/AppData.h"

#include "Eagle/Debug/ImGui/ImGuiLayer.h"
#include "Eagle/Components/ParticleComponents/ParticleSystem.h"

#include "Eagle/ECS/Entity.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Script.h"

//Rendering
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Rendering/Camera.h"
#include "Eagle/Rendering/Framebuffer.h"
#include "Eagle/Rendering/VertexArray.h"
#include "Eagle/Rendering/Buffer.h"
#include "Eagle/Rendering/Shader.h"
#include "Eagle/Rendering/Texture.h"