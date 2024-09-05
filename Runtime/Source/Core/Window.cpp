#include "Window.h"

#include <iostream>
#include <chrono>

engine::Window::Window(const char* title, Vec2i resolution, bool resizable, bool vsync)
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		std::exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	window = glfwCreateWindow(resolution.x, resolution.y, title, nullptr, nullptr);

	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(-1);
	}

	glfwMakeContextCurrent(window);
	if (vsync)
		glfwSwapInterval(1);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window, (mode->width - windowWidth) / 2, (mode->height - windowHeight) / 2);
}

engine::Window::~Window()
{
	glfwMakeContextCurrent(window);
	glfwDestroyWindow(window);
}

// For calculating the time between present calls
std::chrono::high_resolution_clock::time_point previousPresentTime = std::chrono::high_resolution_clock::now();
float deltaTime = 0.0f;

void engine::Window::present()
{
	glfwSwapBuffers(window);

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTimeDuration = currentTime - previousPresentTime;
	previousPresentTime = currentTime;

	deltaTime = deltaTimeDuration.count();
}

void engine::Window::pollEvents()
{
	glfwMakeContextCurrent(window);
	glfwPollEvents();
}

void engine::Window::close()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

const bool engine::Window::getShouldClose()
{
	return glfwWindowShouldClose(window) == GLFW_TRUE;
}

const float engine::Window::getDeltaTime()
{
	return deltaTime;
}

const GLFWwindow* engine::Window::getWindowHandle()
{
	return window;
}

// Wrap the window libraries function loader implementation
void* glFunctionLoaderWrapper(const char* functionName) { return reinterpret_cast<void*>(glfwGetProcAddress(functionName)); }
void* (*engine::Window::getGLFunctionLoader())(const char*) { return &glFunctionLoaderWrapper; }
