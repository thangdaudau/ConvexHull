project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { 
       "Source/**.h", 
       "Source/**.cpp",

       "vendor/stb/stb.cpp",
       --"vendor/glad/glad.c",
       --"vendor/imgui/**.cpp",
   }

   pchsource "stb.cpp"

   includedirs {
      "Source",
      "vendor",
   }

   libdirs {
        "lib"
   }

   links {
       "glfw3",
       "opengl32"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"