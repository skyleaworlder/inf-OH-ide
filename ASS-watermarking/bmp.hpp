#include <Windows.h>
#include <iostream>
#include <string>

using BYTE = u_char;

class BMP {

public:
    BMP() {};
    ~BMP() {};

public:
    BITMAPFILEHEADER bmp_hdr;
    BITMAPINFOHEADER bmp_infoHdr;
    RGBQUAD* color_tablePtr;

    DWORD bmp_size;
    WORD bmp_width;
    WORD bmp_height;
    WORD bmp_bitPx;

    void BMP_read(std::string bmp_name);
};