project "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	warnings "Default"

	targetdir ("../Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../Bin-Int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Src/hzpch.cpp"

	files
	{
		"Src/**.h",
		"Src/**.cpp",
		"Vendor/stb/**.h",
		"Vendor/stb/**.cpp",
		"Vendor/glm/glm/**.hpp",
		"Vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"Src",
		"Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
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
