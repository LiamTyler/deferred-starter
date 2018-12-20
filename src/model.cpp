#include "model.hpp"
#include "mesh.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#include <iostream>

void Model::Free() {
	materials.clear();
	meshes.clear();
}