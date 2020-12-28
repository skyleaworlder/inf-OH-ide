#ifndef DCT_HPP
#define DCT_HPP

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <vector>
#include <cmath>
#include <cassert>

namespace DCT {

    extern std::vector<double> Quant_matrix_50;

    /**
     * DCT 离散余弦变化
     * @name DCT
     * @param pixel_arr: vector<double> 输入
     * @param N: size_t pixel_arr size
     */
    std::vector<double> DCT(
        std::vector<double> pixel_arr,
        const size_t N
    );

    /**
     * DCT_inverse 离散余弦变化逆变换
     * @name DCT_inverse
     * @param pixel_arr: vector<double> 输入
     * @param N: size_t pixel_arr size
     * @param q_level: size_t 量化矩阵指数
     */
    std::vector<double> DCT_inverse(
        std::vector<double> pixel_arr,
        const size_t N,
        size_t q_level=50
    );

    /**
     * round 函数,根据 unsign 可以选择是否忽略负数
     * @name round
     * @param input: double, 输入数字
     * @param unsign: bool
     *      e.g. 如果 unsign 为 0, 那么输出是有负数的,
     *           如果 unsign 为 1, 那么负数变成 0
     */
    int round(double input, bool unsign=1);

    /**
     * 量化矩阵生成
     */
    std::vector<double> Quant_matrix_gen(
        size_t target_Q
    );

    /**
     * T 矩阵生成
     * @see dct.pdf(slides)
     */
    std::pair<
        std::vector<double>,
        std::vector<double>
    > T_matrix_gen(const size_t N);

    /**
     * 一个简单的矩阵乘法实现,
     * 但是只能处理两个相同大小的方阵
     */
    std::vector<double> Mult_square_matrix(
        std::vector<double> a,
        std::vector<double> b
    );
}

#endif