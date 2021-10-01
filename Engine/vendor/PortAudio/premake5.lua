project "PortAudio"
  kind "StaticLib"
  language "C"
  staticruntime "on"
  
  targetdir ("bin/" .. outputDir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
  
  includedirs {
    "include",
    "src/common",
    "src/hostapi",
    "src/dsound"
  }
  files {
    "src/common/**.h",
    "src/common/**.c"
  }
  
  filter "system:windows"
    systemversion "latest"
  
    includedirs { "src/os/win" }

    files {
      "src/os/win/**.c",
      "src/os/win/**.h",

      "src/hostapi/dsound/pa_win_ds.c",
      "src/hostapi/dsound/pa_win_ds_dynlink.c",
      "src/hostapi/dsound/pa_win_ds_dynlink.h"
    }
    
    defines {
      "PA_USE_DS"
    }

  filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"