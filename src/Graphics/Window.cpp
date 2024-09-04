#include "window.h"

#include <iostream>
#include <chrono>

#include "GLFW/glfw3.h"

engine::Window::Window(const char* title, int resX, int resY, bool resizable, bool vsync)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

    window = glfwCreateWindow(resX, resY, title, nullptr, nullptr);
    
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(window);
    if (vsync)
        glfwSwapInterval(1);
}

engine::Window::~Window()
{
    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
}

void engine::Window::pollEvents()
{
    glfwPollEvents();
}

// For calculating the time between present calls
std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
float deltaTime = 0.0f;

void engine::Window::present()
{
    glfwSwapBuffers(window);

    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTimeDuration = currentTime - previousTime;
    previousTime = currentTime;

    deltaTime = deltaTimeDuration.count();
}

bool engine::Window::getIsOpen()
{  
    return glfwWindowShouldClose(window) == GLFW_FALSE;
}

float engine::Window::getDeltaTime()
{
    return deltaTime;
}

// Wraps the libraries function loader implementation
void* glFunctionLoaderWrapper(const char* functionName) {
    return reinterpret_cast<void*>(glfwGetProcAddress(functionName));
}

void* (*engine::Window::getGLFunctionLoader())(const char*) {
    return &glFunctionLoaderWrapper;
}

GLFWwindow* engine::Window::getWindowHandle()
{
    return window;
}
