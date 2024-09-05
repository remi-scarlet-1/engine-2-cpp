#include "Filesystem.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <unistd.h>
#include <limits.h>
#elif PLATFORM_MACOS
#include <mach-o/dyld.h>
#else
#error Unsupported platform. Please define PLATFORM_WINDOWS, PLATFORM_LINUX, or PLATFORM_MACOS.
#endif

using namespace engine;

bool Filesystem::exists(const Path& path)
{
	return std::filesystem::exists(path);
}

Path Filesystem::getExecutablePath()
{
#if PLATFORM_WINDOWS
    char path[MAX_PATH];
    if (GetModuleFileNameA(nullptr, path, MAX_PATH) == 0)
        throw std::runtime_error("Failed to get executable path");
    return Path(path);

#elif PLATFORM_LINUX
    char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len == -1)
        throw std::runtime_error("Failed to get executable path");
    path[len] = '\0';
    return Path(path);

#elif PLATFORM_MACOS
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0)
        throw std::runtime_error("Failed to get executable path");
    return Path(path);

#else
#error Unsupported platform
#endif
}

Path Filesystem::getAssetPath()
{
	return getExecutablePath().parent_path().append("Assets");
}

String Filesystem::readFileToString(const Path& path)
{
	if (!exists(path))
		throw std::runtime_error("File does not exist: " + path.string());

	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file)
		throw std::runtime_error("Failed to open file: " + path.string());

	return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}
