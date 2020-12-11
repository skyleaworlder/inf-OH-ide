#ifndef ASS_HPP
#define ASS_HPP

#define SS_NAMESP_BEG namespace SS {
#define SS_NAMESP_END }

#include "DCT.hpp"
#include <array>
#include <vector>

using block_8_8 = std::array<std::array<double, 8>, 8>;

SS_NAMESP_BEG

enum IMBED_B {
    POS = 1,
    NEG = -1,
    ZER = 0
};

/**
 * @brief 对一块嵌入水印的小函数
 * @param block: block_8_8，输入的被嵌入图像
 * @param strength: double，嵌入强度
 * @param b: IMBED_B，数据隐藏参数，仅在 1 和 -1 之间取值
 * @param zig_num: size_t，表示从 DCT 系数矩阵中取出的斜列数
 *        一般而言，以 “/” 角度的斜对角线为中轴线，在另一斜对角线方向上延伸
 */
block_8_8 imbed(
    block_8_8 block,
    double strength,
    SS::IMBED_B b,
    size_t zig_num
);

/**
 * @brief 对一嵌入水印的小块解码
 * @param block: vector<double>，需要解码的块
 * @param zig_num: size_t，表示从 DCT 系数矩阵中取出的斜列数
 *        一般而言，以 “/” 角度的斜对角线为中轴线，在另一斜对角线方向上延伸
 */
bool decode(
    block_8_8 block,
    size_t zig_num
);

/**
 * @brief 获取 8*8 正方形矩阵中的 slash
 * @param block: block_8_8，输入的块
 * @param zig_num: size_t，在一个轴方向上的 slash num
 */
std::vector<double> _getZigZagSlash(
    block_8_8 input,
    size_t zig_num
);

/**
 * @brief 把 64 的 vector 变成 8*8 的 array
 * @param input: vector<double>
 * @return block_8_8
 */
block_8_8 _vector2array(std::vector<double>);

/**
 * @brief 把 8*8 的 array 变成 64 的 vector
 * @param input: vector<double>
 * @return block_8_8
 */
std::vector<double> _array2vector(block_8_8);

SS_NAMESP_END // namespace SS

#endif