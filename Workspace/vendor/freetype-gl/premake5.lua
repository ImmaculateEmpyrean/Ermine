project "freetype-gl"
	kind "StaticLib"
	language "C++"

	targetdir ("../../bin/"..outputdir.."/%{prj.name}")
	objdir ("../../bin-int/"..outputdir.."/%{prj.name}")

	files
	{
		"freetype-gl-master/platform.c",
		"freetype-gl-master/texture-atlas.c",
		"freetype-gl-master/texture-font.c",
		"freetype-gl-master/vector.c",
		"freetype-gl-master/freetype-gl.h",
		"freetype-gl-master/opengl.h",
		"freetype-gl-master/platform.h",
		"freetype-gl-master/texture-atlas.h",
		"freetype-gl-master/vec234.h",
		"freetype-gl-master/vector.h"
	}

	includedirs
	{
		"freetype-gl-master",
		"../freetype/freetype-2.10.2/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
        staticruntime "off"
        defines{
            "ER_BUILDING_ON_WINDOWS"
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