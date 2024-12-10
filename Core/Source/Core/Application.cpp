#include "Application.h"

namespace Core {

	Application::Application()
	{
		m_window.reset(Window::Create());
		m_window->setWindowCloseCallback(
			[this]() {
				onWindowClose();
			}
		);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_running)
		{
			onUpdate();
			m_window->onUpdate();
		}
	}

	const std::unique_ptr<Window>& Application::getWindow() const
	{
		return m_window;
	}

	void Application::onWindowClose()
	{
		m_running = false;
	}
}
