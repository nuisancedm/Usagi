workspace "Usagi"
    architecture "x64" 

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

project "Usagi"
    location "Usagi"
    kind "SharedLib"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin_int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
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
        symbols "On"
    filter "configurations:Release"
        defines "USG_RELEASE"
        optimize "On"
    filter "configurations:Dist"
        defines "USG_DIST"
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
        symbols "On"
    filter "configurations:Release"
        defines "USG_RELEASE"
        optimize "On"
    filter "configurations:Dist"
        defines "USG_DIST"
        optimize "On"