project "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("../../bin/"..outputdir.."/%{prj.name}")
	objdir ("../../bin-int/"..outputdir.."/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
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