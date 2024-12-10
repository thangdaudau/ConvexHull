#include "Window.h"
#include <cassert>
#include <iostream>




namespace Core {

	CallbackFn0 Dispatcher::s_windowCloseCallback = nullptr;
	CallbackFn3i Dispatcher::s_mouseButtonCallback = nullptr;
	CallbackFn2d Dispatcher::s_cursorPosCallback = nullptr;
	CallbackFn2d Dispatcher::s_scrollCallback = nullptr;
	CallbackFn4i Dispatcher::s_keyCallback = nullptr;


	Window* Window::Create(int width, int height, const std::string& title)
	{
		return new Window(width, height, title);
	}

	Window::Window(int width, int height, const std::string& title)
		:m_title(title), m_width(width), m_height(height)
	{
		init(width, height, title);
	}

	void Window::init(int width, int height, const std::string& title)
	{
		if (not Core::s_glfwInitialized) 
		{
			int success = glfwInit();
			assert(success && "Could not initialize GLFW!");
			s_glfwInitialized = true;
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		}
		m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1); // 60fps
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		assert(status && "Failed to initialize Glad!");
	}

	void Window::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void Window::shutDown() 
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::setWindowCloseCallback(const CallbackFn0& callback)
	{
		Dispatcher::s_windowCloseCallback = callback;
		glfwSetWindowCloseCallback(m_window, Dispatcher::windowCloseCallback);
	}

	void Window::setMouseButtonCallback(const CallbackFn3i& callback)
	{
		Dispatcher::s_mouseButtonCallback = callback;
		glfwSetMouseButtonCallback(m_window, Dispatcher::mouseButtonCallback);
	}

	void Window::setCursorPosCallback(const CallbackFn2d& callback)
	{
		Dispatcher::s_cursorPosCallback = callback;
		glfwSetCursorPosCallback(m_window, Dispatcher::cursorPosCallback);
	}

	void Window::setScrollCallback(const CallbackFn2d& callback)
	{
		Dispatcher::s_scrollCallback = callback;
		glfwSetScrollCallback(m_window, Dispatcher::scrollCallback);
	}

	void Window::setKeyCallback(const CallbackFn4i& callback)
	{
		Dispatcher::s_keyCallback = callback;
		glfwSetKeyCallback(m_window, Dispatcher::keyCallback);
	}

}