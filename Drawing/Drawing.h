#pragma once
#include <vector>
#include "../Objects/objects.h"
#include <Windows.h>
#include <wingdi.h>
#include <iostream>
#pragma comment(lib, "gdiplus.lib")
class Drawing {
public:
    static void DrawCircles(HDC hdc, const std::vector<Circle>& circles, const std::vector<ExclusionZone>& exclusionZones, const Zone& drawingZone);
    static void SaveToBmpFile(HDC hdc, HBITMAP hBitmap, HGDIOBJ hOldBitmap, int width, int height, const std::wstring& filename);
};
