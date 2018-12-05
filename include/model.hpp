#pragma once

#include <vector>

class Mesh;
class Material;

class Model {
public:
    Model() = default;
	Model(const std::string& pathToOBJ, const std::string& materialDirectory);
	virtual ~Model();

	// Deleting the copy constructors, because if there were multiple copies, who would
	// be the single model to delete the mesh
    Model(const Model& mesh) = delete;
    Model& operator=(const Model& mesh) = delete;


    Model(Model&& mesh) = default;
    Model& operator=(Model&& mesh) = default;

    std::vector<Mesh> meshes;
    std::vector<Material> materials;
};