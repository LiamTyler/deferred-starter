#pragma once

#include "common.hpp"
#include "shader.hpp"
#include "model.hpp"

namespace resource {

	bool loadModel(Model& model, const std::string& pathToOBJ, const std::string& materialDirectory);

	bool loadShader(Shader& shader, const std::string& pathToVert, const std::string& pathToFrag);
	
} // namespace resource