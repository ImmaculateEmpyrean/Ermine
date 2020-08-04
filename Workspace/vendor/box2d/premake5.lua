project "Box2D"
    location ""
    kind "StaticLib"

	language "C++"
    cppdialect "C++17"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"include/box2d/**.h",
		"src/**.cpp",
		"src/**.h"
	}

	includedirs
	{
		"include",
		"src"
	}

    defines
	{
        "_CRT_SECURE_NO_WARNINGS"
	}
	
    filter "system:windows"
    systemversion "latest"
    cppdialect "C++17"
    staticruntime "off"
   
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
