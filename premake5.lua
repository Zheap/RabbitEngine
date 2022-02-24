workspace "RabbitEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

			--Debug/Release - windows	 - x64 --
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RabbitEngine"
	location "RabbitEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RB_PLATFORM_WINDOWS",
			"RB_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "RB_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "RB_RELEASE"
		symbols "On"
		
	filter "configurations:Dist"
		defines "RB_DIST"
		symbols "On"
		
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"RabbitEngine/vendor/spdlog/include",
		"RabbitEngine/src"
	}

	links
	{
		"RabbitEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RB_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "RB_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "RB_RELEASE"
		symbols "On"
		
	filter "configurations:Dist"
		defines "RB_DIST"
		symbols "On"