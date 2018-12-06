#include <iostream>
#include "common.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "root_directory.hpp"

const int SW = 1280;
const int SH = 720;

static void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void UploadMaterial(Shader& shader, const Material& material) {
    glUniform3fv(shader["ka"], 1, glm::value_ptr(material.ambient));
    glUniform3fv(shader["kd"], 1, glm::value_ptr(material.diffuse));
    glUniform3fv(shader["ks"], 1, glm::value_ptr(material.specular));
    glUniform1f(shader["specular"], material.shininess);
    glUniform1i(shader["textured"], false);

    // if (material.diffuseTexture) {
    //     glUniform1i(shader["textured"], true);
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, material.diffuseTexture->getGPUHandle());
    //     glUniform1i(shader["diffuseTex"], 0);
    // } else {
    //     glUniform1i(shader["textured"], false);
    // }
}

int main() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

    Shader phongShader(ROOT_DIR "shaders/phong.vert", ROOT_DIR "shaders/phong_forward.frag");

	Model houseModel(ROOT_DIR "resources/glowSphere.obj", ROOT_DIR "resources/");

	Material mat(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(.7f), 50);

    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), SW / (float) SH, 0.1f, 100.0f);

	// Do this stupid hotfix for Mac + GLFW. The window doesn't draw correctly
	// until you move/resize the window, so just move it manually and back again
	// here to fix it.
	{
		int x, y;
		glfwGetWindowPos(window, &x, &y);
		glfwSetWindowPos(window, 100, 100);
		glfwSetWindowPos(window, x, y);
	}

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phongShader.Enable();

        // Update lights and projection matrix
        glm::vec3 lightInViewSpace = glm::vec3(view * glm::vec4(glm::vec3(0, 0, -1), 0));
        glUniform3fv(phongShader["lightInViewSpace"], 1, glm::value_ptr(lightInViewSpace));
        glUniform3fv(phongShader["lightColor"], 1, glm::value_ptr(glm::vec3(1.0f)));
        glUniformMatrix4fv(phongShader["projectionMatrix"], 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(houseModel.meshes[0].vao);
		UploadMaterial(phongShader, mat);
        glm::mat4 M = glm::mat4(1);
        glm::mat4 MV = view * M;
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(MV));
        glUniformMatrix4fv(phongShader["modelViewMatrix"], 1, GL_FALSE, glm::value_ptr(MV));
        glUniformMatrix4fv(phongShader["normalMatrix"], 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glDrawElements(GL_TRIANGLES, houseModel.meshes[0].numTriangles * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }


    // Free up all of the resources
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
