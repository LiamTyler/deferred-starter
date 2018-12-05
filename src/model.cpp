#include "model.hpp"
#include "mesh.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#include <iostream>

Model::Model(const std::string& pathToOBJ, const std::string& materialDirectory) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, pathToOBJ.c_str(), materialDirectory.c_str(), true);

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		std::cout << "Failed to load the OBJ: " << pathToOBJ << std::endl;
		return;
	}

	for (int currentMaterialID = -1; currentMaterialID < (int)materials.size(); ++currentMaterialID) {
		std::shared_ptr<Material> currentMaterial;
		if (currentMaterialID == -1) {
			currentMaterial = ResourceManager::GetMaterial("default");
		}
		else {
			tinyobj::material_t& mat = materials[currentMaterialID];
			glm::vec3 ambient(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
			glm::vec3 diffuse(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
			glm::vec3 specular(mat.specular[0], mat.specular[1], mat.specular[2]);
			glm::vec3 emissive(mat.emission[0], mat.emission[1], mat.emission[2]);
			float shininess = mat.shininess;
			//Texture* diffuseTex = nullptr;
			//if (mat.diffuse_texname != "") {
			//	diffuseTex = new Texture(new Image(PG_RESOURCE_DIR + mat.diffuse_texname), true, true, true);
			//}

			currentMaterial = std::make_shared<Material>(ambient, diffuse, specular, emissive, shininess, diffuseTex);
		}

		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> indices;
		for (const auto& shape : shapes) {
			// Loop over faces(polygon)
			for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
				if (shape.mesh.material_ids[f] == currentMaterialID) {
					// Loop over vertices in the face. Each face should have 3 vertices from the LoadObj triangulation
					for (size_t v = 0; v < 3; v++) {
						tinyobj::index_t idx = shape.mesh.indices[3 * f + v];
						tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
						tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
						tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
						verts.emplace_back(vx, vy, vz);

						tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
						tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
						tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
						normals.emplace_back(nx, ny, nz);

						tinyobj::real_t tx = 0, ty = 0;
						if (idx.texcoord_index != -1) {
							tx = attrib.texcoords[2 * idx.texcoord_index + 0];
							ty = attrib.texcoords[2 * idx.texcoord_index + 1];
							uvs.emplace_back(tx, ty);
						}

						indices.push_back(indices.size());
					}
				}
			}
		}

		// create mesh and upload to GPU
		if (verts.size()) {
			// TODO: make this work for meshes that dont have UVS
			glm::vec2* texCoords = nullptr;
			if (uvs.size())
				texCoords = &uvs[0];
			auto currentMesh = new Mesh(verts.size(), indices.size() / 3, &verts[0], &normals[0], texCoords, &indices[0]);
			currentMesh->UploadToGPU(true, false);

			meshes.push_back(currentMesh);
			materials.push_back(currentMaterial);
		}
	}

	return model;
}

Model::~Model() {
	for (const auto& mesh : meshes)
		if (mesh)
			delete mesh;
	for (const auto& mat : materials)
		if (mat)
			delete mat;
}