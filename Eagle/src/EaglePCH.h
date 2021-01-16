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

#include "Eagle/Core/Keycodes.h"
#include "Eagle/Core/Log.h"
#include "Eagle/Debug/Instrumentor.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/Core/Events/ApplicationEvent.h"
#include "Eagle/Core/Events/MouseEvent.h"
#include "Eagle/Core/Events/KeyEvent.h"

#ifdef EAGLE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif