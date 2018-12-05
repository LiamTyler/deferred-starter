#include <iostream>
#include "common.hpp"
#include "shader.hpp"

const int SW = 640;
const int SH = 480;

std::string rootDirectory;

static void error_callback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	rootDirectory = "C:/Users/Tyler/Documents/deferred-starter/";

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(SW, SH, "Deferred Example", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to gladLoadGLLoader" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	glfwSwapInterval(1);
	glViewport(0, 0, SW, SH);
	glEnable(GL_DEPTH_TEST);

	Shader phongShader(rootDirectory + "shaders/phong.vert", rootDirectory + "shaders/phong_forward.frag");


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	}


	// Free up all of the resources
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}