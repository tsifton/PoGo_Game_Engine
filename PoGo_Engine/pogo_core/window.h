#ifndef WINDOW_H_
#define WINDOW_H_

#include "window_config.h"

#include "glew.h"
#include "glfw3.h"

namespace Pogo
{
	class Window
	{
	public:
		Window(WindowConfiguration config);
		~Window();

		// Initializes a glfw window
		bool Initialize();
		bool IsOpen();
		void SwapBuffers();
		void PollEvents();

	private:
		static void GlfwErrorCallback(int error, const char* description);

		GLFWwindow* m_window{ nullptr };
		WindowConfiguration m_config;
	};
}

#endif // !WINDOW_H_