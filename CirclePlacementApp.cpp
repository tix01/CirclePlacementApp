#include <iostream>
#include "IO/FileIO.h"
#include "Drawing/Drawing.h"
#include "Algorithm/Algorithms.h"
#include <locale>
#include <codecvt>
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <input.xml> <output.xml> <output.bmp>" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    std::string outputXmlFilename = argv[2];
    std::string outputBmpFilename = argv[3];
    InputData inputData;
    if (!FileIO::readInputFromFile(inputFilename, inputData)) {
        std::cerr << "Failed to read input from file." << std::endl;
        return 1;
    }
    std::vector<Circle> circles= inputData.getCircles();
    Zone drawingZone = inputData.getZone().getPlacementZone();
    std::vector<ExclusionZone> exclusionZones = inputData.getZone().getExclusionZones();
    Algorithms a;
    a.initialPlacement(circles, drawingZone, exclusionZones);
    a.optimizeDistribution(circles, drawingZone, exclusionZones, 1000);

    HDC hdc = CreateCompatibleDC(NULL);
    int width = drawingZone.getPlacementZone().getWidth(); 
    int height = drawingZone.getPlacementZone().getHeight(); 
    
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HGDIOBJ hOldBitmap = SelectObject(hdc, hBitmap);
    HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    RECT rect = { 0, 0, width, height };
    FillRect(hdc, &rect, hWhiteBrush);
    Drawing::DrawCircles(hdc, circles, exclusionZones, drawingZone);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wideStr = converter.from_bytes(outputBmpFilename);
    Drawing::SaveToBmpFile(hdc, hBitmap, hOldBitmap, width, height, wideStr);


    if (!FileIO::writeResultToFile(outputXmlFilename, circles)) {
        std::cerr << "Failed to write result to XML file." << std::endl;
        return 1;
    }

    return 0;
}
