#include "STDM.hpp"

std::string lena_path = "LENA.BMP";
std::string logo_path = "LOGO.BMP";
const size_t lena_size = STDM::pic_width * STDM::pic_height;
const size_t logo_size = STDM::logo_width * STDM::logo_height;

/*
void test_imbed() {
    STDM stdm(1);

    double a[64] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
    double b[64];

    stdm.imbed(a, b, 1);
    for (int i = 0; i < 64; i++)
        std::cout << b[i] << " ";
}
*/

/*
void test_decode() {
    STDM stdm(4);

    //double a[64] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
    double a[64] = {1,2,3,4,5,6,7,8,24,25,26,27,28,29,30,31,32,33,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
    double b[64];

    stdm.imbed(a, b, 1);
    for (int i = 0; i < 64; i++)
        std::cout << b[i] << " ";
    std::cout << std::endl;
    std::cout << stdm.decode(b) << std::endl;
}
*/

void test_imbedAll() {
    BMP bmp_lena, bmp_logo;
    bmp_lena.BMP_read(lena_path);
    bmp_logo.BMP_read(logo_path);

    double* lena_inp = new double[lena_size];
    for (size_t idx = 0; idx < lena_size; ++idx)
        lena_inp[idx] = bmp_lena.data[idx];

    int* logo_data = new int[logo_size];
    int* logo_inp = new int[logo_size*8];
    for (size_t idx = 0; idx < logo_size; ++idx) {
        logo_data[idx] = bmp_logo.data[idx];
        int to_sepa = logo_data[idx];
        for (size_t k = 0; k < 8; ++k) {
            logo_inp[7-k+8*idx] = ((to_sepa >> k) & 0x01);
        }
    }

    double* lena_out = new double[lena_size];

    STDM stdm(0.25);
    stdm.imbedAll(lena_inp, lena_out, logo_inp);

    u_char* lena_out_uchar = new u_char[lena_size];
    for (size_t idx = 0; idx < lena_size; ++idx)
        lena_out_uchar[idx] = lena_out[idx];
    std::string lena_out_path = "LENA-bak.BMP";
    bmp_lena.BMP_write(
        lena_out_path, lena_out_uchar,
        STDM::pic_width, STDM::pic_height, bmp_lena.bmp_bitPx
    );
}

void test_decodeAll() {
    BMP bmp_lena, bmp_logo;
    bmp_lena.BMP_read(lena_path);
    bmp_logo.BMP_read(logo_path);

    double* lena_inp = new double[lena_size];
    for (size_t idx = 0; idx < lena_size; ++idx)
        lena_inp[idx] = bmp_lena.data[idx];
    int* logo_data = new int[logo_size / 8];
    int* logo_inp = new int[logo_size];
    for (size_t idx = 0; idx < logo_size / 8; ++idx) {
        logo_data[idx] = bmp_logo.data[idx];
        int to_sepa = logo_data[idx];
        for (size_t k = 0; k < 8; ++k) {
            logo_inp[7-k+8*idx] = ((to_sepa >> k) & 0x01);
        }
    }

    double* lena_out = new double[lena_size];
    std::cout << "aha" << std::endl;
    double stride;
    std::cin >> stride;
    STDM stdm(stride);
    stdm.imbedAll(lena_inp, lena_out, logo_inp);

    int* logo_out = new int[logo_size];
    stdm.decodeAll(lena_out, logo_out);

    u_char* lena_out_uchar = new u_char[lena_size];
    u_char* logo_out_uchar = new u_char[logo_size];
    for (size_t idx = 0; idx < lena_size; ++idx)
        lena_out_uchar[idx] = lena_out[idx];
    int2UChar(logo_out, logo_size, logo_out_uchar);

    std::string lena_out_path = "LENA-bak.BMP";
    bmp_lena.BMP_write(
        lena_out_path, lena_out_uchar,
        STDM::pic_width, STDM::pic_height, bmp_lena.bmp_bitPx
    );
    std::string logo_decode_path = "LOGO-decode.BMP";
    bmp_logo.BMP_write(
        logo_decode_path, logo_out_uchar,
        STDM::logo_width, STDM::logo_height, 1
    );

    std::cout << "pfa:" << stdm.calcuPfa(logo_inp, logo_out) << std::endl;
}

int main() {
    //test_imbed();
    //test_imbedAll();
    //test_decode();
    test_decodeAll();
    return 0;
}