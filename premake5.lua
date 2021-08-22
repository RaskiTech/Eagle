workspace "Eagle"
	architecture "x64"
	startproject "Engine"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	-- Includedirs relative to the solution dir --
	Includes = {}
	Includes["GLFW"] = "Engine/vendor/GLFW/include"
	Includes["Glad"] = "Engine/vendor/Glad/include"
	Includes["glm"] = "Engine/vendor/glm"
	Includes["ImGui"] = "Engine/vendor/ImGui"
	Includes["stb_image"] = "Engine/vendor/stb_image"
	Includes["entt"] = "Engine/vendor/entt/include"
	Includes["spdlog"] = "Engine/vendor/spdlog/include"
	Includes["FreeType"] = "Engine/vendor/FreeType/include"

	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
	location "Engine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
	debugdir "Build"

	postbuildcommands {
		"{copy} ../bin/" .. outputDir .. "/%{prj.name}/%{prj.name}.exe ../Build"
	}

	pchheader "EaglePCH.h"
	pchsource "Engine/src/EaglePCH.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs {
		"%{prj.name}/src",
		"%{Includes.spdlog}",
		"%{Includes.GLFW}",
		"%{Includes.Glad}",
		"%{Includes.glm}",
		"%{Includes.stb_image}",
		"%{Includes.ImGui}",
		"%{Includes.entt}",
		"%{Includes.FreeType}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"freetype.lib",
		"opengl32.lib",
		"Application"
	}
	libdirs {
		"%{prj.name}/vendor/FreeType/lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"EAGLE_PLATFORM_WINDOWS",
			"EAGLE_RENDERER_OPENGL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "EAGLE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EAGLE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EAGLE_DIST"
		buildoptions "/MD"
		optimize "on"

project "Application"
	location "Application"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{Includes.spdlog}",
		"%{Includes.glm}",
		"%{Includes.entt}",
		"%{Includes.ImGui}",
		"Engine/src"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"EAGLE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EAGLE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EAGLE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EAGLE_DIST"
		buildoptions "/MD"
		optimize "on"

group "Dependencies"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/Glad"
	include "Engine/vendor/ImGui"