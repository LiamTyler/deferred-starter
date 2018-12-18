#include "window.hpp"
#include <iostream>

static void error_callback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

static int SW = 0;
static int SH = 0;
static GLFWwindow* glfw_window = nullptr;

namespace window {

	bool Init(const std::string& title, int width, int height) {
		glfwSetErrorCallback(error_callback);
		if (!glfwInit())
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!glfw_window) {
			glfwTerminate();
			return false;
		}
		SW = width; SH = height;

		// glfwSetKeyCallback(window, key_callback);

		// set opengl context to be current window, and load glaad
		glfwMakeContextCurrent(glfw_window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to gladLoadGLLoader" << std::endl;
			glfwTerminate();
			return false;
		}

		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

		glfwSwapInterval(1);

		return true;
	}

	void Free() {
		glfwDestroyWindow(glfw_window);
		glfwTerminate();
	}

	void swapBuffers() {
		glfwSwapBuffers(glfw_window);
	}

	void setShouldClose(bool b) {
		glfwSetWindowShouldClose(glfw_window, b ? GLFW_TRUE : GLFW_FALSE);
	}

	bool shouldClose() {
		return glfwWindowShouldClose(glfw_window);
	}

	int width() { return SW; }
	int height() { return SH; }
	GLFWwindow* handle() { return glfw_window; }

} // namespace window