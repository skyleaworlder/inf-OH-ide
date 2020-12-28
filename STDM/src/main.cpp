#include "bmp.hpp"
#include "STDM.hpp"

#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>

void lab_3_1(
    const double delta, bool with_voice=false,
    const double mean=0, const double sigma=0,
    std::ostream& out=std::cout
) {

    // open pic & logo
    BMP bmp_lena, bmp_logo;
    std::string lena_path = "../assets/LENA.BMP";
    std::string logo_path = "../assets/LOGO.BMP";
    bmp_lena.BMP_read(lena_path);
    bmp_logo.BMP_read(logo_path);

    // about pic
    double* lena_inp = new double[STDM::pic_size];
    for (size_t idx = 0; idx < STDM::pic_size; ++idx)
        lena_inp[idx] = bmp_lena.data[idx];

    // about logo
    int* logo_data = new int[STDM::logo_size / 8];
    int* logo_inp = new int[STDM::logo_size];
    // bit operation spread logo from 2-val(u_char) to int*
    for (size_t idx = 0; idx < STDM::logo_size / 8; ++idx) {
        logo_data[idx] = bmp_logo.data[idx];
        int to_sepa = logo_data[idx];
        for (size_t k = 0; k < 8; ++k) {
            logo_inp[7-k+8*idx] = ((to_sepa >> k) & 0x01);
        }
    }

    // imbed
    STDM stdm(delta);
    double* lena_out = new double[STDM::pic_size];
    stdm.imbedAll(lena_inp, lena_out, logo_inp);

    if (with_voice)
        stdm.addVoice(lena_out, lena_out, mean, sigma, STDM::pic_size);

    // decode
    int* logo_out = new int[STDM::logo_size];
    stdm.decodeAll(lena_out, logo_out);

    // change int* => u_char*
    u_char* lena_out_uchar = new u_char[STDM::pic_size];
    for (size_t idx = 0; idx < STDM::pic_size; ++idx)
        lena_out_uchar[idx] = lena_out[idx];
    u_char* logo_out_uchar = new u_char[STDM::logo_size];
    int2UChar(logo_out, STDM::logo_size, logo_out_uchar);

    // output
    std::string lena_out_path = "../assets/LENA-bak.BMP";
    bmp_lena.BMP_write(
        lena_out_path, lena_out_uchar,
        STDM::pic_width, STDM::pic_height, bmp_lena.bmp_bitPx
    );
    std::string logo_decode_path = "../assets/LOGO-decode.BMP";
    bmp_logo.BMP_write(
        logo_decode_path, logo_out_uchar,
        STDM::logo_width, STDM::logo_height, bmp_logo.bmp_bitPx
    );

    out << delta << " " << mean << " " << sigma << " " << stdm.calcuPfa(logo_inp, logo_out) << std::endl;
}

// about JPEG
void lab_3_2(
    const double delta, const size_t q_level=50,
    std::ostream& out=std::cout
) {

    // open pic & logo
    BMP bmp_lena, bmp_logo;
    std::string lena_path = "../assets/LENA.BMP";
    std::string logo_path = "../assets/LOGO.BMP";
    bmp_lena.BMP_read(lena_path);
    bmp_logo.BMP_read(logo_path);

    // about pic
    double* lena_inp = new double[STDM::pic_size];
    for (size_t idx = 0; idx < STDM::pic_size; ++idx)
        lena_inp[idx] = bmp_lena.data[idx];

    // about logo
    int* logo_data = new int[STDM::logo_size / 8];
    int* logo_inp = new int[STDM::logo_size];
    // bit operation spread logo from 2-val(u_char) to int*
    for (size_t idx = 0; idx < STDM::logo_size / 8; ++idx) {
        logo_data[idx] = bmp_logo.data[idx];
        int to_sepa = logo_data[idx];
        for (size_t k = 0; k < 8; ++k) {
            logo_inp[7-k+8*idx] = ((to_sepa >> k) & 0x01);
        }
    }

    // imbed
    STDM stdm(delta);
    double* lena_out = new double[STDM::pic_size];
    stdm.imbedAll(lena_inp, lena_out, logo_inp, 3, q_level, true);

    // decode
    int* logo_out = new int[STDM::logo_size];
    stdm.decodeAll(lena_out, logo_out);

    // change int* => u_char*
    u_char* lena_out_uchar = new u_char[STDM::pic_size];
    for (size_t idx = 0; idx < STDM::pic_size; ++idx)
        lena_out_uchar[idx] = lena_out[idx];
    u_char* logo_out_uchar = new u_char[STDM::logo_size];
    int2UChar(logo_out, STDM::logo_size, logo_out_uchar);

    // output
    std::string lena_out_path = "../assets/LENA-bak.BMP";
    bmp_lena.BMP_write(
        lena_out_path, lena_out_uchar,
        STDM::pic_width, STDM::pic_height, bmp_lena.bmp_bitPx
    );
    std::string logo_decode_path = "../assets/LOGO-decode.BMP";
    bmp_logo.BMP_write(
        logo_decode_path, logo_out_uchar,
        STDM::logo_width, STDM::logo_height, bmp_logo.bmp_bitPx
    );

    out << delta << " " << q_level << " " << stdm.calcuPfa(logo_inp, logo_out) << std::endl;
}


