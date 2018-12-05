#include "mesh.hpp"

Mesh::Mesh() :
    numTriangles(0)
{
    glGenBuffers(vboID::NUM_VBOS, vbos);
    glGenVertexArrays(1, &vao);
}

Mesh::Mesh(int numVerts, int numTris, glm::vec3* verts, glm::vec3* norms,
           glm::vec2* texCoords, unsigned int* indices)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(vboID::NUM_VBOS, vbos);

    UploadToGPU(numVerts, numTris, verts, norms, texCoords, indices);
}

Mesh::~Mesh() {
    if (vbos[0] != -1)
        glDeleteBuffers(vboID::NUM_VBOS, vbos);
    if (vao != -1)
        glDeleteVertexArrays(1, &vao);
}

Mesh::Mesh(Mesh&& mesh) {
    *this = std::move(mesh);
}

Mesh& Mesh::operator=(Mesh&& mesh) {
    numTriangles = mesh.numTriangles;
    mesh.numTriangles = 0;

    vao = mesh.vao;
    mesh.vao = -1;
    for (int i = 0; i < vboID::NUM_VBOS; ++i) {
        vbos[i] = mesh.vbos[i];
        mesh.vbos[i] = -1;
    }

    return *this;
}

void Mesh::UploadToGPU(int numVerts, int numTris, glm::vec3* verts,
        glm::vec3* norms, glm::vec2* texCoords, unsigned int* indices)
{
    numTriangles = numTris;
    glBindBuffer(GL_ARRAY_BUFFER, vbos[vboID::VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(glm::vec3), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[vboID::NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(glm::vec3), norms, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[vboID::UV]);
    glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(glm::vec2), texCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[vboID::INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * numTriangles * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}
