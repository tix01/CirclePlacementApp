#pragma once
#include <vector>
#include "../Geometry/geometry.h"

class ExclusionZone {
public:
    ExclusionZone(const Rectangle_& zone) : zone(zone) {}
    Rectangle_ getZone() const { return zone; }

private:
    Rectangle_ zone;
};

class Circle {
public:
    Circle(int id, double innerRadius, double outerRadius)
        : id(id), innerRadius(innerRadius), outerRadius(outerRadius), center(Point()) {}
    Circle(int id, double innerRadius, double outerRadius, const Point& otherCenter)
        : id(id), innerRadius(innerRadius), outerRadius(outerRadius), center(otherCenter) {}

    int getId() const { return id; }
    double getInnerRadius() const { return innerRadius; }
    double getOuterRadius() const { return outerRadius; }
    const Point& getCenter() const { return center; }
    void setCenter(const Point& point) { center = point; }
    void setCenter(const double x_, const double y_) { center.setX(x_); center.setY(y_); }

private:
    int id;
    double innerRadius;
    double outerRadius;
    Point center;
};

class Zone {
public:
    Zone(const Rectangle_& placementZone, const std::vector<ExclusionZone>& exclusionZones)
        : placementZone(placementZone), exclusionZones(exclusionZones) {}
    Zone() : placementZone(Rectangle_()), exclusionZones(std::vector<ExclusionZone>()) {}
    Zone(const Rectangle_& placementZone) :placementZone(placementZone), exclusionZones(std::vector<ExclusionZone>()) {}
    const Rectangle_& getPlacementZone() const { return placementZone; }
    const std::vector<ExclusionZone>& getExclusionZones() const { return exclusionZones; }

private:
    Rectangle_ placementZone;
    std::vector<ExclusionZone> exclusionZones;
};

class InputData {
public:
    InputData(const Zone& zone, const std::vector<Circle>& circles)
        : zone(zone), circles(circles) {}
    InputData() : zone(Zone()), circles(std::vector<Circle>()) {}

    const Zone& getZone() const { return zone; }
    std::vector<Circle>& getCircles() { return circles; }

private:
    Zone zone;
    std::vector<Circle> circles;
};
