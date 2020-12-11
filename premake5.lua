workspace "Eagle"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	include "Eagle/vendor/GLFW"
	include "Eagle/vendor/Glad"
	include "Eagle/vendor/ImGui"

project "Eagle"
	location "Eagle"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	pchheader "EaglePCH.h"
	pchsource "Eagle/src/EaglePCH.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/ImGui",
		"%{prj.name}/vendor/glm"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines {
			"EAGLE_PLATFORM_WINDOWS",
			"EAGLE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputDir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "EAGLE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "EAGLE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "EAGLE_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Eagle/vendor/spdlog/include",
		"Eagle/vendor/glm",
		"Eagle/src"
	}

	links {
		"Eagle"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines {
			"EAGLE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EAGLE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "EAGLE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "EAGLE_DIST"
		buildoptions "/MD"
		optimize "On"