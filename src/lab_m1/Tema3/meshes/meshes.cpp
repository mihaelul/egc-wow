#include "lab_m1/Tema3/Meshes/meshes.h"
#include <glm/glm.hpp>

using namespace std;

namespace m1
{
    namespace MeshUtils
    {
        Mesh* CreateTerrain(const string& name, float size, float tile)
        {
            vector<glm::vec3> vertices
            {
                glm::vec3(-size, 0.0f, -size),
                glm::vec3(size, 0.0f, -size),
                glm::vec3(size, 0.0f,  size),
                glm::vec3(-size, 0.0f,  size),
            };

            vector<glm::vec3> normals
            {
                glm::vec3(0, 1, 0),
                glm::vec3(0, 1, 0),
                glm::vec3(0, 1, 0),
                glm::vec3(0, 1, 0),
            };

            vector<glm::vec2> textureCoords
            {
                glm::vec2(0.0f, 0.0f),
                glm::vec2(tile, 0.0f),
                glm::vec2(tile, tile),
                glm::vec2(0.0f, tile),
            };

            vector<unsigned int> indices =
            {
                0, 1, 2,
                2, 3, 0
            };

            Mesh* mesh = new Mesh(name);
            mesh->InitFromData(vertices, normals, textureCoords, indices);
            return mesh;
        }

        Mesh* CreateQuad(const string& name)
        {
            vector<glm::vec3> vertices =
            {
                glm::vec3(-0.5f, 0.0f, -0.5f),
                glm::vec3(0.5f, 0.0f, -0.5f),
                glm::vec3(0.5f, 0.0f,  0.5f),
                glm::vec3(-0.5f, 0.0f,  0.5f),
            };

            vector<glm::vec3> normals(4, glm::vec3(0, 1, 0));

            vector<glm::vec2> textureCoords =
            {
                glm::vec2(0, 0),
                glm::vec2(1, 0),
                glm::vec2(1, 1),
                glm::vec2(0, 1),
            };

            vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

            Mesh* mesh = new Mesh(name);
            mesh->InitFromData(vertices, normals, textureCoords, indices);
            return mesh;
        }
    }
}
