#include "engine.h"

#include "glfw3.h"
#include "application_listener.h"

namespace Pogo
{
	Engine::Engine(ApplicationListener & listener, EngineConfiguration config) 
		: 
		m_listener(listener), 
		m_config(config), 
		m_window(config)
	{
	}

	Engine::~Engine()
	{
		glfwTerminate();
	}

	bool Engine::Run()
	{
		if (!InitializeWindow()) return false;

		while (!glfwWindowShouldClose(m_window.Get()))
		{
			m_listener.Update();

			float ratio;
			int width, height;
			glfwGetFramebufferSize(m_window.Get(), &width, &height);
			ratio = width / (float)height;
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);

			m_listener.Draw();

			glfwSwapBuffers(m_window.Get());
			glfwPollEvents();
		}
		return true;
	}

	bool Engine::InitializeWindow()
	{
		return m_window.Initialize();
	}

}