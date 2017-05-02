#ifndef WINDOW_H_
#define WINDOW_H_

#include "ExportHeader.h"
#include "window_config.h"

#include "GL\glew.h"
#include "glfw3.h"

namespace Pogo
{
	class ENGINE_SHARED Window
	{
	public:
		Window(WindowConfiguration config);
		~Window();

		// Initializes a glfw window
		bool Initialize();
		GLFWwindow* Get();

	private:
		static void GlfwErrorCallback(int error, const char* description);

		GLFWwindow* m_window{ nullptr };
		WindowConfiguration m_config;
	};
}

#endif // !WINDOW_H_