workspace "Usagi"
    architecture "x64" 

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

-- Include directories relative to root folder (solution directories)
IncludeDir = {}
IncludeDir["GLFW"] = "Usagi/vendor/GLFW/include"

include "Usagi/vendor/GLFW"

project "Usagi"
    location "Usagi"
    kind "SharedLib"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin_int/" .. outputdir .. "/%{prj.name}")

    pchheader "usgpch.h"
    pchsource "Usagi/src/usgpch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include", 
        "%{IncludeDir.GLFW}"
    }

    links{
        "GLFW", 
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            "USAGI_PLATFORM_WINDOWS",
            "USAGI_BUILD_DLL"
        }

        postbuildcommands{
            "copy /B /Y ..\\bin\\" .. outputdir .. "\\Usagi\\Usagi.dll ..\\bin\\" .. outputdir .. "\\Sandbox\\ > nul"
        }
    
    filter "configurations:Debug"
        defines "USG_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    filter "configurations:Release"
        defines "USG_RELEASE"
        buildoptions "/MD"
        optimize "On"
    filter "configurations:Dist"
        defines "USG_DIST"
        buildoptions "/MD"
        optimize "On"
    

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin_int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Usagi/vendor/spdlog/include",
        "Usagi/src"
    }

    links{
        "Usagi"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            "USAGI_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "USG_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    filter "configurations:Release"
        defines "USG_RELEASE"
        buildoptions "/MD"
        optimize "On"
    filter "configurations:Dist"
        defines "USG_DIST"
        buildoptions "/MD"
        optimize "On"