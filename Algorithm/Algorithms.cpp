#include "Algorithms.h"

bool Algorithms::innerRadiusWithinBounds(const Zone& drawingZone, const Circle& circle) {
    double minX = drawingZone.getPlacementZone().getMinPoint().getX();
    double minY = drawingZone.getPlacementZone().getMinPoint().getY();
    double maxX = drawingZone.getPlacementZone().getMaxPoint().getX();
    double maxY = drawingZone.getPlacementZone().getMaxPoint().getY();


    if (circle.getCenter().getX() - circle.getInnerRadius() < minX ||
        circle.getCenter().getX() + circle.getInnerRadius() > maxX ||
        circle.getCenter().getY() - circle.getInnerRadius() < minY ||
        circle.getCenter().getY() + circle.getInnerRadius() > maxY) {
        return false;
    }

    return true;
}

bool Algorithms::outerRadiusOverlap(const std::vector<Circle>& circles) {
    for (size_t i = 0; i < circles.size(); ++i) {
        for (size_t j = i + 1; j < circles.size(); ++j) {
            double distance = std::hypot(circles[i].getCenter().getX() - circles[j].getCenter().getX(), circles[i].getCenter().getY() - circles[j].getCenter().getY());
            if (distance < circles[i].getOuterRadius() + circles[j].getOuterRadius()) {
                return true;
            }
        }
    }
    return false;
}

bool Algorithms::outerRadiusOverlap(const std::vector<Circle>& circles, const Circle& newCircle) {
    for (const Circle& circle : circles) {
        double distance = std::hypot(circle.getCenter().getX() - newCircle.getCenter().getX(), circle.getCenter().getY() - newCircle.getCenter().getY());
        if (distance < circle.getOuterRadius() + newCircle.getOuterRadius()) {
            return true;
        }
    }
    return false;
}

bool Algorithms::innerRadiusExclusionOverlap(const std::vector<Circle>& circles, const std::vector<ExclusionZone>& exclusionZones) {
    for (const auto& circle : circles) {
        for (const auto& exclusionZone : exclusionZones) {
            double distanceX = std::max(std::max(exclusionZone.getZone().getMinPoint().getX() - circle.getCenter().getX(), circle.getCenter().getX() - exclusionZone.getZone().getMaxPoint().getX()), 0.0);
            double distanceY = std::max(std::max(exclusionZone.getZone().getMinPoint().getY() - circle.getCenter().getY(), circle.getCenter().getY() - exclusionZone.getZone().getMaxPoint().getY()), 0.0);
            double distance = std::hypot(distanceX, distanceY);

           
            if (distance < circle.getInnerRadius()) {
                return true;
            }
        }
    }
    return false;
}

bool Algorithms::canPlaceCircles(const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones, const std::vector<Circle>& circles) {
    for (const auto& circle : circles) {
        if (!innerRadiusWithinBounds(drawingZone, circle)) {
            return false;
        }
    }
    if (outerRadiusOverlap(circles)) {
        return false;
    }
    if (innerRadiusExclusionOverlap(circles, exclusionZones)) {
        return false;
    }
    return true;
}

double Algorithms::computeUniformity(const std::vector<Circle>& circles) {
    if (circles.empty()) {
        return 0.0; 
    }

    double totalDistance = 0.0;

    for (size_t i = 0; i < circles.size(); ++i) {
        for (size_t j = i + 1; j < circles.size(); ++j) {
            double distance = std::hypot(circles[i].getCenter().getX() - circles[j].getCenter().getX(), circles[i].getCenter().getY() - circles[j].getCenter().getY());
            totalDistance += distance;
        }
    }

    
    double numPairs = (circles.size() * (circles.size() - 1)) / 2;
    return totalDistance / numPairs;
}

