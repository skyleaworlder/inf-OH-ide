#include "utils.hpp"

std::vector<double> getZigZagSlash(
    std::array<std::array<double, 8>, 8> input,
    const size_t zig_num
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

void int2UChar(int* int_inp, const size_t int_inp_size, u_char* uchar_out) {
    const size_t uchar_out_len = int_inp_size / 8;

    for (size_t idx = 0; idx < uchar_out_len; ++idx) {
        uchar_out[idx] = static_cast<u_char>(0);
        for (int cnt = 7; cnt >= 0; --cnt) {
            // small endian
            bool bit = int_inp[idx*8 + cnt];
            if (bit == 1) {
                u_char c_tmp = 0x01 << (7 - cnt);
                uchar_out[idx] |= c_tmp;
            }
        }
    }
}

std::array<std::array<double, 8>, 8> vector2Array(std::vector<double> input) {
    assert(input.size() == 8*8);
    std::array<std::array<double, 8>, 8> to_ret;
    for (size_t index = 0; index < input.size(); index++) {
        size_t row = index / 8;
        size_t col = index - index / 8 * 8;
        to_ret[row][col] = input[index];
    }
    return to_ret;
}

std::vector<double> array2Vector(std::array<std::array<double, 8>, 8> input) {
    std::vector<double> to_ret;
    for (size_t row_idx = 0; row_idx < input.size(); row_idx++)
        for (size_t col_idx = 0; col_idx < input[row_idx].size(); col_idx++)
            to_ret.push_back(input[row_idx][col_idx]);
    return to_ret;
}