#include "shapes.h"

#include <vector>
#include <cmath>
#include <glm/glm.hpp>

using namespace std;

namespace train
{

    Mesh* CreateCylinder(const std::string& name,
        int resolution,
        float radius,
        float height)
    {
        vector<glm::vec3> vertices;
        vector<glm::vec3> normals;
        vector<unsigned int> indices;

        // centre
        int centerBottom = 0;
        int centerTop = 1;

        vertices.push_back(glm::vec3(0, 0, 0));
        normals.push_back(glm::vec3(0, -1, 0));

        vertices.push_back(glm::vec3(0, height, 0));
        normals.push_back(glm::vec3(0, 1, 0));

        int start = 2;

        // cerc jos
        for (int i = 0; i < resolution; i++) {
            float angle = 2 * M_PI * i / resolution;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            vertices.emplace_back(x, 0, z);
            normals.emplace_back(glm::normalize(glm::vec3(x, 0, z)));
        }

        // cerc sus
        for (int i = 0; i < resolution; i++) {
            float angle = 2 * M_PI * i / resolution;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            vertices.emplace_back(x, height, z);
            normals.emplace_back(glm::normalize(glm::vec3(x, 0, z)));
        }

        // laterale
        for (int i = 0; i < resolution; i++) {
            int next = (i + 1) % resolution;

            unsigned int b1 = start + i;
            unsigned int b2 = start + next;
            unsigned int t1 = start + i + resolution;
            unsigned int t2 = start + next + resolution;

            indices.insert(indices.end(), { b1, t1, b2 });
            indices.insert(indices.end(), { b2, t1, t2 });
        }

        // ===== CAPAC JOS =====
        for (int i = 0; i < resolution; i++) {
            int next = (i + 1) % resolution;

            indices.push_back(centerBottom);
            indices.push_back(start + next);
            indices.push_back(start + i);
        }

        // ===== CAPAC SUS =====
        for (int i = 0; i < resolution; i++) {
            int next = (i + 1) % resolution;

            indices.push_back(centerTop);
            indices.push_back(start + i + resolution);
            indices.push_back(start + next + resolution);
        }

        Mesh* mesh = new Mesh(name);
        mesh->InitFromData(vertices, normals, indices);
        return mesh;
    }

    Mesh* CreateWagon(const std::string& name,
        float height,
        float length)
    {
        vector<glm::vec3> vertices = {
            { 0.5f,  height, 0.0f },
            { 0.5f, -height, 0.0f },
            { -0.5f,-height, 0.0f },
            { -0.5f, height, 0.0f },

            { 0.5f,  height, length },
            { 0.5f, -height, length },
            { -0.5f,-height, length },
            { -0.5f, height, length }
        };

        vector<glm::vec3> normals(vertices.size(), glm::vec3(0, 1, 0));

        vector<unsigned int> indices = {
            0,1,3, 1,2,3,
            0,1,4, 1,4,5,
            3,2,7, 2,6,7,
            0,3,4, 3,7,4,
            1,5,2, 5,6,2,
            4,5,7, 5,6,7
        };

        Mesh* mesh = new Mesh(name);
        mesh->InitFromData(vertices, normals, indices);
        return mesh;
    }

    void CreateTrainMeshes(std::unordered_map<std::string, Mesh*>& meshes)
    {
        meshes["cilindru"] = CreateCylinder("cilindru", 32, 0.2f, 0.3f);
        meshes["vagon_verde"] = CreateWagon("vagon_verde", 0.5f, 3.0f);
        meshes["vagon_galben"] = CreateWagon("vagon_galben", 0.1f, 3.0f);
    }