int main() {
    int choice;
    double delta;
    int test_num;
    double a, b;
    size_t q_level;
    std::vector<double> sigma_vec;
    std::vector<double> delta_vec;
    std::vector<double> q_level_vec;
    std::ofstream out;

    std::cout << "(1) test normal STDM;" << std::endl;
    std::cout << "(2) test STDM with Guass-Voice;" << std::endl;
    std::cout << "(3) output pfa list, when delta=4,mean=1 (const), sigma variable;" << std::endl;
    std::cout << "(4) output pfa list, when mean=1,sigma=0 (const), delta variable;" << std::endl;
    std::cout << "(5) simulate normal JPEG compression;" << std::endl;
    std::cout << "(6) simulate JPEG compression, when delta=4 (const), q_level variable;" << std::endl;
    std::cout << "please input your choice: ";
    std::cin >> choice;

    switch (choice) {
    case 1:
        out.open("../data/normal_STDM.txt");
        std::cout << "please input delta of STDM(type unsigned double): ";
        std::cin >> delta;
        assert(delta > 0);
        lab_3_1(delta, false, 0, 0, out);
        out.close();
        break;

    case 2:
        out.open("../data/voice_STDM.txt");
        std::cout << "please input delta of STDM(delta > 0): ";
        std::cin >> delta;
        assert(delta > 0);
        std::cout << "please input mean and sigma of Guass-Dstr: ";
        double mean, sigma;
        std::cin >> mean >> sigma;
        assert(mean >= 0 && sigma >= 0);
        lab_3_1(delta, true, mean, sigma, out);
        out.close();
        break;

    case 3:
        out.open("../data/voice_STDM_sigma.txt");
        std::cout << "please input num of pfa test(num > 0): ";
        std::cin >> test_num;
        assert(test_num > 0);
        std::cout << "sigma will be in [a,b] (a < b && a,b > 0), please input a and b: ";
        std::cin >> a >> b;
        assert(a < b && a >= 0 && b >= 0);

        for (int cnt = 0; cnt < test_num; ++cnt)
            sigma_vec.push_back(a + cnt * ((b-a)/static_cast<double>(test_num)));
        for (int cnt = 0; cnt < test_num; ++cnt) {
            double mean = 1, delta = 4;
            lab_3_1(delta, true, mean, sigma_vec[cnt], out);
        }
        out.close();
        break;

    case 4:
        out.open("../data/voice_STDM_delta.txt");
        std::cout << "please input num of delta pfa test(num > 0): ";
        std::cin >> test_num;
        assert(test_num > 0);
        std::cout << "delta will be in [a,b] (a < b && a,b > 0), please input a and b: ";
        std::cin >> a >> b;
        assert(a < b && a >= 0 && b >= 0);

        for (int cnt = 0; cnt < test_num; ++cnt)
            delta_vec.push_back(a + cnt * ((b-a)/static_cast<double>(test_num)));
        for (int cnt = 0; cnt < test_num; ++cnt) {
            double mean = 1, sigma = 4;
            lab_3_1(delta_vec[cnt], true, mean, sigma, out);
        }
        out.close();
        break;

    case 5:
        out.open("../data/jpeg_STDM.txt");
        std::cout << "please input delta of STDM(delta > 0): ";
        std::cin >> delta;
        assert(delta);
        std::cout << "please input q_level of Quant_matrix(recommand 50): ";
        std::cin >> q_level;
        lab_3_2(delta, q_level, out);
        out.close();
        break;

    case 6:
        out.open("../data/jpeg_STDM_q_level.txt");
        delta = 4;
        std::cout << "please input num of jpeg compressioni test(num > 0): ";
        std::cin >> test_num;
        assert(test_num > 0);
        std::cout << "q_level will be in [a,b] (a < b && a,b > 0), please input a and b: ";
        std::cin >> a >> b;
        assert(a < b && a >= 0 && b >= 0);

        for (int cnt = 0; cnt < test_num; ++cnt)
            q_level_vec.push_back(a + cnt * ((b-a)/static_cast<double>(test_num)));
        for (int cnt = 0; cnt < test_num; ++cnt)
            lab_3_2(delta, q_level_vec[cnt], out);
        out.close();
        break;

    default:
        break;
    }
}