#ifndef STDM_HPP
#define STDM_HPP

#include "DCT.hpp"
#include "utils.hpp"
#include "bmp.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <cmath>

#include <Windows.h>

/**
 * class STDM(Spread Transform Dither Modulation) obey SRP,
 *  only able to imbed, decode and calcu pfa
 */
class STDM {

private:
    /**
     * 量化步长，就是那个三角
     */
    double delta;

public:
    /**
     * @name: "静态域"
     */
    static const int pic_width;
    static const int pic_height;
    static const int logo_width;
    static const int logo_height;
    static const int pic_size;
    static const int logo_size;
    static const int block_size;

private:
    /**
     * 嵌入函数
     * @name imbed
     * @param block_inp: 类型为 double[64]，仅仅允许一个 64 大小的数组作为源
     * @param block_out: 类型为 double[64]，通过指针输出该函数的结果
     * @param b: 从 logo 二值图中取出的一位，d[b] 中的 b
     * @param zig_num: 默认为 3，zig-zag 取出的于一轴上的列数
     *    e.g. zig_num = 3, 说明 x 方向取了 3 列，y 方向取了 3 列，一共 3+3-1 列
     */
    void imbed(double block_inp[64], double block_out[64], int b, const size_t zig_num=3);

    /**
     * 解码函数
     * @name decode
     * @param block_inp: 类型为 double[64]，仅仅允许一个 64 大小的数组作为源
     * @param zig_num: 默认为 3，zig-zag 取出的于一轴上的列数
     * @return 类型为 bool，表示二值图像中的一个比特
     */
    bool decode(double block_inp[64], const size_t zig_num=3);

    /**
     * q 函数，由于函数体过于简单，作为内联函数
     * 实际为 slides 中的 q_{\delta}(x)
     * @name q_delta
     * @param delta: 类型为 const double，量化步长
     * @param x: 类型为 const double，输入
     * @return 类型为 double
     */
    inline double q_delta(const double delta, const double x) {
        return delta * floor(x / delta + 0.5);
    }

public:

    // constructor
    STDM(double delta): delta(delta) {};
    // destructor
    ~STDM() {};

    /**
     * @name getDelta: delta 的 get 方法
     * @return this->delta
     */
    constexpr inline double getDelta() {
        return this->delta;
    }

    /**
     * 嵌入外层函数
     * @param pic_inp: 类型为 double*，装 pic 的空间，需要首先分配
     * @param pic_imbed_out: 类型为 double*，指向函数执行结果所在的空间
     * @param logo_inp: 类型为 int*，装 logo 的空间，需要首先分配
     */
    void imbedAll(double* pic_inp, double* pic_imbed_out, int* logo_inp);

    /**
     * 解码外层函数
     * @param pic_imbed_inp: 类型为 double*，需装有被嵌入水印的图像
     * @param logo_inp: 类型为 int*，装 logo 的空间，需要首先分配
     */
    void decodeAll(double* pic_imbed_inp, int* logo_out);

    /**
     * 计算 pfa
     * @param logo_origin: 类型为 int*，源数据（类似 origin）
     * @param logo_decode: 类型为 int*，解码后的数据（类似 income）
     * @return 类型为 double，错误率 pfa
     */
    double calcuPfa(int* logo_origin, int* logo_decode);
};

#endif