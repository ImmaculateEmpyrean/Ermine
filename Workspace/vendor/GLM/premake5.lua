project "GLM"
    kind "StaticLib"
    language "c++"
    staticruntime "on"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

    files{
		"glm/**.hpp",
    "glm/**.inl",
    "GLMDummy.cpp"
    }

    includedirs {

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