#pragma once

#include <GLFW/glfw3.h>

#include "Math.h"

namespace engine {
	class Window {
	public:
		Window(const char* title, Vec2i resolution, bool resizable, bool vsync);
		~Window();

		void present();

		const bool getIsOpen();
		const float getDeltaTime();
		const GLFWwindow* getWindowHandle();

		void* (*getGLFunctionLoader())(const char*);

	private:
		GLFWwindow* window;
	};
}