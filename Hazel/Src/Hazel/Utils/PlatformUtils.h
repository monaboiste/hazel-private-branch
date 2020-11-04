#pragma once

#include <optional>
#include <string>

namespace Hazel {

	class FileDialog
	{
	public:
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
	};

}