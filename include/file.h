#pragma once

#include <string>
#include <filesystem>

namespace file {
	std::filesystem::path getExecutablePath();
	std::string readFileToString(const char* path);
}