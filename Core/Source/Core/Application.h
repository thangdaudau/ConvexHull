#pragma once
#include "Window.h"
#include <memory>
#include <iostream>

namespace Core {

	class Application
	{
	public:
		Application();
		~Application();
		void run();
		
		const std::unique_ptr<Window>& getWindow() const;
		virtual void onUpdate() = 0;
	private:
		void onWindowClose();

	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};
}