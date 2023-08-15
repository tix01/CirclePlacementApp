#pragma once
#include<cmath>
class Point {
public:
    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}

    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }

private:
    double x;
    double y;
};

class Rectangle_ {
public:
    Rectangle_(const Point& minPoint, const Point& maxPoint)
        : minPoint(minPoint), maxPoint(maxPoint) {
        width = abs(maxPoint.getX() - minPoint.getX());
        hight = abs(maxPoint.getY() - minPoint.getY());
    }
    Rectangle_() : minPoint(Point()), maxPoint(Point()),hight(0),width(0) {}
    const Point& getMinPoint() const { return minPoint; }
    const Point& getMaxPoint() const { return maxPoint; }
    const double& getHeight() const { return hight; }
    const double& getWidth() const { return width; }

private:
    Point minPoint;
    Point maxPoint;
    double hight;
    double width;
};
