#pragma once
#include <vector>
#include <string>

#include "components/simple_scene.h"
#include "core/gpu/mesh.h"

namespace m1
{
    namespace MeshUtils
    {
        Mesh* CreateTerrain(const std::string& name, float size, float tile);
        Mesh* CreateQuad(const std::string& name);
    }
}
