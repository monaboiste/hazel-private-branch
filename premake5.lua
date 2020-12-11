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
IncludeDir["ImGuizmo"] = "Hazel/Vendor/ImGuizmo"
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

project "Hazel"
	location "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	warnings "Default"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazel/Src/hzpch.cpp"

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",

		"%{prj.name}/Vendor/stb/**.h",
		"%{prj.name}/Vendor/stb/**.cpp",
		
		"%{prj.name}/Vendor/glm/glm/**.hpp",
		"%{prj.name}/Vendor/glm/glm/**.inl",

		"%{prj.name}/Vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/Vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"%{prj.name}/Src",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}
	
	filter "files:Hazel/Vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		buildoptions "/experimental:external /external:I\"Vendor\\entt\\include\" /external:I\"Vendor\\spdlog\\include\" /external:W0"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",
		"Hazel/Vendor/glm/glm/**.hpp",
		"Hazel/Vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"Hazel/Vendor/spdlog/include",
		"Hazel/Src",
		"Hazel/Vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hazel"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"


project "Hazelnut"
	location "Hazelnut"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	warnings "Default"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",
		"Hazel/Vendor/glm/glm/**.hpp",
		"Hazel/Vendor/glm/glm/**.inl",
		
		"Hazel/Vendor/ImGuizmo/ImGuizmo.h",
		"Hazel/Vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"Hazel/Vendor/spdlog/include",
		"Hazel/Src",
		"Hazel/Vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.entt}",
	}

	links
	{
		"Hazel"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/experimental:external /external:I\"Vendor\\entt\\include\" /external:I\"Vendor\\spdlog\\include\" /external:W0"

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"