void Algorithms::optimizeDistribution(std::vector<Circle>& circles, const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones, size_t numIterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> xDistribution(static_cast<int>(drawingZone.getPlacementZone().getMinPoint().getX()), static_cast<int>(drawingZone.getPlacementZone().getMaxPoint().getX()));
    std::uniform_int_distribution<int> yDistribution(static_cast<int>(drawingZone.getPlacementZone().getMinPoint().getY()), static_cast<int>(drawingZone.getPlacementZone().getMaxPoint().getX()));

    double initialUniformity = computeUniformity(circles);

    for (size_t iteration = 0; iteration < numIterations; ++iteration) {
        std::vector<Circle> newCircles = circles;

        for (Circle& circle : newCircles) {
            double oldX = circle.getCenter().getX();
            double oldY = circle.getCenter().getY();

            circle.setCenter(xDistribution(gen), yDistribution(gen));
           

            if (!canPlaceCircles(drawingZone, exclusionZones, newCircles)) {
                circle.setCenter(oldX, oldY);
            }
        }

        double newUniformity = computeUniformity(newCircles);

        if (newUniformity < initialUniformity) {
            circles = newCircles;
            initialUniformity = newUniformity;
        }
    }
}

void Algorithms::initialPlacement(std::vector<Circle>& circles, const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones) {
    std::sort(circles.begin(), circles.end(), [](const Circle& a, const Circle& b) {
        return a.getOuterRadius() > b.getOuterRadius();
        });
    calculateTotalArea(circles, drawingZone, exclusionZones);
    double step = 1.0;  
    double currentX = drawingZone.getPlacementZone().getMinPoint().getX() + circles.front().getInnerRadius();
    double currentY = drawingZone.getPlacementZone().getMaxPoint().getY() - circles.front().getInnerRadius();
    std::vector<Circle> tempCircles;

    for (auto& circle : circles) {
        circle.setCenter(currentX, currentY);
        tempCircles.push_back(circle);

        int counter = 0;
        while (!canPlaceCircles(drawingZone, exclusionZones, tempCircles)) {
            currentX += step;

            

            if (currentX + circle.getInnerRadius() > drawingZone.getPlacementZone().getMaxPoint().getX()) {
                currentX = drawingZone.getPlacementZone().getMinPoint().getX() + circle.getInnerRadius();
                currentY -= step;
                tempCircles[tempCircles.size() - 1].setCenter(currentX, currentY);
            }

            tempCircles[tempCircles.size() - 1].setCenter(currentX, currentY);

            if (counter == drawingZone.getPlacementZone().getHeight() * drawingZone.getPlacementZone().getWidth()) {
                std::cout << "Error: Unable to place circles.";
                return;
            }

            counter++;
        }

        if (currentX + circle.getInnerRadius() > drawingZone.getPlacementZone().getMaxPoint().getX() && currentY - circle.getInnerRadius() < drawingZone.getPlacementZone().getMinPoint().getY()) {
            currentX = drawingZone.getPlacementZone().getMinPoint().getX() + circle.getInnerRadius();
            currentY -= step * 2;
            tempCircles.pop_back();
            continue;
        }

        circle.setCenter(currentX, currentY);
        currentX += circle.getInnerRadius() * 2;
        std::cout << circle.getCenter().getX() << "   " << circle.getCenter().getY() << '\n';
    }
    
}

bool Algorithms::calculateTotalArea(const std::vector<Circle>& circles, const Zone& drawingZone, const std::vector<ExclusionZone>& exclusionZones)
{
    double totalArea = drawingZone.getPlacementZone().getHeight() * drawingZone.getPlacementZone().getWidth();
    double exclusionAreas = 0.0;
    for (const auto& exclusionZone : exclusionZones) {
        exclusionAreas += exclusionZone.getZone().getHeight() * exclusionZone.getZone().getWidth();
    }
    double circleAreas = 0.0;
    for (auto& circle : circles) {
        circleAreas += M_PI * circle.getOuterRadius() * circle.getOuterRadius();
    }
    
    if (totalArea - exclusionAreas >= circleAreas) {
        return true;
    }

    return false;
}
