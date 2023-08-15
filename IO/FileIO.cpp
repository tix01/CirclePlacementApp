#include "FileIO.h"

bool FileIO::readInputFromFile(const std::string& filename, InputData& inputData) {
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str())) {
        std::cerr << "Failed to load input XML file." <<"    " << filename<< std::endl;
        return false;
    }

    pugi::xml_node dataNode = doc.child("data");
    if (!dataNode) {
        std::cerr << "Invalid input XML format." << std::endl;
        return false;
    }

    pugi::xml_node placementZoneNode = dataNode.child("placement_zone");
    if (!placementZoneNode) {
        std::cerr << "Missing placement_zone node." << std::endl;
        return false;
    }

    // Чтение зоны расстановки
    pugi::xml_node rectNode = placementZoneNode.child("rect");
    if (!rectNode) {
        std::cerr << "Missing rect node." << std::endl;
        return false;
    }

    Point minPointRect(
        rectNode.child("min_point").attribute("x").as_double(),
        rectNode.child("min_point").attribute("y").as_double()
    );

    Point maxPointRect(
        rectNode.child("max_point").attribute("x").as_double(),
        rectNode.child("max_point").attribute("y").as_double()
    );

    Rectangle_ placementZone(minPointRect, maxPointRect);

    // Чтение зон исключения
    pugi::xml_node alarmsNode = placementZoneNode.child("alarms");
    if (!alarmsNode) {
        std::cerr << "Missing alarms node." << std::endl;
        return false;
    }

    std::vector<ExclusionZone> exclusionZones;
    for (pugi::xml_node alarmNode : alarmsNode.children("alarms")) {
        Point minPointAlarm(
            alarmNode.child("min_point").attribute("x").as_double(),
            alarmNode.child("min_point").attribute("y").as_double()
        );

        Point maxPointAlarm(
            alarmNode.child("max_point").attribute("x").as_double(),
            alarmNode.child("max_point").attribute("y").as_double()
        );

        Rectangle_ exclusionZone(minPointAlarm, maxPointAlarm);
        exclusionZones.push_back(exclusionZone);
    }



    Zone zone(placementZone, exclusionZones);

    // Чтение кругов
    pugi::xml_node circlesNode = dataNode.child("circles");
    if (!circlesNode) {
        std::cerr << "Missing circles node." << std::endl;
        return false;
    }

    std::vector<Circle> circles;
    for (pugi::xml_node circleNode = circlesNode.child("circle"); circleNode; circleNode = circleNode.next_sibling("circle")) {
        int id = circleNode.attribute("id").as_int();
        double innerRadius = circleNode.attribute("inner_rad").as_double();
        double outerRadius = circleNode.attribute("outter_rad").as_double();

        Circle circle(id, innerRadius, outerRadius, Point(0, 0)); // Временный центр, замените на действительный

        circles.push_back(circle);
    }

    inputData = InputData(zone, circles);
    return true;
}

bool FileIO::writeResultToFile(const std::string& filename, const std::vector<Circle>& circles) {
    pugi::xml_document doc;
    pugi::xml_node dataNode = doc.append_child("data");

    // Создание узла circles
    pugi::xml_node circlesNode = dataNode.append_child("circles");

    // Запись информации о каждом круге
    for (const Circle& circle : circles) {
        pugi::xml_node circleNode = circlesNode.append_child("circle");
        circleNode.append_attribute("id").set_value(circle.getId());
        circleNode.append_attribute("x").set_value(circle.getCenter().getX());
        circleNode.append_attribute("y").set_value(circle.getCenter().getY());
    }

    // Сохранение документа в файл
    if (!doc.save_file(filename.c_str())) {
        std::cerr << "Failed to save result XML file." << std::endl;
        return false;
    }

    return true;
}
