-- premake5.lua
-- 配置的工作区域是全局的，通用配置
workspace "RabbitEngine"        -- 设置工作区目录
    architecture "x64"          -- 设置项目使用的架构
    startproject "Rabbitnut"    -- 设置默认启动项目

    configurations              -- 配置，解决方案平台
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

-- 配置输出目录，cfg是上面的configurations的缩写
-- 拼接目录类似：Debug-windows-x86_64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "RabbitEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "RabbitEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "RabbitEngine/vendor/imgui"
IncludeDir["glm"] = "RabbitEngine/vendor/glm"
IncludeDir["stb_image"] = "RabbitEngine/vendor/stb_image"
IncludeDir["entt"] = "RabbitEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "RabbitEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "RabbitEngine/vendor/ImGuizmo"


group "Dependencies"
    include "RabbitEngine/vendor/GLFW"
    include "RabbitEngine/vendor/Glad"
    include "RabbitEngine/vendor/imgui"
    include "RabbitEngine/vendor/yaml-cpp"
group ""


-- 配置一个动态库项目：RabbitEngine
project "RabbitEngine"
    location "RabbitEngine"		-- 项目根目录
    kind "StaticLib"			-- 项目类型
    language "C++"
    cppdialect "C++17"				-- 使用C++ 17标准
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")		-- 生成的目标文件夹 bin/Debug-windows-x86_64/RabbitEngine, prj是project的缩写
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")		-- 中间临时文件生成目录

    pchheader "rbpch.h"
    pchsource "RabbitEngine/src/rbpch.cpp"

-- 当前项目源码目录
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",

        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
        "YAML_CPP_STATIC_DEFINE"
    }

-- 当前项目附加的包含库目录
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:RabbitEngine/vendor/ImGuizmo/**.cpp"
    flags { "NoPCH" }

    filter "system:windows"				-- 设置针对Windows平台的过滤器
        systemversion "latest"			-- 使用最新版本的系统sdk 比如Windows 10.0

        defines							-- 添加Windows平台上的预处理定义
        {
        }

    filter "configurations:Debug"		-- 配置Debug的过滤器
        defines "RB_DEBUG"				-- 添加Debug的预编译定义
        runtime "Debug"
        symbols "on"					-- 设置此宏有效
        
    filter "configurations:Release"
        defines "RB_RELEASE"
        runtime "Release"
        symbols "on"
        
    filter "configurations:Dist"
        defines "RB_DIST"
        runtime "Release"
        symbols "on"
        
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "RabbitEngine/src",
        "RabbitEngine/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}"
    }

-- 设置此项目引用RabbitEngine
    links
    {
        "RabbitEngine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "RB_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "RB_RELEASE"
        runtime "Release"
        symbols "on"
        
    filter "configurations:Dist"
        defines "RB_DIST"
        runtime "Release"
        symbols "on"

project "Rabbitnut"
    location "Rabbitnut"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "RabbitEngine/src",
        "RabbitEngine/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}"
    }

-- 设置此项目引用RabbitEngine
    links
    {
        "RabbitEngine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "RB_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "RB_RELEASE"
        runtime "Release"
        symbols "on"
        
    filter "configurations:Dist"
        defines "RB_DIST"
        runtime "Release"
        symbols "on"