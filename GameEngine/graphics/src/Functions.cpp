#include "../includes/Functions.hpp"


namespace GameBoi { namespace graphics {

	void windowResize(GLFWwindow *window, int width, int height);


	void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
		Window * win = (Window*) glfwGetWindowUserPointer(window);
		win->m_Keys[key] = action != GLFW_RELEASE;
	}

	void	mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
		Window * win = (Window*) glfwGetWindowUserPointer(window);
		win->m_MouseButtons[button] = action != GLFW_RELEASE;
	}

	void	cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
		Window * win = (Window*) glfwGetWindowUserPointer(window);
		win->mx = xpos;
		win->my = ypos;
	}

	Window::Window(const char *title, int width, int height) {
		m_Title = title;
		m_Width = width;
		m_Height = height;
		if (!init())
			glfwTerminate();

		for (int i = 0; i < MAX_KEYS; i++) {
			m_Keys[i] = false; 
		}

		for (int i = 0; i < MAX_BUTTONS; i++) {
			m_MouseButtons[i] = false; 
		}
	}

	Window::~Window() {
		glfwTerminate();
	}

	bool	Window::init() {
		

		glfwSetErrorCallback(GameBoi::graphics::Window::error_callback);

		if(!glfwInit()) {
        	std::cout << "Failed to Init GLFW" << std::endl;
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    	glfwWindowHint(GLFW_SAMPLES, 4);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif
		
		m_Window = glfwCreateWindow(m_Width, m_Height, "OREWA BOMBERTJOMBA DESUNE!", NULL, NULL);
		if (m_Window == NULL) {
			std::cout << "Failed to create a GLFW WINDOW! :(" << std::endl;
			return false;
		}
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetWindowSizeCallback(m_Window, windowResize);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetCursorPosCallback(m_Window, cursor_position_callback);
		// glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //remove if needed for mouse
		glfwSwapInterval(0.0);


		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;
			return false;
    	}
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		std::cout << "opengl " << glGetString(GL_VERSION) << std::endl;
		return true;

	}

	bool	Window::isKeyPressed(unsigned int keycode) const {
		// TODO: log here
		if (keycode >= MAX_KEYS)
		return false;
		return m_Keys[keycode];
	}

	bool	Window::isMouseButtonPressed(unsigned int button) const {
		// TODO: log here
		if (button >= MAX_BUTTONS)
		return false;
		return m_MouseButtons[button];
	}

	void	Window::getMousePosition(double &x, double &y) const {
		x = mx;
		y = my;
	}

	void	Window::clear() const {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void	Window::update() {
		
		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
			std::cout << "OpenGl Error: " << error << std::endl;

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		
	}

	bool	Window::closed() const {
		return glfwWindowShouldClose(m_Window) == 1;
	}

	void	windowResize(GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void	Window::createObject() {
		float vertices[] = {
			// -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			// -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			// -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			// -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			// -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			// -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			// 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			// 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			// -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		
		unsigned int indices[] = {  
        	0, 1, 3, // first triangle
        	1, 2, 3  // second triangle
    	};

    	
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
    	// glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		// glEnableVertexAttribArray(2);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);

	}
	void	Window::bind() {
		glBindVertexArray(VAO);
	}

	void Window::error_callback(int error, const char* desc){
		std::cout << "ERROR: "<<  desc << std::endl;
	};

	
} }