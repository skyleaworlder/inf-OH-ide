#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <vector>
#include <cmath>
#include <cassert>

namespace DCT {

    std::vector<double> Quant_matrix_50 {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109,103,77,
        24, 35, 55, 64, 81, 104,113,92,
        49, 64, 78, 87, 103,121,120,101,
        72, 92, 95, 98, 112,100,103,99
    };

    std::vector<double> DCT(
        std::vector<double> pixel_arr,
        size_t N
    );

    std::vector<double> DCT_inverse(
        std::vector<double> pixel_arr,
        size_t q_level,
        size_t N
    );

    int round(double input, bool unsign=1);

    std::vector<double> Quant_matrix_gen(
        size_t target_Q
    );

    std::pair<
        std::vector<double>,
        std::vector<double>
    > T_matrix_gen(size_t N);
}