    void CreateLocomotiveMeshes(std::unordered_map<std::string, Mesh*>& meshes)
    {
        float halfWidth = 0.5f;

        // CABINĂ VERDE (CUB)
        float greenHalf = 0.5f;   // cub 1x1x1
        float greenLength = 1.0f;

        // CORP ALBASTRU
        float blueHalfHeight = greenHalf; // jumătate din verde
        float blueLength = 2.0f;                  // dublu față de verde

        // ȘASIU
        float yellowHalfHeight = 0.1f;

        // ================= CABINĂ VERDE =================
        {
            vector<glm::vec3> vertices = {
                { 0.5f, greenHalf, 0.0f },
                { 0.5f, -greenHalf, 0.0f },
                { -0.5f,-greenHalf, 0.0f },
                { -0.5f, greenHalf, 0.0f },

                { 0.5f,  greenHalf, greenLength },
                { 0.5f, -greenHalf, greenLength },
                { -0.5f,-greenHalf,greenLength },
                { -0.5f, greenHalf, greenLength }
            };

            vector<glm::vec3> normals(vertices.size(), glm::vec3(0, 1, 0));

            vector<unsigned int> indices = {
                0,1,3, 1,2,3,
                0,1,4, 1,4,5,
                3,2,7, 2,6,7,
                0,3,4, 3,7,4,
                1,5,2, 5,6,2,
                4,5,7, 5,6,7
            };

            Mesh* mesh = new Mesh("loco_cabin");
            mesh->InitFromData(vertices, normals, indices);
            meshes["loco_cabin"] = mesh;
        }


        {
            int resolution = 32;

            float radius = blueHalfHeight-1.f;  
            float length = blueLength;     

            Mesh* mesh = CreateCylinder(
                "loco_body",
                resolution,
                radius,
                length
            );

            meshes["loco_body"] = mesh;
        }


        // ================= CORP mov (CILINDRU) =================
        {
            int resolution = 32;

            float radius = blueHalfHeight/4.f;  
            float length = blueLength+1.f;     

            Mesh* mesh = CreateCylinder(
                "center_cylinder_body",
                resolution,
                radius,
                length
            );

            meshes["center_cylinder_body"] = mesh;
        }

        // ================= ROTI =================
        {
            int resolution = 32;

            float radius = blueHalfHeight / 2.f; 
            float length = blueLength-1.f;    

            Mesh* mesh = CreateCylinder(
                "wheels_body",
                resolution,
                radius,
                length
            );

            meshes["wheels_body"] = mesh;
        }


   
        {
            vector<glm::vec3> vertices = {
                {  halfWidth,  yellowHalfHeight, 0 },
                {  halfWidth, -yellowHalfHeight, 0 },
                { -halfWidth, -yellowHalfHeight, 0 },
                { -halfWidth,  yellowHalfHeight, 0 },

                {  halfWidth,  yellowHalfHeight, blueLength },
                {  halfWidth, -yellowHalfHeight, blueLength },
                { -halfWidth, -yellowHalfHeight, blueLength },
                { -halfWidth,  yellowHalfHeight, blueLength }
            };

            vector<glm::vec3> normals(vertices.size(), glm::vec3(0, 1, 0));

            vector<unsigned int> indices = {
                0,1,3, 1,2,3,
                0,1,4, 1,4,5,
                3,2,7, 2,6,7,
                0,3,4, 3,7,4,
                1,5,2, 5,6,2,
                4,5,7, 5,6,7
            };

            Mesh* mesh = new Mesh("loco_chassis");
            mesh->InitFromData(vertices, normals, indices);
            meshes["loco_chassis"] = mesh;
        }

    }

    Mesh* CreateTerrainTile(const std::string& name, float size)
    {
        float half = size;
        float thickness = 0.001f;

        vector<glm::vec3> vertices = {
            {  half,  thickness, -half }, //3
            {  half, -thickness, -half },
            { -half, -thickness, -half },
            { -half,  thickness, -half },

            {  half,  thickness,  half },
            {  half, -thickness,  half },
            { -half, -thickness,  half },
            { -half,  thickness,  half }
        };

        vector<glm::vec3> normals(vertices.size(), glm::vec3(0, 1, 0));

        vector<unsigned int> indices = {
            0,1,3, 1,2,3,
            0,1,4, 1,4,5,
            3,2,7, 2,6,7,
            0,3,4, 3,7,4,
            1,5,2, 5,6,2,
            4,5,7, 5,6,7
        };

        Mesh* mesh = new Mesh(name);
        mesh->InitFromData(vertices, normals, indices);
        return mesh;
    }

    Mesh* CreateNormalRail(const std::string& name, float length)
    {
        float halfWidth = 0.3f;
        float halfHeight = 0.01f;

        vector<glm::vec3> vertices = {
            {  halfWidth,  halfHeight, 0 },
            {  halfWidth, -halfHeight, 0 },
            { -halfWidth, -halfHeight, 0 },
            { -halfWidth,  halfHeight, 0 },

            {  halfWidth,  halfHeight, length },
            {  halfWidth, -halfHeight, length },
            { -halfWidth, -halfHeight, length },
            { -halfWidth,  halfHeight, length }
        };

        vector<glm::vec3> normals(vertices.size(), glm::vec3(0, 1, 0));

        vector<unsigned int> indices = {
            0,1,3, 1,2,3,
            0,1,4, 1,4,5,
            3,2,7, 2,6,7,
            0,3,4, 3,7,4,
            1,5,2, 5,6,2,
            4,5,7, 5,6,7
        };

        Mesh* mesh = new Mesh(name);
        mesh->InitFromData(vertices, normals, indices);
        return mesh;
    }

