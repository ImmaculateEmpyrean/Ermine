project "Glad"
    kind "StaticLib"
    language "c"
    staticruntime "on"

    targetdir("%{wks.location}/bin/"  .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "src/glad.c",
        "include/khr/khrplatform.h",
        "include/glad/glad.h"
    }

    includedirs { 
        "include/",
        "src/"
   }
    
    filter "system:windows"
        systemversion "latest"

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