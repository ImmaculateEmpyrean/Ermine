project "GLFW"
    kind "StaticLib"
    language "c"
    staticruntime "on"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "glfw-3.3.2/include/GLFW/glfw3.h",
        "glfw-3.3.2/include/GLFW/glfw3native.h",
        "glfw-3.3.2/src/glfw_config.h",
		"glfw-3.3.2/src/context.c",
		"glfw-3.3.2/src/init.c",
		"glfw-3.3.2/src/input.c",
		"glfw-3.3.2/src/monitor.c",
		"glfw-3.3.2/src/vulkan.c",
		"glfw-3.3.2/src/window.c"
    }

    includedirs { 
        "glfw-3.3.2/include/",
        "glfw-3.3.2/src/"
   }
        
    filter "system:windows"
        systemversion "latest"

        defines{
            "ER_BUILDING_ON_WINDOWS",
            "_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
        }
		systemversion "latest"
		staticruntime "On"

		files
		{
			"glfw-3.3.2/src/win32_init.c",
			"glfw-3.3.2/src/win32_joystick.c",
			"glfw-3.3.2/src/win32_monitor.c",
			"glfw-3.3.2/src/win32_time.c",
			"glfw-3.3.2/src/win32_thread.c",
			"glfw-3.3.2/src/win32_window.c",
			"glfw-3.3.2/src/wgl_context.c",
			"glfw-3.3.2/src/egl_context.c",
			"glfw-3.3.2/src/osmesa_context.c"
		}