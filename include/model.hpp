#pragma once

#include <vector>
#include <string>
#include "mesh.hpp"
#include "material.hpp"
#include "transform.hpp"

// An OBJ model can have more than one mesh inside of it, so a "model"
// here is simply a collection of meshes, and their corresponding materials.
// Each mesh has one material
class Model {
public:
	Model() = default;

    // Deleting the copy constructors, since meshes cant be copied
    // (see mesh.hpp as to why)
    Model(const Model& mesh) = delete;
    Model& operator=(const Model& mesh) = delete;
    Model(Model&& mesh) = default;
    Model& operator=(Model&& mesh) = default;

	void Free();

    std::vector<Mesh> meshes;
    std::vector<Material> materials;
	Transform transform;
};
