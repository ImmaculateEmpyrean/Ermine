project "json"
    kind "StaticLib"
    language "c++"
    staticruntime "on"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

    files{
       "json-3.7.3/single_include/nlohmann/json.hpp",
       "jsondummy.cpp"
    }