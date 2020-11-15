#include "bmp.hpp"

#include <iostream>
#include <fstream>

BMP::BMP(
    std::string bmpFilePath
) {
    std::ifstream fin { bmpFilePath };
    fin.read(reinterpret_cast<char*>(&_header), 14);

    fin.read(reinterpret_cast<char*>(&_infoHeader), 40);
    _numColors = (1 << _infoHeader.bitsPerPixel);

    size_t colorTableSize { _header.dataOffset - 0x36 };
    _colorTable = new ColorTableItem[colorTableSize / 4];
    fin.read(reinterpret_cast<char*>(_colorTable), colorTableSize);
}

BMP::~BMP() {
    delete[] _colorTable;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Wrong Arguments." << std::endl;
    } else {
        BMP bmp {argv[1]};
    }
}

/*
void BMP::BMP_read(
    std::string bmp_name
) {

    FILE* fp = fopen(bmp_name.c_str(), "rb");
    if (fp == NULL) {
        std::cout << "open file failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "begin to read bmp file..." << std::endl;
    // get length of bmp file
    fseek(fp, 0, SEEK_END);
    size_t bmp_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fread(&bmp_hdr, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&bmp_infoHdr, sizeof(BITMAPINFOHEADER), 1, fp);

    // tmp use
    bmp_size = bmp_hdr.bfSize - sizeof(BITMAPFILEHEADER)
                - sizeof(BITMAPINFOHEADER);
    bmp_width = bmp_infoHdr.biWidth;
    bmp_height = bmp_infoHdr.biHeight;
    // it has to be noticed that bmp_BITpx, not bmp_BYTEpx!
    bmp_bitPx = bmp_infoHdr.biBitCount;

    /* bmp format says: bytes per line should be divided 4 exactly.
     * e.g.
     * byte per line without padding     padding     byte per line
     *               16                      0              16
     *               14                      2              16
     *               15                      1              16
     *               21                      3              24
     *
     * so byte_per_line_without_padding need +3
     // * /
    size_t byte_per_line_without_padding = bmp_bitPx * bmp_width / 8;
    size_t byte_per_line = (byte_per_line_without_padding+3) / 4 * 4;

    if (bmp_bitPx == 8) {
        color_tablePtr = new RGBQUAD[256];
        fread(color_tablePtr, sizeof(RGBQUAD), 256, fp);
    }
    else if (bmp_bitPx == 1) {
        color_tablePtr = new RGBQUAD[2];
        fread(color_tablePtr, sizeof(RGBQUAD), 2, fp);
    }
    else
        ;

    u_char* data_buf = new u_char[byte_per_line * bmp_height];
    fread(data_buf, 1, byte_per_line * bmp_height, fp);
    fclose(fp);

    for (size_t index = 0; index < byte_per_line * bmp_height; ++index)
        std::cout << int(data_buf[index]) <<  " ";

    return;
}
*/
