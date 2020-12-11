#ifndef ASS_HPP
#define ASS_HPP

#define SS_NAMESP_BEG namespace SS {
#define SS_NAMESP_END }

#include "DCT.hpp"
#include <vector>

SS_NAMESP_BEG

enum IMBED_B {
    POS = 1,
    NEG = -1,
    ZER = 0
};

/**
 * @brief 对一块嵌入水印的小函数
 * @param block: vector<double>，输入的被嵌入图像
 * @param strength: double，嵌入强度
 * @param b: IMBED_B，数据隐藏参数，仅在 1 和 -1 之间取值
 * @param len: size_t，表示嵌入的行，默认为8
 * @param zig_num: size_t，表示从 DCT 系数矩阵中取出的斜列数
 *        一般而言，以 “/” 角度的斜对角线为中轴线，在另一斜对角线方向上延伸
 */
std::vector<double> imbed(
    std::vector<double> block,
    double strength,
    SS::IMBED_B b,
    size_t len,
    size_t zig_num
);

/**
 * @brief 对一嵌入水印的小块解码
 * @param block: vector<double>，需要解码的块
 * @param len: size_t，表示嵌入的行，默认为8
 * @param zig_num: size_t，表示从 DCT 系数矩阵中取出的斜列数
 *        一般而言，以 “/” 角度的斜对角线为中轴线，在另一斜对角线方向上延伸
 */
std::vector<double> decode(
    std::vector<double> block,
    size_t len,
    size_t zig_num
);

/**
 * @brief Zig-zag Algothrim 获得全部 zig-zag 排列矩阵
 * @param input: vector<double>
 * @return vector<double> (zig-zag vector)
 * @see it was me that make this func public: https://gist.github.com/skyleaworlder/94ffd831e6cafbe694f10e1c2acbe650
 */
std::vector<double> _zig_zag(std::vector<double> input);

/**
 * @brief 获取指定斜列的元素
 * @param input: vector<double> zig-zag vector
 * @param slash_num: size_t，指定在一个轴上选取的斜列数量
 *                   e.g. slash_num = 3, 真实选中了 3*2-1 = 5 个斜列
 *                        因为包含了横向和纵向的
 * @return 返回指定的元素组成的 vector
 */
std::vector<double> _getSlashFromZigZag(
    std::vector<double> input,
    size_t slash_num
);

/**
 * @brief 把嵌入水印的部分放入原图像
 * @param block: vector<double> 原图像
 * @param imbed: vector<double> 嵌入水印的部分(zig-zag)
 * @return vector<double> 嵌入水印后的图像 block
 */
std::vector<double> _imbedSS2Block(
    std::vector<double> block,
    std::vector<double> imbed
);

SS_NAMESP_END // namespace SS

#endif