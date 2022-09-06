include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "RabbitEngine"
	architecture "x86_64"
	startproject "Rabbitnut"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "RabbitEngine/vendor/Box2D"
	include "RabbitEngine/vendor/GLFW"
	include "RabbitEngine/vendor/Glad"
	include "RabbitEngine/vendor/imgui"
	include "RabbitEngine/vendor/yaml-cpp"
group ""

include "RabbitEngine"
include "Sandbox"
include "Rabbitnut"
