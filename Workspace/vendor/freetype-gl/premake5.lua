project "freetype-gl"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
	objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

	files
	{
		"freetype-gl-master/platform.c",
		"freetype-gl-master/texture-atlas.c",
		"freetype-gl-master/texture-font.c",
		"freetype-gl-master/texture-font.h",
		"freetype-gl-master/vector.c",
		"freetype-gl-master/freetype-gl.h",
		"freetype-gl-master/opengl.h",
		"freetype-gl-master/platform.h",
		"freetype-gl-master/texture-atlas.h",
		"freetype-gl-master/vec234.h",
		"freetype-gl-master/vector.h",
		"freetype-gl-master/distance-field.h",
		"freetype-gl-master/distance-field.c",
		"freetype-gl-master/utf8-utils.h",
		"freetype-gl-master/utf8-utils.c",
		"freetype-gl-master/edtaa3func.h",
		"freetype-gl-master/edtaa3func.c"
	}

	includedirs
	{
		"freetype-gl-master",
		"../freetype/freetype-2.10.2/include",
		"../Glad/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
        staticruntime "on"
        defines{
            "ER_BUILDING_ON_WINDOWS"
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