#include "bmp.hpp"
#include <string>
BMP bmp;

void test_bmp_read(std::string in, std::string out) {
    std::cout << "size: " << bmp.bmp_size << std::endl
            << "width: " << bmp.bmp_width << std::endl
            << "height: " << bmp.bmp_height << std::endl
            << "bit per pixel: " << bmp.bmp_bitPx << std::endl
            << std::endl;

    bmp.BMP_read(in);
    /*for (size_t i = 0; i < bmp.bmp_width * bmp.bmp_height; i++) {
        std::cout << int(bmp.data[i]) << " ";
    }*/
    bmp.BMP_write(out, bmp.data, bmp.bmp_width, bmp.bmp_height, bmp.bmp_bitPx);

}

int main() {
    test_bmp_read("LENA.BMP", "LENA-bak.BMP");
    test_bmp_read("tj-logo.bmp", "tj-logo-bak.bmp");
}
