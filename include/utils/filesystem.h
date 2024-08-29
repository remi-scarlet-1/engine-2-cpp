#pragma once

#include <filesystem>
#include <string>

namespace engine::utils {
	std::filesystem::path getExecutableDir();
	std::filesystem::path getAssetsDir();

	std::string loadFileToString(std::filesystem::path path);
}