#include "bmp.hpp"

#include <iostream>
#include <fstream>

// read till eof
std::ifstream& operator>>(
    std::ifstream& in,
    ByteString& bytes
) {
    while (!in.eof()) {
        bytes += in.get();
    }
    return in;
}

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

    fin >> _pixelData;
}

BMP::~BMP() {
    delete[] _colorTable;
}
