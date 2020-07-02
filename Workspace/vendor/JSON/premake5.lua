project "json"
    kind "StaticLib"
    language "c++"
    staticruntime "off"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

    files{
       "json-3.7.3/single_include/nlohmann/json.hpp",
       "jsondummy.cpp",
       "ErmineJsonFunctions.h",
       "ErmineJsonFunctions.cpp"
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