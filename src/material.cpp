#include "material.hpp"

Material::Material(const glm::vec3& a, const glm::vec3& d, const glm::vec3& s, float ns, GLuint diffuseTex) :
    ambient(a),
    diffuse(d),
	specular(s),
	shininess(ns),
    diffuseTexture(diffuseTex)
{
}

Material::~Material() {
	if (diffuseTexture != -1)
		glDeleteTextures(1, &diffuseTexture);
}

Material::Material(Material&& shader) {
	*this = std::move(shader);
}

Material& Material::operator=(Material&& shader) {
	ambient = std::move(shader.ambient);
	diffuse = std::move(shader.diffuse);
	specular = std::move(shader.specular);
	shininess = std::move(shader.shininess);
	diffuseTexture = std::move(shader.diffuseTexture);

	// set to -1 to avoid double deletion in destructor
	shader.diffuseTexture = -1;

	return *this;
}