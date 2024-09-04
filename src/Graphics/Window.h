#pragma once

struct GLFWwindow;

namespace engine {
	class Window {
	public:
		Window(const char* title, int resX, int resY, bool resizable, bool vsync);
		~Window();

		void pollEvents();
		void present();

		bool getIsOpen();
		float getDeltaTime();
		void* (*getGLFunctionLoader())(const char*);
		GLFWwindow* getWindowHandle();

	private:
		GLFWwindow* window;
	};
}