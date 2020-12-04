workspace "UniversumErminia"
    architecture "x86_64"
    startproject "Game"

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
GameEngineProjectName = "Ermine"

IncludeDir = {}
IncludeDir["Glad"]="vendor/Glad/include"
IncludeDir["GLFW"]="vendor/GLFW/glfw-3.3.2/include"
IncludeDir["ImGui"]="vendor/ImGuiDocking"
IncludeDir["GLM"]="vendor/GLM/glm"
IncludeDir["stb"]="vendor/stb"
IncludeDir["JSON"]="vendor/JSON/json-3.7.3/single_include"
IncludeDir["JSON_ERMINE"]="vendor/JSON"
IncludeDir["freetype"]="vendor/freetype/freetype-2.10.2/include"
IncludeDir["freetypegl"]="vendor/freetype-gl"
IncludeDir["Box2D"] ="vendor/box2d/include"
IncludeDir["tinyxml2"] = "vendor/tinyxml2"
IncludeDir["libtmxParser"] = "vendor/libtmx-parser/src"
IncludeDir["tmxlite"] = "vendor/tmxlite/tmxlite/include"
IncludeDir["lua"] = "vendor/lua/lua-5.4.2/src"
IncludeDir["sol2"] = "vendor/sol2/sol2-3.2.2/single/include"

group "Dependencies"
    include "vendor/Glad"
    include "vendor/GLFW"
    include "vendor/ImGuiDocking"
    include "vendor/GLM"
    include "vendor/stb"
    include "vendor/JSON"
    include "vendor/freetype"
    include "vendor/freetype-gl"
	include "vendor/box2d"
	include "vendor/tinyxml2"
	include "vendor/libtmx-parser"
	include "vendor/tmxlite"
	include "vendor/lua"
	include "vendor/sol2"
--group ""
project "LogSystem"
    location "LogSystem"
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    staticruntime "off"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/spdlog-1.x/include/spdlog/**.h"
    }

    includedirs { 
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog-1.x/include/"
    }

    filter "configurations:Debug_Development"
    defines{
      "ER_DEBUG_DEVELOP"
    }
    runtime "Debug"
		symbols "on"

filter "configurations:Debug_Distribution"
    defines{
        "ER_DEBUG_SHIP"
    }
    runtime "Debug"
		symbols "on"

filter "configurations:Release_Distribution"
    defines{
        "ER_RELEASE_SHIP"
    }
    runtime "Release"
		optimize "on"

filter "platforms:Windows"
    defines{
        "ER_BUILDING_FOR_WINDOWS"
    }
    
filter "system:windows"
    systemversion "latest"

    defines{
        "ER_BUILDING_ON_WINDOWS"
    }

group ""

project "Ermine"
    location "Ermine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    pchheader "stdafx.h"
    pchsource "%{prj.name}/src/PCH/stdafx.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
        --"%LogSystem/vendor/spdlog-1.x/include/spdlog/**.h"
    }

    includedirs { 
         "%{prj.name}/src",
         "%{prj.name}/src/PCH",
         "LogSystem/vendor/spdlog-1.x/include/",
         "%{IncludeDir.Glad}",
         "%{IncludeDir.GLFW}",
         "LogSystem/src/",
         "GameErmineCommonExchangeHeaders/",
         "%{IncludeDir.ImGui}",
         "%{IncludeDir.GLM}",
         "%{IncludeDir.stb}",
         "%{IncludeDir.JSON}",
         "%{IncludeDir.JSON_ERMINE}",
         "%{IncludeDir.freetype}",
         "%{IncludeDir.freetypegl}",
		 "%{IncludeDir.Box2D}",
		 "%{IncludeDir.tinyxml2}",
		 "%{IncludeDir.libtmxParser}",
		 "%{IncludeDir.tmxlite}",
		 "%{IncludeDir.sol2}",
		 "%{IncludeDir.lua}"
    }

    defines{
        "DLL=__declspec(dllimport)"
    }

    filter "configurations:Debug_Development"
        defines{
          "ER_DEBUG_DEVELOP"
        }
        runtime "Debug"
		symbols "on"
    
    filter "configurations:Debug_Distribution"
        defines{
            "ER_DEBUG_SHIP"
        }
        runtime "Debug"
		symbols "on"
    
    filter "configurations:Release_Distribution"
        defines{
            "ER_RELEASE_SHIP"
        }
        runtime "Release"
		optimize "on"
    
    filter "platforms:Windows"
        defines{
            "ER_BUILDING_FOR_WINDOWS"
        }
        
    filter "system:windows"
        systemversion "latest"

        defines{
            "ER_BUILDING_ON_WINDOWS"
        }

        --Game Project Description--

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    pchheader "stdafx.h"
    pchsource "%{prj.name}/src/PCH/stdafx.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs { 
         "%{prj.name}/src",
         "%{prj.name}/src/PCH",
         "LogSystem/vendor/spdlog-1.x/include/",
         "LogSystem/src/",
         "%{IncludeDir.Glad}",
         "%{IncludeDir.GLFW}",
         "GameErmineCommonExchangeHeaders/",
         "%{IncludeDir.ImGui}",
         "%{IncludeDir.GLM}",
         "%{IncludeDir.stb}",
         "%{IncludeDir.JSON}",
         "%{IncludeDir.JSON_ERMINE}",
         "%{IncludeDir.freetype}",
         "%{IncludeDir.freetypegl}",
		 "%{IncludeDir.Box2D}",
		 "Ermine/src",
		 "%{IncludeDir.tinyxml2}",
		 "%{IncludeDir.libtmxParser}",
		 "%{IncludeDir.tmxlite}",
		 "%{IncludeDir.sol2}",
		 "%{IncludeDir.lua}"
    }

   links {
        "Glad",
        "GLFW",
        "opengl32.lib",
        "LogSystem",
        "ImGui",
        "GLM",
        "stb",
        "JSON",
        "Freetype",
        "freetype-gl",
		"Box2D",
		"Ermine",
		"tinyxml2",
		"libtmx-parser",
		"tmxlite",
		"lua",
		"sol2"
    }

    postbuildcommands {
       ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Ermine/\"")
      }

    filter "configurations:Debug_Development"
        defines{
          "ER_DEBUG_DEVELOP",
          "GAME_DEBUG_DEVELOP"
        }
        runtime "Debug"
		symbols "on"
    
    filter "configurations:Debug_Distribution"
        defines{
        	"ER_DEBUG_SHIP",
            "GAME_DEBUG_SHIP"
        }
        runtime "Debug"
		symbols "on"
    
    filter "configurations:Release_Distribution"
        defines{
        	"ER_RELEASE_SHIP",
            "GAME_RELEASE_SHIP"
        }
        runtime "Release"
		optimize "on"
    
    filter "platforms:Windows"
        defines{
        	"ER_BUILDING_FOR_WINDOWS",
            "GAME_BUILDING_FOR_WINDOWS"
        }
        
    filter "system:windows"
        systemversion "latest"

        defines{
        	"ER_BUILDING_ON_WINDOWS",
            "GAME_BUILDING_ON_WINDOWS"
        }

                --Game Project Description End --