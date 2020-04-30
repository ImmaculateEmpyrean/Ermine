workspace "UniversumErminia"
    architecture "x86_64"
    startproject "Ermine"

    configurations{
        "Debug_Development", --This is supposed to have things like tilemap editor and such imgui tools
        "Debug_Distribution", --This is to check distribution build with debug symbols included
        "Release_Distribution" --Use This Build To Ship
    }

    platforms{
        "Windows"
    }

    flags{
        "MultiprocessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.platform}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Glad"]="vendor/Glad/include"
IncludeDir["GLFW"]="vendor/GLFW/glfw-3.3.2/include"

group "Dependencies"
    include "vendor/Glad"
    include "vendor/GLFW"
group ""

project "Ermine"
    location "Ermine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    ErmineTargetDirectory = "bin/"..outputdir.."/%{prj.name}"
    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    pchheader "stdafx.h"
    pchsource "%{prj.name}/src/stdafx.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/spdlog-1.x/include/spdlog/**.h"
    }

    includedirs { 
         "%{prj.name}/src",
         "%{prj.name}/vendor/spdlog-1.x/include/",
         "%{IncludeDir.Glad}",
         "%{IncludeDir.GLFW}"
    }

    links {
        "Glad",
        "GLFW",
        "opengl32.lib",
        "Game"
    }

    defines{
        "DLL=__declspec(dllimport)"
    }

    filter "configurations:Debug_Development"
        defines{
          "ER_DEBUG_DEVELOP"
        }
    
    filter "configurations:Debug_Distribution"
        defines{
            "ER_DEBUG_SHIP"
        }
    
    filter "configurations:Release_Distribution"
        defines{
            "ER_RELEASE_SHIP"
        }
    
    filter "platforms:Windows"
        defines{
            "ER_BUILDING_FOR_WINDOWS"
        }
        
    filter "system:windows"
        systemversion "latest"

        defines{
            "ER_BUILDING_ON_WINDOWS"
        }

GameEngineProjectName = "%{prj.name}"

        --Game Project Description--

project "Game"
    location "Game"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    GameTargetDirectory = "bin/"..outputdir.."/%{prj.name}"
    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    --pchheader "stdafx.h"
    --pchsource "%{prj.name}/src/stdafx.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        GameEngineProjectName.."/vendor/spdlog-1.x/include/spdlog/**.h"
    }

    includedirs { 
         "%{prj.name}/src",
         GameEngineProjectName.."/vendor/spdlog-1.x/include/",
         "%{IncludeDir.Glad}",
         "%{IncludeDir.GLFW}"
    }

    links {
        "Glad",
        "GLFW",
        "opengl32.lib"
    }
    defines{
        "DLL=__declspec(dllexport)"
    }

    GameDllPath = GameTargetDirectory.."/Game.dll"

    postbuildcommands {
       ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Ermine/\"")
      }

    filter "configurations:Debug_Development"
        defines{
          "GAME_DEBUG_DEVELOP"
        }
    
    filter "configurations:Debug_Distribution"
        defines{
            "GAME_DEBUG_SHIP"
        }
    
    filter "configurations:Release_Distribution"
        defines{
            "GAME_RELEASE_SHIP"
        }
    
    filter "platforms:Windows"
        defines{
            "GAME_BUILDING_FOR_WINDOWS"
        }
        
    filter "system:windows"
        systemversion "latest"

        defines{
            "GAME_BUILDING_ON_WINDOWS"
        }

                --Game Project Description End --