#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Core {
	static bool s_glfwInitialized = false;

	template<typename... T>
	using CallbackFn = std::function<void(T...)>;

	using CallbackFn0 = CallbackFn<>;
	using CallbackFn1i = CallbackFn<int>;
	using CallbackFn2i = CallbackFn<int, int>;
	using CallbackFn3i = CallbackFn<int, int, int>;
	using CallbackFn4i = CallbackFn<int, int, int, int>;
	using CallbackFn2d = CallbackFn<double, double>;

	struct Dispatcher
	{
		static CallbackFn0 s_windowCloseCallback;
		static void windowCloseCallback(GLFWwindow* window) 
		{
			s_windowCloseCallback();
		}

		static CallbackFn3i s_mouseButtonCallback;
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
		{
			s_mouseButtonCallback(button, action, mods);
		}

		static CallbackFn2d s_cursorPosCallback;
		static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
		{
			s_cursorPosCallback(xpos, ypos);
		}

		static CallbackFn2d s_scrollCallback;
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
		{
			s_scrollCallback(xoffset, yoffset);
		}

		static CallbackFn4i s_keyCallback;
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
		{
			s_keyCallback(key, scancode, action, mods);
		}
	};


	class Window
	{
	public:
		static Window* Create(int width = 1600, int height = 900, const std::string& title = "Window");
	public:

		Window(int width, int height, const std::string& title);
		void onUpdate();
		void shutDown();
		// Params: empty
		void setWindowCloseCallback(const CallbackFn0& callback);
		// Params: int button, int action, int mods
		void setMouseButtonCallback(const CallbackFn3i& callback);
		// Params: double xpos, double ypos
		void setCursorPosCallback(const CallbackFn2d& callback);
		// Params: double xoffset, double yoffset
		void setScrollCallback(const CallbackFn2d& callback);
		// Params: int key, int scancode, int action, int mods
		void setKeyCallback(const CallbackFn4i& callback);
		
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		GLFWwindow* getNativeWindow() const { return m_window; }

	private:
		void init(int width, int height, const std::string& title);

	private:
		GLFWwindow* m_window;
		std::string m_title;
		int m_width;
		int m_height;
	};

}