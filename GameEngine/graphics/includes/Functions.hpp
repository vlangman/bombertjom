#pragma once

// #ifndef WINDOW_HPP
// #define WINDOW_HPP



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>


namespace GameBoi { namespace graphics {


#define MAX_KEYS 	1024
#define MAX_BUTTONS 32

	class Window {
	private:
		const char  *m_Title;
		int		m_Width, m_Height;
		GLFWwindow *m_Window;
		bool m_Closed();

		bool		m_Keys[MAX_KEYS];
		bool		m_MouseButtons[MAX_BUTTONS];
		double	mx, my;
		unsigned int VBO, VAO;

	public:
		Window(const char *name, int width, int height);
		~Window();
		bool	closed() const;
		void	update();
		void	clear() const;
		void	createObject();
		void	bind();
		static void error_callback(int error, const char* desc);
		

		bool	isKeyPressed(unsigned int keycode) const;
		bool	isMouseButtonPressed(unsigned int button) const;
		void	getMousePosition(double &x, double &y) const;

	private:
	// just keeping methods and functions separate
		bool	init();		
		friend  void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		friend  void	mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
		friend  void	cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

	};

} }

// #endif