workspace "UniversumErminia"
    architecture "x86_64"
    startproject "Demiruge"

    configurations{
        "Debug_Development", --This is supposed to have things like tilemap editor and such imgui tools
        "Debug_Distribution", --This is to check distribution build with debug symbols included
        "Release_Distribution" --Use This Build To Ship
    }

    platforms{
        "Windows"
    }

    flags{
        "MultiprocessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.platform}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {} 

IncludeDir["Glad"]       ="%{wks.location}/vendor/Glad/include"
IncludeDir["GLFW"]       ="%{wks.location}/vendor/GLFW/glfw-3.3.2/include"
IncludeDir["ImGui"]      ="%{wks.location}/vendor/ImGuiDocking"

IncludeDir["GLM"]        ="%{wks.location}/vendor/GLM/glm"
IncludeDir["stb"]        ="%{wks.location}/vendor/stb"

IncludeDir["JSON"]       ="%{wks.location}/vendor/JSON/json-3.7.3/single_include"
IncludeDir["JSON_ERMINE"]="%{wks.location}/vendor/JSON"

IncludeDir["freetype"]   ="%{wks.location}/vendor/freetype/freetype-2.10.2/include"
IncludeDir["freetypegl"] ="%{wks.location}/vendor/freetype-gl"

IncludeDir["Box2D"]      ="%{wks.location}/vendor/box2d/include"

IncludeDir["tmxlite"]    = "%{wks.location}/vendor/tmxlite/tmxlite/include"
IncludeDir["tinyxml2"]   = "%{wks.location}/vendor/tinyxml2"
 
IncludeDir["lua"]        = "%{wks.location}/vendor/lua/lua-5.4.2/src"
IncludeDir["sol2"]       = "%{wks.location}/vendor/sol2/sol2-3.2.2/single/include"

IncludeDir["spdlog"]     = "%{wks.location}/vendor/spdlog/spdlog-1.x/include"

IncludeDir["Ermine"]     = "%{wks.location}/Ermine/include"
IncludeDir["ErmineSrc"]  = "%{wks.location}/Ermine/src"

group "Dependencies"
    include "vendor/Glad"
    include "vendor/GLFW"
    include "vendor/ImGuiDocking"

    include "vendor/GLM"
    include "vendor/stb"

    include "vendor/JSON"

    include "vendor/freetype"
    include "vendor/freetype-gl"

	include "vendor/box2d"
    
    include "vendor/tmxlite"
    include "vendor/tinyxml2"
    
	include "vendor/lua"
	include "vendor/sol2"
group ""

include "Ermine"

include "Demiruge"
include "Game"
