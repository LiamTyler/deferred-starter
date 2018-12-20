#include "resource_loader.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include <iostream>

namespace resource {

	bool loadModel(Model& model, const std::string& pathToOBJ, const std::string& materialDirectory) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> tinyobj_materials;
		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &tinyobj_materials, &err, pathToOBJ.c_str(), materialDirectory.c_str(), true);

		if (!err.empty()) {
			std::cerr << "Tinyobj warning: " << err << std::endl;
		}

		if (!ret)
			return false;

		model.Free();

		// In OBJ format, each triangle can have its own material. So, loop through all the materials that TinyOBJ
		// parsed, and for each one create a mesh containing all of the triangles that have that material. 
		for (int currentMaterialID = -1; currentMaterialID < (int)tinyobj_materials.size(); ++currentMaterialID) {

			// if currentMaterialID == -1, use the default mesh, otherwise use the one that tinyobj loaded
			Material currentMaterial;
			if (currentMaterialID != -1) {
				tinyobj::material_t& mat = tinyobj_materials[currentMaterialID];
				glm::vec3 ambient(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
				glm::vec3 diffuse(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
				glm::vec3 specular(mat.specular[0], mat.specular[1], mat.specular[2]);
				float shininess = mat.shininess;
				GLuint diffuseTex = -1;
				if (mat.diffuse_texname != "") {
					std::string fullTexPath = materialDirectory + mat.diffuse_texname;
					int nC, width, height;
					stbi_set_flip_vertically_on_load(true);
					unsigned char* pixels = stbi_load(fullTexPath.c_str(), &width, &height, &nC, 4);
					if (!pixels) {
						std::cout << "Failed to load diffuse texture: " << fullTexPath << std::endl;
						model.Free();
						return false;
					} else {
						glGenTextures(1, &diffuseTex);
						glBindTexture(GL_TEXTURE_2D, diffuseTex);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						stbi_image_free(pixels);
					}
				}

				currentMaterial = Material(ambient, diffuse, specular, shininess, diffuseTex);
			}

			// now find all of the triangles with this materialID, and build the mesh
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

							// give default texture coordinates, even if the mesh doesn't have tex coords
							// NOTE: you might want to change this so we don't waste space if the mesh doesn't
							// actually have tex coords. Or not though
							tinyobj::real_t tx = 0, ty = 0;
							if (idx.texcoord_index != -1) {
								tx = attrib.texcoords[2 * idx.texcoord_index + 0];
								ty = attrib.texcoords[2 * idx.texcoord_index + 1];
							}
							uvs.emplace_back(tx, ty);

							indices.push_back((unsigned int)indices.size());
						}
					}
				}
			}

			// create mesh and upload to GPU
			if (verts.size()) {
				Mesh currentMesh((int)verts.size(), (int)indices.size() / 3, &verts[0], &normals[0], &uvs[0], &indices[0]);
				model.meshes.push_back(std::move(currentMesh));
				model.materials.push_back(std::move(currentMaterial));
			}
		}
		return true;
	}

	bool loadShader(Shader& shader, const std::string& pathToVert, const std::string& pathToFrag) {
		shader.Free();
		if (shader.AttachShaderFromFile(GL_VERTEX_SHADER, pathToVert) && shader.AttachShaderFromFile(GL_FRAGMENT_SHADER, pathToFrag))
			return shader.CreateAndLinkProgram();

		return false;
	}

} // namespace resource