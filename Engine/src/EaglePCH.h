#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <sstream>
#include <algorithm>
#include <limits>

#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include <Dependencies/GLM.h>
#include <Dependencies/Entt.h>
#include <Dependencies/ImGui.h>

#include "Eagle/Core/Core.h"
#include "Eagle/Debug/Instrumentor.h"
#include "Eagle/Core/Keycodes.h"
#include "Eagle/Core/Log.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/Core/Events/ApplicationEvent.h"
#include "Eagle/Core/Events/MouseEvent.h"
#include "Eagle/Core/Events/KeyEvent.h"

#include "Eagle/Rendering/Buffer.h"
#include "Eagle/Rendering/Camera.h"
#include "Eagle/Rendering/Framebuffer.h"
#include "Eagle/Rendering/RenderAPI.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/Shader.h"
#include "Eagle/Rendering/Texture.h"
#include "Eagle/Rendering/VertexArray.h"

#ifdef EAGLE_PLATFORM_WINDOWS
#    define NOMINMAX // Don't confuse min and max with other defenitions
#    include <windows.h>
#    undef NOMINMAX
#endif
