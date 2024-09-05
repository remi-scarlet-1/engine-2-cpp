#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace engine {
	using Path = std::filesystem::path;
	using String = std::string;

	class Filesystem {
	public:
		static bool exists(const Path& path);

		static Path getExecutablePath();
		static Path getAssetPath();
		
		static String readFileToString(const Path& path);

	private:
		Filesystem() = default;
		~Filesystem() = default;
	};
}
