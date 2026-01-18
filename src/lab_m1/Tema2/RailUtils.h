#pragma once
#include <glm/glm.hpp>

enum class RailDir;

glm::vec3 DirToVector(RailDir dir);
float DirToAngle(RailDir dir);