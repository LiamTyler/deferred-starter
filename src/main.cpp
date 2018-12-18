#include <iostream>
#include "common.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "root_directory.hpp"
#include "camera.hpp"
#include "resource_loader.hpp"
#include "window.hpp"


void UploadMaterial(const Shader& shader, const Material& material) {
    glUniform3fv(shader.getUniform("ka"), 1, glm::value_ptr(material.ambient));
    glUniform3fv(shader.getUniform("kd"), 1, glm::value_ptr(material.diffuse));
    glUniform3fv(shader.getUniform("ks"), 1, glm::value_ptr(material.specular));
    glUniform1f(shader.getUniform("specular"), material.shininess);
    glUniform1i(shader.getUniform("textured"), false);

    // if (material.diffuseTexture) {
    //     glUniform1i(shader.getUniformLocation("textured"], true);
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, material.diffuseTexture->getGPUHandle());
    //     glUniform1i(shader.getUniformLocation("diffuseTex"], 0);
    // } else {
    //     glUniform1i(shader.getUniformLocation("textured"], false);
    // }
}

void RenderModel(const Camera& camera, const Shader& shader, const Model& model) {
	for (int i = 0; i < model.meshes.size(); ++i) {
		glBindVertexArray(model.meshes[i].vao);
		UploadMaterial(shader, model.materials[i]);
		glm::mat4 M = glm::mat4(1);
		glm::mat4 MV = camera.GetV() * M;
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(MV));
		glUniformMatrix4fv(shader.getUniform("modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
		glUniformMatrix4fv(shader.getUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glDrawElements(GL_TRIANGLES, model.meshes[i].numTriangles * 3, GL_UNSIGNED_INT, 0);
	}
}

int main() {
	window::Init();
    glViewport(0, 0, window::width(), window::height());
	glEnable(GL_DEPTH_TEST);

	std::string pathToVertexShader = ROOT_DIR "shaders/phong.vert";
	std::string pathToFragmentShader = ROOT_DIR "shaders/phong_forward.frag";
	Shader phongShader;
	if (!resource::loadShader(phongShader, pathToVertexShader, pathToFragmentShader)) {
		std::cout << "Failed to load the shader: " << pathToVertexShader << " + " << pathToFragmentShader << std::endl;
		exit(EXIT_FAILURE);
	}

	Model houseModel;
	if (!resource::loadModel(houseModel, ROOT_DIR "resources/cube.obj", ROOT_DIR "resources/")) {
		std::cout << "Failed to load the OBJ: " << ROOT_DIR "resources/cube.obj" << std::endl;
		exit(EXIT_FAILURE);
	}
	houseModel.materials[0] = Material(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(.7f), 100);
	houseModel.transform = Transform(glm::vec3(0), glm::vec3(0), glm::vec3(1));

	Camera camera = Camera(Transform(glm::vec3(0, 0, 10)));

	// Do this stupid hotfix for Mac + GLFW. The window doesn't draw correctly
	// until you move/resize the window, so just move it manually and back again
	// here to fix it.
	{
		int x, y;
		glfwGetWindowPos(window::handle(), &x, &y);
		glfwSetWindowPos(window::handle(), 100, 100);
		glfwSetWindowPos(window::handle(), x, y);
	}

	// Fix the mouse on the screen, and record first position
	glfwSetInputMode(window::handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glm::dvec2 lastCursorPos, currCursorPos;
	glfwGetCursorPos(window::handle(), &lastCursorPos.x, &lastCursorPos.y);
    while (!window::shouldClose()) {
		// Update internal input input state (keys, mouse, etc)
        glfwPollEvents();

		// Here is one method for polling key input from GLFW.
		// you can also use a key callback, which avoids the chance of missing
		// a keypress https://www.glfw.org/docs/latest/input_guide.html#input_key
		auto wHandle = window::handle();
		if (glfwGetKey(wHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			window::setShouldClose(true);

		if (glfwGetKey(wHandle, GLFW_KEY_A) == GLFW_PRESS)
			camera.velocity.x = -3;
		if (glfwGetKey(wHandle, GLFW_KEY_D) == GLFW_PRESS)
			camera.velocity.x = 3;
		if (glfwGetKey(wHandle, GLFW_KEY_A) == GLFW_RELEASE &&
			glfwGetKey(wHandle, GLFW_KEY_D) == GLFW_RELEASE)
			camera.velocity.x = 0;

		if (glfwGetKey(wHandle, GLFW_KEY_S) == GLFW_PRESS)
			camera.velocity.z = -3;
		if (glfwGetKey(wHandle, GLFW_KEY_W) == GLFW_PRESS)
			camera.velocity.z = 3;
		if (glfwGetKey(wHandle, GLFW_KEY_W) == GLFW_RELEASE &&
			glfwGetKey(wHandle, GLFW_KEY_S) == GLFW_RELEASE)
			camera.velocity.z = 0;

		glfwGetCursorPos(window::handle(), &currCursorPos.x, &currCursorPos.y);
		auto dPos = -0.003 * (currCursorPos - lastCursorPos);
		camera.transform.rotation.x += dPos.y;
		camera.transform.rotation.y += dPos.x;
		lastCursorPos = currCursorPos;


		// Update scene
		camera.Update(.016f);


		// Render scene
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phongShader.Enable();

        // Update lights and projection matrix
		glm::vec3 lightDirInWorldSpace = glm::vec3(0, 0, -1);
        glm::vec3 lightInViewSpace = glm::vec3(camera.GetV() * glm::vec4(lightDirInWorldSpace, 0));
        glUniform3fv(phongShader.getUniform("lightInViewSpace"), 1, glm::value_ptr(lightInViewSpace));
        glUniform3fv(phongShader.getUniform("lightColor"), 1, glm::value_ptr(glm::vec3(1.0f)));
        glUniformMatrix4fv(phongShader.getUniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetP()));

		RenderModel(camera, phongShader, houseModel);

        window::swapBuffers();
    }

	// Free up all of the resources
	phongShader.Free();
	window::Free();

    return 0;
}
