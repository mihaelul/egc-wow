#pragma once
#include <glm/glm.hpp>
#include "tipuriStatii.h"

enum class RailType
{
    Normal,
    Bridge,
    Tunnel
};

enum class RailDir
{
    Forward,
    Backward,
    Left,
    Right
};

inline glm::vec3 DirToVector(RailDir dir)
{
    switch (dir)
    {
    case RailDir::Forward:  return glm::vec3(0, 0, 1);
    case RailDir::Backward: return glm::vec3(0, 0, -1);
    case RailDir::Left:     return glm::vec3(-1, 0, 0);
    case RailDir::Right:    return glm::vec3(1, 0, 0);
    }
    return glm::vec3(0, 0, 0);
}

struct RailSegment
{
    RailType type;
    RailDir dir;
    float length;
    glm::vec3 start;

    StationType station = StationType::None;


    std::vector<int> nextAtEnd;
    std::vector<bool> forwardAtEnd; 


    std::vector<int> nextAtStart;
    std::vector<bool> forwardAtStart;

    bool isIntersection = false;
};