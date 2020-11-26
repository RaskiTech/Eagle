workspace "Eagle"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Eagle"
	location "Eagle"
	kind "SharedLib"
	language "C++"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {
			"EAGLE_PLATFORM_WINDOWS",
			"EAGLE_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "EAGLE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "EAGLE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "EAGLE_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Eagle/vendor/spdlog/include",
		"Eagle/src"
	}

	links {
		"Eagle"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {
			"EAGLE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EAGLE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "EAGLE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "EAGLE_DIST"
		optimize "On"