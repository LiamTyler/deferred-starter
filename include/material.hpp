#pragma once

#include "common.hpp"
//#include "types/texture.h"

class Shader;
class Material {
public:
	Material(
		const glm::vec3& a = glm::vec3(0),
		const glm::vec3& d = glm::vec3(0),
		const glm::vec3& s = glm::vec3(0),
		float ns = 0);
		//Texture* diffuseTex = nullptr);
        
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    //Texture* diffuseTexture;
};
