#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRadialSquare(const std::string& name, glm::vec3 leftBottomCorner, float length);
    Mesh* CreateMotor(const std::string& name, glm::vec3 leftBottomCorner, float length);
    Mesh* CreateBumper(const std::string& name, glm::vec3 leftBottomCorner, float squareLength);
    Mesh* CreateTun(const std::string& name, glm::vec3 leftBottomCorner, float length);
    Mesh* CreateStart(const std::string& name, glm::vec3 leftBottomCorner, float length);
}
