project "Glad"
    kind "StaticLib"
    language "c"
    staticruntime "off"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

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

      