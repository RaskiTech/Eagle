#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

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
#include "Eagle/Rendering/FrameBuffer.h"
#include "Eagle/Rendering/RenderAPI.h"
#include "Eagle/Rendering/Shader.h"
#include "Eagle/Rendering/Texture.h"
#include "Eagle/Rendering/VertexArray.h"

#ifdef EAGLE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif