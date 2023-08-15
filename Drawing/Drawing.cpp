#include "Drawing.h"

void Drawing::DrawCircles(HDC hdc, const std::vector<Circle>& circles, const std::vector<ExclusionZone>& exclusionZones, const Zone& drawingZone)
{
	BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    int transparency = 128;
    blend.SourceConstantAlpha = static_cast<BYTE>(transparency);
    blend.AlphaFormat = AC_SRC_ALPHA;

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    
    for (const auto& circle : circles)
    {
        
        double x_,y_;
        if (circle.getCenter().getY() < 0) {
            y_ = abs(circle.getCenter().getY()) + 100;
        }
        else {
            y_ = 100-abs(circle.getCenter().getY());
           
        }
        if (drawingZone.getPlacementZone().getMinPoint().getX() < 0) {
            x_ = drawingZone.getPlacementZone().getWidth() / 2;

        }
        else {
            x_= 0;
        }
        Ellipse(hdc,
            circle.getCenter().getX() - circle.getOuterRadius() + x_,
            y_ - circle.getOuterRadius(),
            circle.getCenter().getX() + circle.getOuterRadius() +x_,
            y_ + circle.getOuterRadius());
        Ellipse(hdc,
            circle.getCenter().getX() - circle.getInnerRadius() + x_,
            y_ - circle.getInnerRadius(),
            circle.getCenter().getX() + circle.getInnerRadius() + x_,
            y_ + circle.getInnerRadius());
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
    for (const auto& exclusionZone : exclusionZones) {

        double downY_, topY_, x_;
        if (exclusionZone.getZone().getMinPoint().getY() < 0) {
            downY_ = abs(exclusionZone.getZone().getMinPoint().getY()) + 100;
        }
        else {
            downY_ = 100 - abs(exclusionZone.getZone().getMinPoint().getY());
        }
        if (exclusionZone.getZone().getMaxPoint().getY() < 0) {
            topY_ = abs(exclusionZone.getZone().getMaxPoint().getY()) + 100;
        }
        else {
            topY_ = 100 - abs(exclusionZone.getZone().getMaxPoint().getY());
        }
        if (drawingZone.getPlacementZone().getMinPoint().getX() < 0) {
            x_ = drawingZone.getPlacementZone().getWidth() / 2;
        }
        else {
            x_ = 0;
        }
        Rectangle(hdc,
            exclusionZone.getZone().getMinPoint().getX() + x_,
            topY_,
            exclusionZone.getZone().getMaxPoint().getX() + x_,
            downY_);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
}

void Drawing::SaveToBmpFile(HDC hdc, HBITMAP hBitmap, HGDIOBJ hOldBitmap, int width, int height, const std::wstring& filename)
{
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height; 
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    DWORD dwBmpSize = ((width * 24 + 31) / 32) * 4 * height; 
    BYTE* pBmpData = new BYTE[dwBmpSize];
    GetDIBits(hdc, hBitmap, 0, height, pBmpData, &bmi, DIB_RGB_COLORS);

    HANDLE hFile = CreateFile(L"out.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        BITMAPFILEHEADER bmfh = {};
        bmfh.bfType = 0x4D42;
        bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmpSize;
        bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        DWORD dwBytesWritten;
        WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
        WriteFile(hFile, &bmi.bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
        WriteFile(hFile, pBmpData, dwBmpSize, &dwBytesWritten, NULL);

        CloseHandle(hFile);
    }

    SelectObject(hdc, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdc);

    delete[] pBmpData;
}







