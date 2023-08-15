#pragma once
#include <string>
#include "../Objects/objects.h"
#include "../pugixml/pugixml.hpp"
#include <iostream>
class FileIO
{
public:
    static bool readInputFromFile(const std::string& filename, InputData& inputData);
    static bool writeResultToFile(const std::string& filename, const std::vector<Circle>& circles);
};

