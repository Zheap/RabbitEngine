-- premake5.lua
-- 配置的工作区域是全局的，通用配置
workspace "RabbitEngine"		-- 设置工作区目录
	architecture "x64"			-- 设置项目使用的架构
	startproject "Sandbox"		-- 设置默认启动项目

	configurations				-- 配置，解决方案平台
	{
		"Debug",
		"Release",
		"Dist"
	}

-- 配置输出目录，cfg是上面的configurations的缩写
-- 拼接目录类似：Debug-windows-x86_64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "RabbitEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "RabbitEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "RabbitEngine/vendor/imgui"

include "RabbitEngine/vendor/GLFW"
include "RabbitEngine/vendor/Glad"
include "RabbitEngine/vendor/imgui"


-- 配置一个动态库项目：RabbitEngine
project "RabbitEngine"
	location "RabbitEngine"		-- 项目根目录
	kind "SharedLib"			-- 项目类型
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")		-- 生成的目标文件夹 bin/Debug-windows-x86_64/RabbitEngine, prj是project的缩写
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")		-- 中间临时文件生成目录

	pchheader "rbpch.h"
	pchsource "RabbitEngine/src/rbpch.cpp"

-- 当前项目源码目录
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

-- 当前项目附加的包含库目录
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"				-- 设置针对Windows平台的过滤器
		cppdialect "C++17"				-- 使用C++ 17标准
		systemversion "latest"			-- 使用最新版本的系统sdk 比如Windows 10.0

		defines							-- 添加Windows平台上的预处理定义
		{
			"RB_PLATFORM_WINDOWS",
			"RB_BUILD_DLL",
		    "GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

		postbuildcommands				-- 编译后处理命令
		{
			-- 复制一份dll到Sandbox目录下   {COPY}命令
			-- %{cfg.buildtarget.relpath} = "bin/Debug-windows-x86_64/RabbitEngine/RabbitEngine.dll"
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"		-- 配置Debug的过滤器
		defines "RB_DEBUG"				-- 添加Debug的预编译定义
		runtime "Debug"
		symbols "On"					-- 设置此宏有效
		
	filter "configurations:Release"
		defines "RB_RELEASE"
		runtime "Release"
		symbols "On"
		
	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		symbols "On"
		
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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

-- 设置此项目引用RabbitEngine
	links
	{
		"RabbitEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"RB_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "RB_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "RB_RELEASE"
		runtime "Release"
		symbols "On"
		
	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		symbols "On"