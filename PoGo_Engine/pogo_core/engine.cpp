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

		while (m_window.IsOpen())
		{
			m_listener.Update();

			m_listener.Draw();

			m_window.SwapBuffers();
			m_window.PollEvents();
		}
		return true;
	}

	bool Engine::InitializeWindow()
	{
		return m_window.Initialize();
	}

}