#pragma once

#include <GLFW/glfw3.h>

#include "Math.h"

namespace engine {
	class Window { // TODO: Support for multiple windows?
	public:
		Window(const char* title, Vec2i resolution, bool resizable, bool vsync);
		~Window();

		void present();
		void pollEvents();
		void close();

		const bool getShouldClose();
		const float getDeltaTime();
		const GLFWwindow* getWindowHandle();

		void* (*getGLFunctionLoader())(const char*);

	private:
		GLFWwindow* window;
	};
}