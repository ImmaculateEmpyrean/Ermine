project "Ermine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/"..outputdir.."/%{prj.name}")
    objdir ("../bin-int/"..outputdir.."/%{prj.name}")

    pchheader "stdafx.h"
    pchsource "src/PCH/stdafx.cpp"

    files{
        "src/**.h",
        "src/**.cpp"
    }

    defines{
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs { 
         "src",
         "src/PCH",
         "%{IncludeDir.spdlog}",
         "%{IncludeDir.Glad}",
         "%{IncludeDir.GLFW}",
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
    