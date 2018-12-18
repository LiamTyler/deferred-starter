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
    //virtual ~Mesh();

    // Prevent copies from being made. Don't usually need deep copies ever,
    // and shallow copies bring up the issue of who frees up the VBOs
    // if multiple people have references to the same VBOs. Moving is fine though
    //Mesh(const Mesh& mesh) = delete;
    //Mesh& operator=(const Mesh& mesh) = delete;
    //Mesh(Mesh&& mesh);
    //Mesh& operator=(Mesh&& mesh);

    void UploadToGPU(int numVerts, int numTris, glm::vec3* verts, glm::vec3* norms,
            glm::vec2* uvs, unsigned int* indices);

    unsigned int numTriangles;
    GLuint vao;
    GLuint vbos[vboID::NUM_VBOS];
};