    Mesh* CreateBridgeRail(const std::string& name, float length)
    {
        float railHeight = 0.02f;
        float railWidth = 0.05f;

        float outerHalfWidth = 0.3f;   
        float innerHalfWidth = 0.15f;  

        vector<glm::vec3> vertices;
        vector<glm::vec3> normals;
        vector<unsigned int> indices;

        auto AddBox = [&](glm::vec3 center, glm::vec3 halfSize)
            {
                unsigned int start = vertices.size();

             
                vertices.push_back(center + glm::vec3(halfSize.x, halfSize.y, -halfSize.z));
                vertices.push_back(center + glm::vec3(halfSize.x, -halfSize.y, -halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, halfSize.y, -halfSize.z));

                vertices.push_back(center + glm::vec3(halfSize.x, halfSize.y, halfSize.z));
                vertices.push_back(center + glm::vec3(halfSize.x, -halfSize.y, halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, -halfSize.y, halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, halfSize.y, halfSize.z));

                normals.insert(normals.end(), 8, glm::vec3(0, 1, 0));

                indices.insert(indices.end(), {
                    start + 0,start + 1,start + 3, start + 1,start + 2,start + 3,
                    start + 0,start + 1,start + 4, start + 1,start + 4,start + 5,
                    start + 3,start + 2,start + 7, start + 2,start + 6,start + 7,
                    start + 0,start + 3,start + 4, start + 3,start + 7,start + 4,
                    start + 1,start + 5,start + 2, start + 5,start + 6,start + 2,
                    start + 4,start + 5,start + 7, start + 5,start + 6,start + 7
                    });
            };

        float zCenter = length / 2.0f;


        AddBox(
            glm::vec3(-outerHalfWidth, 0, zCenter),
            glm::vec3(railWidth, railHeight, length / 2)
        );


        AddBox(
            glm::vec3(outerHalfWidth, 0, zCenter),
            glm::vec3(railWidth, railHeight, length / 2)
        );

        AddBox(
            glm::vec3(0, 0, 0),
            glm::vec3(outerHalfWidth, railHeight, railWidth)
        );

        AddBox(
            glm::vec3(0, 0, length),
            glm::vec3(outerHalfWidth, railHeight, railWidth)
        );

  
        AddBox(
            glm::vec3(0, 0, zCenter),
            glm::vec3(innerHalfWidth, railHeight, length / 2)
        );

        Mesh* mesh = new Mesh(name);
        mesh->InitFromData(vertices, normals, indices);
        return mesh;
    }

    Mesh* CreateTunnelRail(const std::string& name, float length)
    {
        float railThickness = 0.03f;

        float halfWidth = 0.3f;
        float halfHeight = 0.25f;

        int barsCount = 4; 

        vector<glm::vec3> vertices;
        vector<glm::vec3> normals;
        vector<unsigned int> indices;

        auto AddBox = [&](glm::vec3 center, glm::vec3 halfSize)
            {
                unsigned int start = vertices.size();

                vertices.push_back(center + glm::vec3(halfSize.x, halfSize.y, -halfSize.z));
                vertices.push_back(center + glm::vec3(halfSize.x, -halfSize.y, -halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, halfSize.y, -halfSize.z));

                vertices.push_back(center + glm::vec3(halfSize.x, halfSize.y, halfSize.z));
                vertices.push_back(center + glm::vec3(halfSize.x, -halfSize.y, halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, -halfSize.y, halfSize.z));
                vertices.push_back(center + glm::vec3(-halfSize.x, halfSize.y, halfSize.z));

                normals.insert(normals.end(), 8, glm::vec3(0, 1, 0));

                indices.insert(indices.end(), {
                    start + 0,start + 1,start + 3, start + 1,start + 2,start + 3,
                    start + 0,start + 1,start + 4, start + 1,start + 4,start + 5,
                    start + 3,start + 2,start + 7, start + 2,start + 6,start + 7,
                    start + 0,start + 3,start + 4, start + 3,start + 7,start + 4,
                    start + 1,start + 5,start + 2, start + 5,start + 6,start + 2,
                    start + 4,start + 5,start + 7, start + 5,start + 6,start + 7
                    });
            };

        float zCenter = length / 2.0f;

        // === cadru tunel ===
      

        // jos
        AddBox(
            glm::vec3(0, -halfHeight, zCenter),
            glm::vec3(halfWidth, railThickness, length / 2)
        );

       
        AddBox(
            glm::vec3(-halfWidth, 0, zCenter),
            glm::vec3(railThickness, halfHeight, length / 2)
        );

       
        AddBox(
            glm::vec3(halfWidth, 0, zCenter),
            glm::vec3(railThickness, halfHeight, length / 2)
        );


        for (int i = 1; i <= barsCount; i++)
        {
            float z = (length / (barsCount + 1)) * i;

            AddBox(
                glm::vec3(0, 0, z),
                glm::vec3(halfWidth, railThickness, railThickness)
            );
        }

        Mesh* mesh = new Mesh(name);
        mesh->InitFromData(vertices, normals, indices);
        return mesh;
    }



} 
