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
IncludeDir["Glad"] = "Usagi/vendor/Glad/include"
IncludeDir["ImGui"] = "Usagi/vendor/imgui"
IncludeDir["glm"] = "Usagi/vendor/glm"


include "Usagi/vendor/GLFW"
include "Usagi/vendor/Glad"
include "Usagi/vendor/imgui"

project "Usagi"
    location "Usagi"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"


    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin_int/" .. outputdir .. "/%{prj.name}")

    pchheader "usgpch.h"
    pchsource "Usagi/src/usgpch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include", 
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
        
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    links{
        "GLFW", 
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"

        defines{
            "USAGI_PLATFORM_WINDOWS",
            "USAGI_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
    filter "configurations:Debug"
        defines "USG_DEBUG"
        runtime "Debug"
        symbols "on"
    filter "configurations:Release"
        defines "USG_RELEASE"
        runtime "Release"
        optimize "on"
    filter "configurations:Dist"
        defines "USG_DIST"
        runtime "Release"
        optimize "on"
    

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin_int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Usagi/vendor/spdlog/include",
        "Usagi/src",
        "Usagi/vendor",
        "%{IncludeDir.glm}"
    }

    links{
        "Usagi"
    }

    filter "system:windows"
        systemversion "latest"

        defines{
            "USAGI_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "USG_DEBUG"
        runtime "Debug"
        symbols "on"
    filter "configurations:Release"
        defines "USG_RELEASE"
        runtime "Release"
        optimize "on"
    filter "configurations:Dist"
        defines "USG_DIST"
        runtime "Release"
        optimize "on"