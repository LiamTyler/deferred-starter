#include "shader.hpp"
#include <iostream>
#include <fstream>

Shader::Shader() :
	program_(-1),
	loaded_(false)
{
}

Shader::~Shader() {
	Free();
}

Shader::Shader(Shader&& shader) {
    *this = std::move(shader);
}

Shader& Shader::operator=(Shader&& shader) {
    program_ = shader.program_;
    shaders_ = std::move(shader.shaders_);
    loaded_ = shader.loaded_;
    shader.program_ = -1;

    return *this;
}

void Shader::Free() {
	if (program_ != -1)
		glDeleteProgram(program_);
}

bool Shader::AttachShaderFromString(GLenum shaderType, const std::string& source) {
	GLuint newShader = glCreateShader(shaderType);
	char const * sourcePointer = source.c_str();
	glShaderSource(newShader, 1, &sourcePointer, NULL);
	glCompileShader(newShader);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(newShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(newShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (!Result) {
		std::vector<char> ErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(newShader, InfoLogLength, NULL, &ErrorMessage[0]);
		printf("Error while loading shader:\n%s\n\n", &ErrorMessage[0]);
		return false;
	}

	shaders_.push_back(newShader);
	return true;
}

bool Shader::AttachShaderFromFile(GLenum shaderType, const std::string& filename) {
	std::ifstream in(filename);
	if (in.fail()) {
		std::cerr << "Failed to open the shader file: " << filename << std::endl;
		return false;
	}
	std::string file, line;
	while (std::getline(in, line))
		file += line + '\n';
	in.close();
	return AttachShaderFromString(shaderType, file);
}

bool Shader::CreateAndLinkProgram() {
	program_ = glCreateProgram();
	for (int i = 0; i < shaders_.size(); i++)
		glAttachShader(program_, shaders_[i]);

	glLinkProgram(program_);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(program_, GL_LINK_STATUS, &Result);
	glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (!Result) {
		std::vector<char> ErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(program_, InfoLogLength, NULL, &ErrorMessage[0]);
		printf("Error while compiling and linking program:\n%s\n\n", &ErrorMessage[0]);
		return false;
	}

	for (int i = 0; i < shaders_.size(); i++) {
		glDetachShader(program_, shaders_[i]);
		glDeleteShader(shaders_[i]);
	}
	shaders_.clear();
	loaded_ = true;
	return true;
}

void Shader::Enable() {
	glUseProgram(program_);
}

void Shader::Disable() {
	glUseProgram(0);
}

GLuint Shader::getUniform(const std::string& name) const {
	return glGetUniformLocation(program_, name.c_str());
}

GLuint Shader::getAttribute(const std::string& name) const {
	return glGetAttribLocation(program_, name.c_str());
}