workspace "Hazel"
	architecture "x64"
	startproject "Hazelnut"

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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/Vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/Vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/Vendor/imgui"
IncludeDir["glm"] = "Hazel/Vendor/glm"
IncludeDir["stb"] = "Hazel/Vendor/stb"
IncludeDir["entt"] = "Hazel/Vendor/entt/include"
IncludeDir["yaml"] = "Hazel/Vendor/yaml-cpp/include"

group "Dependencies"
	include "Hazel/Vendor/GLFW"
	include "Hazel/Vendor/Glad"
	include "Hazel/Vendor/imgui"
	include "Hazel/Vendor/yaml-cpp"
group ""

include "Hazel"
include "Hazelnut"
include "Sandbox"