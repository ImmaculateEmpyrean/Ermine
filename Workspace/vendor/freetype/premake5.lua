project "Freetype"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
	objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")
    
    vpaths{
        ["FT_MODULES"] = {
            "freetype-2.10.2/src/base/ftbbox.c",
            "freetype-2.10.2/src/base/ftbdf.c",
            "freetype-2.10.2/src/base/ftbitmap.c",
            "freetype-2.10.2/src/base/ftcid.c",
            "freetype-2.10.2/src/base/ftfstype.c",
            "freetype-2.10.2/src/base/ftgasp.c",
            "freetype-2.10.2/src/base/ftglyph.c",
            "freetype-2.10.2/src/base/ftgxval.c",
            "freetype-2.10.2/src/base/ftmm.c",
            "freetype-2.10.2/src/base/ftotval.c",
            "freetype-2.10.2/src/base/ftpatent.c",
            "freetype-2.10.2/src/base/ftpfr.c",
            "freetype-2.10.2/src/base/ftstroke.c",
            "freetype-2.10.2/src/base/ftsynth.c",
            "freetype-2.10.2/src/base/fttype1.c",
            "freetype-2.10.2/src/base/ftwinfnt.c"
        }
    }
	files
	{
            "freetype-2.10.2/src/base/ftbbox.c",
            "freetype-2.10.2/src/base/ftbdf.c",
            "freetype-2.10.2/src/base/ftbitmap.c",
            "freetype-2.10.2/src/base/ftcid.c",
            "freetype-2.10.2/src/base/ftfstype.c",
            "freetype-2.10.2/src/base/ftgasp.c",
            "freetype-2.10.2/src/base/ftglyph.c",
            "freetype-2.10.2/src/base/ftgxval.c",
            "freetype-2.10.2/src/base/ftmm.c",
            "freetype-2.10.2/src/base/ftotval.c",
            "freetype-2.10.2/src/base/ftpatent.c",
            "freetype-2.10.2/src/base/ftpfr.c",
            "freetype-2.10.2/src/base/ftstroke.c",
            "freetype-2.10.2/src/base/ftsynth.c",
            "freetype-2.10.2/src/base/fttype1.c",
            "freetype-2.10.2/src/base/ftwinfnt.c",
        
        "freetype-2.10.2/src/autofit/autofit.c",
        "freetype-2.10.2/src/bdf/bdf.c",
        "freetype-2.10.2/src/cff/cff.c",
        "freetype-2.10.2/src/base/ftbase.c",
        "freetype-2.10.2/src/cache/ftcache.c",
        "freetype-2.10.2/builds/windows/ftdebug.c", -- Added The Ftdebug in windows folder as opposed to the one in base folder if that is a bad descision change it in the future :>
        "freetype-2.10.2/src/gzip/ftgzip.c",
        "freetype-2.10.2/src/base/ftinit.c",
        "freetype-2.10.2/src/lzw/ftlzw.c",
        "freetype-2.10.2/src/base/ftsystem.c",
        "freetype-2.10.2/src/pcf/pcf.c",
        "freetype-2.10.2/src/pfr/pfr.c",
        "freetype-2.10.2/src/psaux/psaux.c",
        "freetype-2.10.2/src/pshinter/pshinter.c",
        "freetype-2.10.2/src/psnames/psmodule.c",
        "freetype-2.10.2/src/raster/raster.c",
        "freetype-2.10.2/src/sfnt/sfnt.c",
        "freetype-2.10.2/src/smooth/smooth.c",
        "freetype-2.10.2/src/truetype/truetype.c",
        "freetype-2.10.2/src/type1/type1.c",
        "freetype-2.10.2/src/cid/type1cid.c",
        "freetype-2.10.2/src/type42/type42.c",
        "freetype-2.10.2/src/winfonts/winfnt.c"
    }
    
    includedirs { 
        "freetype-2.10.2/src",
        "freetype-2.10.2/include",
        "freetype-2.10.2bin/include/freetype/config",
        "freetype-2.10.2/bin/include"
    }

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
        staticruntime "on"
        defines{
            "ER_BUILDING_ON_WINDOWS",
            "WIN32",
            "_WINDOWS",
            "FT2_BUILD_LIBRARY",
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_NONSTDC_NO_WARNINGS",
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
    
    filter "platforms:Windows"
        defines{
            "ER_BUILDING_FOR_WINDOWS"
        }