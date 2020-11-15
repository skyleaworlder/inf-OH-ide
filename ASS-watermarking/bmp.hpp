#include <cstddef>
#include <string>

using Byte = unsigned char;
using ByteString = std::basic_string<Byte>;

struct __attribute__ ((__packed__)) BitmapHeader {
    Byte        signature[2];  // = "BM"
    uint32_t    fileSize;
    Byte        reserved[4];
    uint32_t    dataOffset;
};

struct __attribute__ ((__packed__)) BitmapInfoHeader {
    uint32_t    size;  // = 40
    uint32_t    width, height;
    uint16_t    planes;    // = 1
    uint16_t    bitsPerPixel;
    uint32_t    compression;
    uint32_t    imageSize;
    uint32_t    xPixelsPerM, yPixelsPerM;
    uint32_t    colorsUsed;
    uint32_t    importantColors;
};

struct ColorTableItem {
    Byte red, green, blue, reserved;
};

class __attribute__ ((__packed__)) BMP {
    BitmapHeader        _header;
    BitmapInfoHeader    _infoHeader;
    ColorTableItem*          _colorTable;
    ByteString          _pixelData;
    unsigned int _numColors;
public:
    BMP(std::string bmpFilePath);
    ~BMP();
};
