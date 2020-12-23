#ifndef BMP_HPP
#define BMP_HPP

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
    WORD bmp_bytePerLine;

    BYTE* data;

    void BMP_read(std::string bmp_name);
    void BMP_write(
        std::string bmp_name,
        BYTE* img_data,
        const int width,
        const int height,
        const int bitPerPixel
    );
};

#endif
