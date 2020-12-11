#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath> // for sqrt
#include "ASS.hpp"

const std::vector<int> w = { -1, 1 };

std::vector<double> SS::imbed(
    std::vector<double> block,
    double strength,
    SS::IMBED_B b,
    size_t len=8,
    size_t zig_num=3
) {
    assert(block.size() == len*len);

    std::vector<double> DCT_block = DCT::DCT(block, len);

    // 获得全部 zig-zag 排列矩阵
    std::vector<double> zig_block = SS::_zig_zag(DCT_block);
    // 获取用来水印嵌入的 zig-zag 排列的部分
    std::vector<double> to_process = SS::_getSlashFromZigZag(zig_block, zig_num);

    // 处理，嵌入水印
    std::vector<double> imbed_arr;
    for (size_t index = 0; index < to_process.size(); index++)
        imbed_arr.push_back(to_process[index] + strength*(b ? 1 : -1) * w[index % 2]);

    // 将嵌入水印的 imbed_arr 嵌入到 block 中，形成返回结果
    std::vector<double> to_ret = _imbedSS2Block(block, imbed_arr);

    to_ret = DCT::DCT_inverse(to_ret, len);

    return to_ret;
}

std::vector<double> SS::decode(
    std::vector<double> block,
    size_t len=8,
    size_t zig_num=3
) {
    std::vector<double> DCT_block = DCT::DCT(block, len);
    size_t L_s = 0;

    std::vector<double> zig_block = SS::_zig_zag(block);
    std::vector<double> to_process = SS::_getSlashFromZigZag(block, zig_num);

    std::vector<double> decode_arr;
    for (size_t index = 0; index < to_process.size(); index++) {
        L_s += to_process[index] * w[index % 2];
    }

    if (L_s > 0) {
        std::cout << "true" << std::endl;
        return {};
    }
    else {
        std::cout << "false" << std::endl;
        return {};
    }
}

std::vector<double> SS::_zig_zag(
    std::vector<double> input
) {
    size_t len = static_cast<size_t>(sqrt(input.size()));
    assert(input.size() == len * len);

    std::vector<double> to_ret;
    size_t row_idx = 0, col_idx = 0;

    enum STATE {
        INIT,   RIGHT,      LEFT_DOWN,
        DOWN,   RIGHT_UP,   END
    };

    STATE next_step = STATE::INIT;
    while(next_step != STATE::END) {

        to_ret.push_back(input[row_idx*len + col_idx]);
        //std::cout << row_idx << ", " << col_idx << ": " << input[row_idx*len + col_idx] << std::endl; // debug
        // init
        if (next_step == STATE::INIT) {
            next_step = STATE::RIGHT;
            col_idx += 1;
        }
        // end
        else if (row_idx == (len-1) && col_idx == (len-1)) {
            next_step = STATE::END;
        }
        // corner
        else if (next_step == STATE::RIGHT_UP && col_idx == len-1 && row_idx == 0) {
            next_step = STATE::DOWN;
            row_idx += 1;
        }
        // corner
        else if (next_step == STATE::LEFT_DOWN && row_idx == len-1 && col_idx == 0) {
            next_step = STATE::RIGHT;
            col_idx += 1;
        }
        // left half || right half
        else if (
            (next_step == STATE::RIGHT && row_idx == 0) ||
            (next_step == STATE::DOWN && col_idx == len-1)
         ) {
            next_step = STATE::LEFT_DOWN;
            col_idx -= 1;
            row_idx += 1;
        }
        // left half || right half
        else if (
            (next_step == STATE::DOWN && col_idx == 0) ||
            (next_step == STATE::RIGHT && row_idx == len-1)
        ) {
            next_step = STATE::RIGHT_UP;
            row_idx -= 1;
            col_idx += 1;
        }
        // left half || right
        else if (
            (next_step == STATE::LEFT_DOWN && col_idx == 0) ||
            (next_step == STATE::RIGHT_UP && col_idx == len-1)
        ) {
            next_step = STATE::DOWN;
            row_idx += 1;
        }
        // left half || right half
        else if (
            (next_step == STATE::LEFT_DOWN && row_idx == len-1) ||
            (next_step == STATE::RIGHT_UP && row_idx == 0)
         ) {
            next_step = STATE::RIGHT;
            col_idx += 1;
        }
        // normal
        else if (next_step == STATE::LEFT_DOWN) {
            col_idx -= 1;
            row_idx += 1;
        }
        // normal
        else if (next_step == STATE::RIGHT_UP) {
            row_idx -= 1;
            col_idx += 1;
        }
        else {
            std::cout << "error!" << std::endl;
        }
    }
    return to_ret;
}

std::vector<double> SS::_getSlashFromZigZag(
    std::vector<double> input,
    size_t slash_num
) {
    size_t len = static_cast<size_t>(sqrt(input.size()));
    assert(slash_num <= len);

    std::vector<double> to_ret;
    size_t begin = ((len-slash_num+1)*(len-slash_num)) / 2;
    size_t end = input.size() - begin;
    std::cout << begin << " " << end << std::endl;
    for (size_t index = begin; index < end; index++)
        to_ret.push_back(input[index]);

    return to_ret;
}

std::vector<double> SS::_imbedSS2Block(
    std::vector<double> block,
    std::vector<double> imbed
) {
    // hahahaha
    size_t front_end = (block.size() - imbed.size()) / 2;
    size_t back_end = front_end + imbed.size();

    std::vector<double> to_ret;
    std::cout << front_end << " " << back_end << std::endl;
    for (size_t index = 0; index < block.size(); index++) {
        if (index < front_end || index >= back_end) {
            //std::cout << "c";
            to_ret.push_back(block[index]);
        }
        else {
            //std::cout << "s";
            to_ret.push_back(imbed[index-front_end]);
        }
    }
    return to_ret;
}