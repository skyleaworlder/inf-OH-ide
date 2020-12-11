#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath> // for sqrt
#include "ASS.hpp"

const std::array<int, 2> w { -1, 1 };

block_8_8 SS::imbed(
    block_8_8 block,
    double strength,
    SS::IMBED_B b,
    size_t zig_num=3
) {

    std::vector<double> vector_block = _array2vector(block);
    std::vector<double> DCT_block = DCT::DCT(vector_block, block.size());
    block_8_8 to_zig_zag = SS::_vector2array(DCT_block);

    for (auto arr : to_zig_zag) {
        for (auto elem : arr)
            std::cout << elem << " ";
        std::cout << std::endl;
    }

    std::vector<double> zig_zag_arr = SS::_getZigZagSlash(block, zig_num);

    // imbed watermark
    for (size_t index = 0; index < zig_zag_arr.size(); index++)
        zig_zag_arr[index] += strength * (b ? 1 : -1) * w[index % 2];

    // 将嵌入水印的部分填回原 block 中
    // left half
    size_t zig_zig_idx = 0;
    for (int zig_beg = zig_num; zig_beg > 0; zig_beg--)
        for (size_t index = 0; index <= to_zig_zag.size()-zig_beg; index++)
            to_zig_zag[index][to_zig_zag.size()-zig_beg-index] = zig_zag_arr[zig_zig_idx++];

    // right half
    for (int zig_beg = 1; zig_beg < zig_num; zig_beg++)
        for (size_t index = zig_beg; index < to_zig_zag.size(); index++)
            to_zig_zag[index][to_zig_zag.size()+zig_beg-1-index] = zig_zag_arr[zig_zig_idx++];

    std::vector<double> to_dct_inverse = SS::_array2vector(to_zig_zag);
    std::vector<double> to_ret = DCT::DCT_inverse(to_dct_inverse, block.size());

    return SS::_vector2array(to_ret);
}


bool SS::decode(
    block_8_8 block,
    size_t zig_num=3
) {
    std::vector<double> to_dct = SS::_array2vector(block);
    std::vector<double> DCT_block = DCT::DCT(to_dct, block.size());
    double L_s = 0;

    size_t cnt = 0;
    std::vector<double> zig_zag_arr = SS::_getZigZagSlash(block, zig_num);

    for (const auto& elem : zig_zag_arr)
        std::cout << elem << " ";

    for (size_t index = 0; index < zig_zag_arr.size(); index++) {
        L_s += zig_zag_arr[index] * w[index % 2];
        /* debug
        std::cout << "zig_zag_arr[index]: " << zig_zag_arr[index] << std::endl;
        std::cout << "w[index%2]: " << w[index % 2] << std::endl;
        std::cout << "L_s = " << L_s << std::endl;
        */
    }

    return (L_s >= 0);
}

std::vector<double> SS::_getZigZagSlash(
    block_8_8 input,
    size_t zig_num
) {
    std::vector<double> zig_zag_arr;
    // 取出需要嵌入水印的部分
    // left half
    for (int zig_beg = zig_num; zig_beg > 0; zig_beg--)
        for (size_t index = 0; index <= input.size()-zig_beg; index++)
            zig_zag_arr.push_back(input[index][input.size()-zig_beg-index]);

    // right half
    // block[index][block.size()+zig_beg-1-index] is very difficult for me to calculate.
    for (int zig_beg = 1; zig_beg < zig_num; zig_beg++)
        for (size_t index = zig_beg; index < input.size(); index++)
            zig_zag_arr.push_back(input[index][input.size()+zig_beg-1-index]);

    return zig_zag_arr;
}

block_8_8 SS::_vector2array(std::vector<double> input) {
    assert(input.size() == 8*8);
    block_8_8 to_ret;
    for (size_t index = 0; index < input.size(); index++) {
        size_t row = index / 8;
        size_t col = index - index / 8 * 8;
        to_ret[row][col] = input[index];
    }
    return to_ret;
}

std::vector<double> SS::_array2vector(block_8_8 input) {
    std::vector<double> to_ret;
    for (size_t row_idx = 0; row_idx < input.size(); row_idx++)
        for (size_t col_idx = 0; col_idx < input[row_idx].size(); col_idx++)
            to_ret.push_back(input[row_idx][col_idx]);
    return to_ret;
}