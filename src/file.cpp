#include "file.h"

#include <iostream>
#include <fstream>
#include <windows.h>

std::filesystem::path file::getExecutablePath() {
    // Try get exePath (using Windows OS function)
    char buffer[MAX_PATH];
    DWORD result = GetModuleFileNameA(NULL, buffer, sizeof(buffer));

    // Check if getting exePath succeded
    if (result == 0) {
        std::cerr << "Error getting executable path: " << GetLastError() << std::endl;
        return std::filesystem::path("");
    }

    // Return path
    return std::filesystem::path(buffer);
}

std::string file::readFileToString(const char* relativePath) {
    // Get exePath
    std::filesystem::path exePath = getExecutablePath();
    if (exePath.empty())
        return std::string("");

    // Construct absolute path
    std::filesystem::path fullPath = exePath.parent_path().append(relativePath);

    // Open the file
    std::ifstream file(fullPath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fullPath << std::endl;
        return "";
    }

    // Read the file content
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Return content as std::string
    return buffer.str();
}
