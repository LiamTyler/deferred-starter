#pragma once

#include <vector>
#include <string>
#include "mesh.hpp"
#include "material.hpp"


class Model {
public:
    Model() = default;
	Model(const std::string& pathToOBJ, const std::string& materialDirectory);
	virtual ~Model() = default;

    // Deleting the copy constructors, since meshes cant be copied
    // (see mesh.hpp as to why)
    Model(const Model& mesh) = delete;
    Model& operator=(const Model& mesh) = delete;
    Model(Model&& mesh) = default;
    Model& operator=(Model&& mesh) = default;

    std::vector<Mesh> meshes;
    std::vector<Material> materials;
};
