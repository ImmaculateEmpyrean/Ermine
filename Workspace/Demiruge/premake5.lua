project "Demiruge"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/"..outputdir.."/%{prj.name}")
    objdir ("../bin-int/"..outputdir.."/%{prj.name}")

    pchheader "Dstdafx.h"
    pchsource "src/PCH/Dstdafx.cpp"

    files{
        "src/**.h",
        "src/**.cpp"
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
        "%{IncludeDir.lua}",

        "%{IncludeDir.Ermine}",
        "%{IncludeDir.ErmineSrc}"
    }

   links {
        "Ermine",
        "Glad",
        "GLFW",
        "opengl32.lib",
        "ImGui",
        "GLM",
        "stb",
        "JSON",
        "Freetype",
        "freetype-gl",
        "Box2D",
        "tinyxml2",
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
          "DEMIRUGE_DEBUG_DEVELOP"
        }
        runtime "Debug"
		symbols "on"
    
    filter "configurations:Debug_Distribution"
        defines{
        	"ER_DEBUG_SHIP",
            "DEMIRUGE_DEBUG_SHIP"
        }
        runtime "Debug"
		symbols "on"
    
    filter "configurations:Release_Distribution"
        defines{
        	"ER_RELEASE_SHIP",
            "DEMIRUGE_RELEASE_SHIP"
        }
        runtime "Release"
		optimize "on"
    
    filter "platforms:Windows"
        defines{
        	"ER_BUILDING_FOR_WINDOWS",
            "DEMIRUGE_BUILDING_FOR_WINDOWS"
        }
        
    filter "system:windows"
        systemversion "latest"

        defines{
        	"ER_BUILDING_ON_WINDOWS",
            "DEMIRUGE_BUILDING_ON_WINDOWS"
        }