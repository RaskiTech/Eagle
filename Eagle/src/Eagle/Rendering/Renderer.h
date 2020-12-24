#pragma once

// How rendering works:
// The Renderer.cpp will look what renderer it will use at compile time. It will compile only the scripts that are for that renderer.
// To add a new renderer, create new function declarions in a platform specific file and include it as an option in the Renderer.cpp file.

namespace Egl {
	class Renderer {

	};
}