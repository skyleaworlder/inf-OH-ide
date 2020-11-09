#include "bmp.hpp"

BMP bmp;

void test_bmp_read() {
    std::cout << "size: " << bmp.bmp_size << std::endl
            << "width: " << bmp.bmp_width << std::endl
            << "height: " << bmp.bmp_height << std::endl
            << "bit per pixel: " << bmp.bmp_bitPx << std::endl
            << std::endl;
    for (BYTE by : bmp.data) {
        std::cout << by << " ";
    }
}

int main() {
    std::string s = "tj-logo.bmp";
    bmp.BMP_read(s);
    test_bmp_read();
}
