workspace "Eagle"
	architecture "x64"
	startproject "Eagle"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	-- Includedirs relative to the solution dir --
	Includes = {}
	Includes["GLFW"] = "Eagle/vendor/GLFW/include"
	Includes["Glad"] = "Eagle/vendor/Glad/include"
	Includes["glm"] = "Eagle/vendor/glm"
	Includes["ImGui"] = "Eagle/vendor/ImGui"
	Includes["stb_image"] = "Eagle/vendor/stb_image"
	Includes["entt"] = "Eagle/vendor/entt/include"
	Includes["spdlog"] = "Eagle/vendor/spdlog/include"
	Includes["FreeType"] = "Eagle/vendor/FreeType/include"

	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Eagle"
	location "Eagle"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	pchheader "EaglePCH.h"
	pchsource "Eagle/src/EaglePCH.cpp"

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

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"freetype.lib",
		"Sandbox"
	}
	libdirs {
		"Eagle/vendor/FreeType/lib"
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

project "Sandbox"
	location "Sandbox"
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
		"Eagle/src"
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
	include "Eagle/vendor/GLFW"
	include "Eagle/vendor/Glad"
	include "Eagle/vendor/ImGui"