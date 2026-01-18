#pragma once

#include <unordered_map>
#include <string>

#include "core/gpu/mesh.h"

namespace train
{
   
    void CreateTrainMeshes(
        std::unordered_map<std::string, Mesh*>& meshes
    );


    Mesh* CreateCylinder(const std::string& name,
        int resolution,
        float radius,
        float height);

    Mesh* CreateWagon(const std::string& name,
        float height,
        float length);

    void CreateLocomotiveMeshes(
        std::unordered_map<std::string, Mesh*>& meshes
    );


    Mesh* CreateTerrainTile(const std::string& name, float size);


    Mesh* CreateNormalRail(const std::string& name, float length);

 
    Mesh* CreateBridgeRail(const std::string& name, float length);


    Mesh* CreateTunnelRail(const std::string& name, float length);


}
