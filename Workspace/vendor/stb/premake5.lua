project "stb"
    kind "StaticLib"
    language "c"
    staticruntime "off"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

    files{
       "stb_image_resize.h",
       "stb_image_resize.cpp",
       "stb_image_write.h",
       "stb_image_write.cpp",
       "stb_image.cpp",
       "stb_image.h"
    }
