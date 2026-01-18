#include "tipuriStatii.h"
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include <vector>

using namespace std;


Mesh* CreatePyramid(const string& name)
{
    vector<VertexFormat> vertices =
    {
        {{ 0,  0.6f,  0}, {1, 0, 0}},   
        {{-0.5f, 0, -0.5f}, {1, 0, 0}},
        {{ 0.5f, 0, -0.5f}, {0, 1, 0}},
        {{ 0.5f, 0,  0.5f}, {0, 0, 1}},
        {{-0.5f, 0,  0.5f}, {1, 1, 0}}
    };

    vector<unsigned int> indices =
    {
        0,1,2,
        0,2,3,
        0,3,4,
        0,4,1,
        1,2,3,  1,3,4
    };

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}