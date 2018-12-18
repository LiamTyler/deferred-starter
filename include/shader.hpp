#pragma once

#include "common.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class Shader {
public:
	Shader();
	~Shader();
        
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;
    Shader(Shader&& shader);
    Shader& operator=(Shader&& shader);

	bool AttachShaderFromString(GLenum shaderType, const std::string& source);
	bool AttachShaderFromFile(GLenum shaderType, const std::string& fname);
	bool CreateAndLinkProgram();
	void Free();
	void Enable();
	void Disable();
	GLuint getUniform(const std::string& name) const;
	GLuint getAttribute(const std::string& name) const;

	GLuint getGLHandle() { return program_; }
	bool isLoaded() { return loaded_; }

private:
	GLuint program_;
	std::vector<GLuint> shaders_;
	bool loaded_;
};