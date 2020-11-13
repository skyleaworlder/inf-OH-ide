#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <vector>
#include <cmath>
#include <cassert>

namespace DCT {

    extern std::vector<double> Quant_matrix_50;

    std::vector<double> DCT(
        std::vector<double> pixel_arr,
        const size_t N
    );

    std::vector<double> DCT_inverse(
        std::vector<double> pixel_arr,
        const size_t N,
        size_t q_level=50
    );

    int round(double input, bool unsign=1);

    std::vector<double> Quant_matrix_gen(
        size_t target_Q
    );

    std::pair<
        std::vector<double>,
        std::vector<double>
    > T_matrix_gen(const size_t N);

    std::vector<double> Mult_square_matrix(
        std::vector<double> a,
        std::vector<double> b
    );
}