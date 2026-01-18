#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "tipuriSine.h"

namespace sineRandare
{
    glm::vec3 CalculateRailEnd(const RailSegment& r);
    void BuildRailMap(std::vector<RailSegment>& railMap);
    glm::vec3 GetWorldHeading(const RailSegment& nextRail, bool entersForward);
    void BuildRailGraph(std::vector<RailSegment>& railMap, float eps = 0.35f);
    void MarkIntersections(std::vector<RailSegment>& railMap);
}