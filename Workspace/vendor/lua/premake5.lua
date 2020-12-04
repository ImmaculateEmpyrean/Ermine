project "lua"
    kind "StaticLib"
    language "c"
    staticruntime "off"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

    files{
       "lua-5.4.2/src/**"
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