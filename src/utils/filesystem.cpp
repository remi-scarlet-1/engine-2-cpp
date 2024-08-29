#include "utils/filesystem.h"

#include <iostream>
#include <fstream>
#include <windows.h>

std::filesystem::path engine::utils::getExecutableDir()
{
    // Get projects executable file dir by calling windows(os) function
    char buffer[MAX_PATH];
    DWORD result = GetModuleFileNameA(NULL, buffer, sizeof(buffer));

    if (result == 0) {
        std::cerr << "Error getting executable path: " << GetLastError() << std::endl;
        return std::filesystem::path("");
    }

    return std::filesystem::path(buffer);
}

std::filesystem::path engine::utils::getAssetsDir()
{
    return getExecutableDir().parent_path().append("assets");
}

std::string engine::utils::loadFileToString(std::filesystem::path path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return "";
    }

    // Accumulate file stream
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
