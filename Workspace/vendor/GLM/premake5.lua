project "GLM"
    kind "StaticLib"
    language "c++"
    staticruntime "on"

    targetdir("../../".."bin/" .. outputdir .. "/%{prj.name}")
    objdir("../../".."bin-int/" .. outputdir .. "/%{prj.name}")

    files{
		"glm/**.hpp",
		"glm/**.inl"
    }

    includedirs { 
		
   }
