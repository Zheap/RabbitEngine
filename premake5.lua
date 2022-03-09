-- premake5.lua
-- ���õĹ���������ȫ�ֵģ�ͨ������
workspace "RabbitEngine"		-- ���ù�����Ŀ¼
	architecture "x64"			-- ������Ŀʹ�õļܹ�
	startproject "Sandbox"		-- ����Ĭ��������Ŀ

	configurations				-- ���ã��������ƽ̨
	{
		"Debug",
		"Release",
		"Dist"
	}

-- �������Ŀ¼��cfg�������configurations����д
-- ƴ��Ŀ¼���ƣ�Debug-windows-x86_64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "RabbitEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "RabbitEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "RabbitEngine/vendor/imgui"

include "RabbitEngine/vendor/GLFW"
include "RabbitEngine/vendor/Glad"
include "RabbitEngine/vendor/imgui"


-- ����һ����̬����Ŀ��RabbitEngine
project "RabbitEngine"
	location "RabbitEngine"		-- ��Ŀ��Ŀ¼
	kind "SharedLib"			-- ��Ŀ����
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")		-- ���ɵ�Ŀ���ļ��� bin/Debug-windows-x86_64/RabbitEngine, prj��project����д
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")		-- �м���ʱ�ļ�����Ŀ¼

	pchheader "rbpch.h"
	pchsource "RabbitEngine/src/rbpch.cpp"

-- ��ǰ��ĿԴ��Ŀ¼
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

-- ��ǰ��Ŀ���ӵİ�����Ŀ¼
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

	filter "system:windows"				-- �������Windowsƽ̨�Ĺ�����
		cppdialect "C++17"				-- ʹ��C++ 17��׼
		systemversion "latest"			-- ʹ�����°汾��ϵͳsdk ����Windows 10.0

		defines							-- ���Windowsƽ̨�ϵ�Ԥ������
		{
			"RB_PLATFORM_WINDOWS",
			"RB_BUILD_DLL",
		    "GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

		postbuildcommands				-- �����������
		{
			-- ����һ��dll��SandboxĿ¼��   {COPY}����
			-- %{cfg.buildtarget.relpath} = "bin/Debug-windows-x86_64/RabbitEngine/RabbitEngine.dll"
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"		-- ����Debug�Ĺ�����
		defines "RB_DEBUG"				-- ���Debug��Ԥ���붨��
		runtime "Debug"
		symbols "On"					-- ���ô˺���Ч
		
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

-- ���ô���Ŀ����RabbitEngine
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