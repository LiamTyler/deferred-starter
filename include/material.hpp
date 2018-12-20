#pragma once

#include "common.hpp"

class Material {
public:
	Material(
		const glm::vec3& a = glm::vec3(0),
		const glm::vec3& d = glm::vec3(0),
		const glm::vec3& s = glm::vec3(0),
		float ns = 0,
		GLuint diffuseTex = -1);
	~Material();

	// Prevent copies to avoid double deletion of diffuseTex. See note in mesh.hpp
	// for explanation + an alternative. 
	Material(const Material& shader) = delete;
	Material& operator=(const Material& shader) = delete;
	Material(Material&& shader);
	Material& operator=(Material&& shader);
        
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    GLuint diffuseTexture;
};
