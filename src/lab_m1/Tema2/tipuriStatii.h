#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

enum class StationType
{
    None,
    Cube,
    Sphere,
    Pyramid
};

struct Station
{
    StationType type;
    glm::vec3 position;
    float radius;
};

