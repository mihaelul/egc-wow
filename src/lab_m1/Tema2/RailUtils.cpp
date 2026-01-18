#include "RailUtils.h"

glm::vec3 DirToVector(RailDir dir)
{
    switch (dir) {
    case RailDir::Forward:  return glm::vec3(0, 0, 1);
    case RailDir::Backward: return glm::vec3(0, 0, -1);
    case RailDir::Left:     return glm::vec3(-1, 0, 0);
    case RailDir::Right:    return glm::vec3(1, 0, 0);
    }
    return glm::vec3(0, 0, 1);
}

float DirToAngle(RailDir dir)
{
    switch (dir) {
    case RailDir::Forward:  return 0.0f;
    case RailDir::Right:    return RADIANS(90);
    case RailDir::Backward: return RADIANS(180);
    case RailDir::Left:     return RADIANS(-90);
    }
    return 0.0f;