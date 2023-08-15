#pragma once
#define _USE_MATH_DEFINES
#include "../Objects/objects.h"
#include <cmath>
#include <random>
#include <iostream>
class Algorithms {
public:
    bool innerRadiusWithinBounds(const Zone& drawingZone, const Circle& circle);
    bool outerRadiusOverlap(const std::vector<Circle>& circles);
    bool outerRadiusOverlap(const std::vector<Circle>& circles, const Circle& newCircle);
    bool innerRadiusExclusionOverlap(const std::vector<Circle>& circles, const std::vector<ExclusionZone>& exclusionZones);
    bool canPlaceCircles(const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones, const std::vector<Circle>& circles);
    double computeUniformity(const std::vector<Circle>& circles);
    void optimizeDistribution(std::vector<Circle>& circles, const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones, size_t numIterations);
    void initialPlacement(std::vector<Circle>& circles, const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones);
    bool calculateTotalArea(const std::vector<Circle>& circles, const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones);
};
