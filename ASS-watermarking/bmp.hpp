#include <cstddef>
#include <iostream>
#include <string>

using Byte = unsigned char;
using ByteString = std::basic_string<Byte>;

struct BitmapHeader {
    Byte signature[2];  // = "BM"
    uint32_t fileSize;
    Byte reserved[4];
    uint32_t dataOffset;
};

struct BitmapInfoHeader {
    uint32_t size;  // = 40
    uint32_t width, height;
    uint16_t planes;    // = 1
    uint16_t bitsPerPixel;
    uint16_t compression;
    uint16_t imageSize;
    uint16_t xPixelsPerM, yPixelsPerM;
    uint16_t colorsUsed;
    uint16_t importantColors;
};

struct ColorTable {
    Byte red, green, blue, reserved;
};

class BMP {
    BitmapHeader header;
    BitmapInfoHeader infoHeader;
    ColorTable colorTable;
    ByteString pixelData;
public:
    BMP() = default;
    ~BMP() = default;
};
