#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <array>
#include <cassert>

#include <Windows.h>    // for u_char

/**
 * 获取伪 zig-zag 的工具函数，
 * 其实不需要实现真正的 zig-zag，只要去那几列就可以了
 * 如果想要使用真正的 zig-zag algorithm，可参照下方的 @see
 * @name: getZigZagSlash
 * @see: https://gist.github.com/skyleaworlder/94ffd831e6cafbe694f10e1c2acbe650
 *          zig-zag implemented by skyleaworlder
 */
std::vector<double> getZigZagSlash(std::array<std::array<double, 8>, 8> input, const size_t zig_num);

/**
 * vector2Array 将 vector 变成 C-array
 * @tparam T，一般为 int, double, char...
 * @param input: vector<T>，被转换
 * @param output: T*，输出结果
 */
template<typename T>
void vector2Array(std::vector<T> input, T* output) {
    for (size_t idx = 0; idx < input.size(); idx++)
        output[idx] = input[idx];
}
/**
 * vector2Array 将 vector 变成 Cpp-array
 * 但是这里只是 64 => 8*8
 * @param input: vector<double>，被转换的输入
 * @return array<array<double, 8>, 8> 输出
 */
std::array<std::array<double, 8>, 8> vector2Array(std::vector<double> input);

/**
 * array2Vector 将 C-array 变成 vector
 * @tparam T，一般为 int, double, char...
 * @param input: T*，被转换
 * @param output: vector<T>，输出结果
 */
template<typename T>
std::vector<T> array2Vector(T* input, const size_t size) {
    std::vector<T> to_ret;
    for (size_t idx = 0; idx < size; idx++)
        to_ret.push_back(input[idx]);
    return to_ret;
}
/**
 * array2Vector 将 Cpp-array 变成 vector
 * 但是这里只是 8*8 => 64
 * @param input: array<array<double, 8>, 8>，被转换的输入
 * @return vector<double> 输出
 */
std::vector<double> array2Vector(std::array<std::array<double, 8>, 8> input);

/**
 * int2UChar 把 int 数组转化为 u_char 数组
 * 原本的 int 数组 size 为 8k, 那么 u_char 数组的大小为 k
 * @param int_inp: int*, 输入的 int 数组
 * @param int_inp_size: int, 输入的 int 数组大小
 * @param uchar_out: u_char, 指向函数生成的 u_char 数组数据的指针
 */
void int2UChar(int* int_inp, const size_t int_inp_size, u_char* uchar_out);

#endif