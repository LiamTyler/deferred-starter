#pragma once

#include "common.hpp"

class Mesh {
public:
    enum vboID : unsigned int {
        VERTEX,
        NORMAL,
        INDEX,
        UV,
        NUM_VBOS
    };


    Mesh();
    Mesh(int numVerts, int numTris, glm::vec3* verts, glm::vec3* norms,
         glm::vec2* uvs, unsigned int* indices);
    ~Mesh();

    // Disable copy constructors. The reason is if you did something like this:
	// Mesh mesh2 = mesh1;
	// Then you would have 1 copy of the data on the GPU, but two pointers (vao / vbos)
	// to the data. Then you have the following issue: who frees the data on the GPU?
	// You dont want them both the try freeing it, but you want someone to free it eventually.
	// You also probably dont want two copies on the GPU by doing a deep copy.
	// One solution below, is to just not allow copies to be made. Moving is fine, since then
	// mesh1 can be invalidated.
	// NOTE! This is a common problem potential problem with resources. This consideration could
	// largely be avoided just by making resource manager (highly recommended).
    Mesh(const Mesh& mesh) = delete;
    Mesh& operator=(const Mesh& mesh) = delete;
    Mesh(Mesh&& mesh);
    Mesh& operator=(Mesh&& mesh);

    void UploadToGPU(int numVerts, int numTris, glm::vec3* verts, glm::vec3* norms,
            glm::vec2* uvs, unsigned int* indices);

    unsigned int numTriangles;
    GLuint vao;
    GLuint vbos[vboID::NUM_VBOS];
};
