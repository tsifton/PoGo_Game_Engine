#include "window.h"

#include <stdio.h>

namespace Pogo
{
	Window::Window(WindowConfiguration config) : m_config(config)
	{

	}

	Window::~Window()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
	}

	bool Window::Initialize()
	{

		glfwSetErrorCallback(Window::GlfwErrorCallback);

		// If glfw fails to init, the error callback will handle the error logging
		if (!glfwInit()) return false;

		// Warning! All hints must come before window creation
		// hint to GLFW to maximize the created window
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		// hint to GLFW to use extra debugging
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		// hint to GLFW to use MSAAx16 (multisample anti-aliasing)
		glfwWindowHint(GLFW_SAMPLES, 16);

		// If glfw fails to create a window, the error callback will handle the error logging
		m_window = glfwCreateWindow(m_config.width, m_config.height, m_config.title, NULL, NULL);
		if (!m_window) return false;

		// Makes this window the current context
		glfwMakeContextCurrent(m_window);

		GLenum initResult = glewInit();
		if (initResult != GLEW_OK)
		{
			// TODO: Add logging
			printf("GLEW ERROR[%d]: GLEW was unable to initialize for the current context.", initResult);
			return false;
		}

		float ratio;
		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapInterval(1);

		return true;
	}

	bool Window::IsOpen()
	{
		return !glfwWindowShouldClose(m_window);
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::GlfwErrorCallback(int error, const char * description)
	{
		// TODO: Add logging of errors
		printf("GLFW Error[%d]: [%s]\n", error, description);
	}
}